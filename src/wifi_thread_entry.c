#include "wifi_thread.h"
#include "app_types.h"
#include <stdio.h>
#include <string.h>

/* Semaphore đồng bộ cho UART TX hoàn tất */
static SemaphoreHandle_t g_uart_tx_done;
static StaticSemaphore_t g_uart_tx_done_memory;

/* Hàm Callback cho UART (Do FSP yêu cầu) */
void user_uart_callback(uart_callback_args_t *p_args)
{
    if (p_args->event == UART_EVENT_TX_COMPLETE)
    {
        /* Báo hiệu cho WiFi Thread biết là UART đã truyền xong */
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(g_uart_tx_done, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/* WiFi Thread entry function */
void wifi_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    EdgeMessage_t incoming_msg;
    char json_payload[UART_BUFFER_SIZE];
    fsp_err_t err;

    /* 0. Tạo semaphore đồng bộ UART TX */
    g_uart_tx_done = xSemaphoreCreateBinaryStatic(&g_uart_tx_done_memory);

    /* 1. Mở cổng UART giao tiếp với ESP32 */
    err = R_SCI_UART_Open(&g_uart_esp_ctrl, &g_uart_esp_cfg);
    if (err != FSP_SUCCESS)
    {
        /* UART khởi tạo thất bại -> Không thể giao tiếp ESP32, dừng thread */
        vTaskDelete(NULL);
        return;
    }

    while (1)
    {
        /* 2. Chờ nhận gói tin từ AI Task thông qua g_wifi_queue */
        if (xQueueReceive(g_wifi_queue, &incoming_msg, portMAX_DELAY) == pdPASS)
        {
            /* 3. Đóng gói dữ liệu thành chuỗi JSON */
            /* Lưu ý: Bắt buộc phải có ký tự \n ở cuối để ESP32 dùng hàm readStringUntil('\n') */
            snprintf(json_payload, sizeof(json_payload), 
                     "{\"eco2\":%.1f,\"tvoc\":%.1f,\"iaq\":%.1f,\"pred_tvoc\":%.1f}\n",
                     incoming_msg.current_data.eco2,
                     incoming_msg.current_data.tvoc,
                     incoming_msg.current_data.iaq,
                     incoming_msg.predicted_future_tvoc);

            /* 4. Đẩy chuỗi JSON sang UART cho ESP32 */
            err = R_SCI_UART_Write(&g_uart_esp_ctrl, (uint8_t *)json_payload, (uint32_t)strlen(json_payload));

            if (err == FSP_SUCCESS)
            {
                /* Chờ UART truyền xong thật sự thông qua semaphore (timeout 500ms) */
                xSemaphoreTake(g_uart_tx_done, pdMS_TO_TICKS(500));
            }
        }
    }
}

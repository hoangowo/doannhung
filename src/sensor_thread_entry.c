#include "sensor_thread.h"
#include "app_types.h"

/* Biến lưu trữ dữ liệu thô và dữ liệu đã qua tính toán của ZMOD4410 */
static rm_zmod4xxx_raw_data_t zmod_raw_data;
static rm_zmod4xxx_iaq_2nd_data_t zmod_iaq_data;

/* Hàm Callback cho I2C của ZMOD4410 (Do FSP yêu cầu) */
void zmod4xxx_comms_i2c_callback(rm_zmod4xxx_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    /* Báo hiệu Semaphore khi I2C giao tiếp xong (nếu dùng) */
    BaseType_t context_switch;
    xSemaphoreGiveFromISR(g_zmod_semaphore, &context_switch);
    portYIELD_FROM_ISR(context_switch);
}

/* Hàm Callback cho ngắt IRQ của ZMOD4410 (Do FSP yêu cầu) */
void zmod4xxx_irq_callback(rm_zmod4xxx_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    /* Báo hiệu Semaphore khi ngắt xảy ra */
    BaseType_t context_switch;
    xSemaphoreGiveFromISR(g_zmod_semaphore, &context_switch);
    portYIELD_FROM_ISR(context_switch);
}

/* Sensor Thread entry function */
void sensor_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    SensorData_t sensor_data;
    fsp_err_t err;

    /* 1. Mở kết nối I2C và khởi tạo ZMOD4410 */
    err = g_zmod4xxx_sensor0.p_api->open(g_zmod4xxx_sensor0.p_ctrl, g_zmod4xxx_sensor0.p_cfg);
    if (err != FSP_SUCCESS) {
        /* Khởi tạo cảm biến thất bại -> Dừng thread, tránh gọi API liên tục */
        vTaskDelete(NULL);
        return;
    }

    while (1)
    {
        /* 2. Kích hoạt quá trình sấy (heating) và đo của cảm biến */
        err = g_zmod4xxx_sensor0.p_api->measurementStart(g_zmod4xxx_sensor0.p_ctrl);
        
        if (err == FSP_SUCCESS)
        {
            /* Cảm biến ZMOD4410 cần khoảng 1.99 giây để sấy và đo xong (cho chuẩn IAQ 2nd Gen) */
            vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_INTERVAL_MS));
            
            /* 3. Kiểm tra xem quá trình đo đã hoàn tất chưa */
            err = g_zmod4xxx_sensor0.p_api->statusCheck(g_zmod4xxx_sensor0.p_ctrl);
            if (err == FSP_SUCCESS)
            {
                /* 4. Đọc dữ liệu thô (ADC bytes) từ cảm biến qua I2C */
                err = g_zmod4xxx_sensor0.p_api->read(g_zmod4xxx_sensor0.p_ctrl, &zmod_raw_data);
                
                if (err == FSP_SUCCESS)
                {
                    /* 5. Chạy thư viện toán học của Renesas để tính ra eCO2, TVOC, IAQ */
                    err = g_zmod4xxx_sensor0.p_api->iaq2ndGenDataCalculate(g_zmod4xxx_sensor0.p_ctrl, &zmod_raw_data, &zmod_iaq_data);
                    
                    if (err == FSP_SUCCESS)
                    {
                        /* 6. Chuyển dữ liệu vào cấu trúc của chúng ta để gửi cho AI Task */
                        sensor_data.eco2 = zmod_iaq_data.eco2;
                        sensor_data.tvoc = zmod_iaq_data.tvoc;
                        sensor_data.iaq  = zmod_iaq_data.iaq;

                        /* Gửi dữ liệu vào g_sensor_queue */
                        if (xQueueSend(g_sensor_queue, &sensor_data, pdMS_TO_TICKS(10)) != pdPASS)
                        {
                            /* Xử lý khi hàng đợi đầy */
                        }
                    }
                }
            }
        }
        
        /* Nghỉ một chút trước khi đo chu kỳ tiếp theo */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

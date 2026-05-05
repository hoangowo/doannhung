#include "ai_thread.h"
#include "app_types.h"

/* --- THAM SỐ AI TỪ GOOGLE COLAB (KẾT QUẢ HUẤN LUYỆN) --- */
/* Hệ số cho eCO2 (field5) */
static const float ai_w1 = 0.6667930273802654f;
/* Hệ số cho TVOC hiện tại (field6) */
static const float ai_w2 = -3.8307690556863823f;
/* Sai số chặn (Intercept) */
static const float ai_intercept = -251.90307790369928f;

/* --- HÀM TIỀN XỬ LÝ: BỘ LỌC TRUNG BÌNH (MOVING AVERAGE) --- */
#define FILTER_WINDOW_SIZE 5
static float eco2_history[FILTER_WINDOW_SIZE] = {0};
static float tvoc_history[FILTER_WINDOW_SIZE] = {0};
static uint8_t history_index = 0;
static uint32_t sample_count = 0;

static void moving_average_filter(float *current_eco2, float *current_tvoc)
{
    /* Lưu giá trị mới vào mảng lịch sử (circular buffer) */
    eco2_history[history_index] = *current_eco2;
    tvoc_history[history_index] = *current_tvoc;
    
    history_index++;
    if (history_index >= FILTER_WINDOW_SIZE) {
        history_index = 0;
    }
    sample_count++;

    /* Mẫu đầu tiên: trả về giá trị gốc, chưa cần lọc */
    if (sample_count == 1) {
        return; 
    }

    /* Tính số lượng mẫu hợp lệ để tính trung bình */
    uint8_t count = (sample_count >= FILTER_WINDOW_SIZE) ? FILTER_WINDOW_SIZE : (uint8_t)sample_count;

    /* Tính trung bình cộng */
    float sum_eco2 = 0.0f;
    float sum_tvoc = 0.0f;

    for (uint8_t i = 0; i < count; i++) {
        sum_eco2 += eco2_history[i];
        sum_tvoc += tvoc_history[i];
    }

    *current_eco2 = sum_eco2 / count;
    *current_tvoc = sum_tvoc / count;
}

/**
 * Hàm dự đoán TVOC trong tương lai (Inference)
 * Được tính toán trực tiếp trên CPU Cortex-M33 của RA6M5.
 */
static float predict_future_tvoc(float current_eco2, float current_tvoc)
{
    float prediction = (current_eco2 * ai_w1) + (current_tvoc * ai_w2) + ai_intercept;

    /* Ràng buộc dữ liệu: TVOC không thể là số âm */
    if (prediction < 0.0f) {
        prediction = 0.0f;
    }
    return prediction;
}

/**
 * AI Thread Entry Function
 * Nhận dữ liệu từ Sensor Task -> Tiền xử lý -> Dự đoán -> Gửi sang WiFi Task.
 */
void ai_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    SensorData_t raw_data;      // Biến tạm chứa dữ liệu thô nhận được
    EdgeMessage_t out_msg;      // Gói tin chứa cả data thực và data dự đoán

    while (1)
    {
        /* 1. Đợi dữ liệu từ Sensor Task gửi qua g_sensor_queue */
        if (xQueueReceive(g_sensor_queue, &raw_data, portMAX_DELAY) == pdPASS)
        {
            /* 2. Thực hiện Tiền xử lý (Preprocessing) - Lọc nhiễu trung bình */
            float filtered_eco2 = raw_data.eco2;
            float filtered_tvoc = raw_data.tvoc;
            moving_average_filter(&filtered_eco2, &filtered_tvoc);

            /* Cập nhật lại dữ liệu đã lọc sạch vào raw_data (để đẩy lên server cho đẹp) */
            raw_data.eco2 = filtered_eco2;
            raw_data.tvoc = filtered_tvoc;

            /* 3. Chạy mô hình dự đoán (Inference) */
            float predicted_val = predict_future_tvoc(filtered_eco2, filtered_tvoc);

            /* 4. Đóng gói gói tin Edge AI */
            out_msg.current_data = raw_data;
            out_msg.predicted_future_tvoc = predicted_val;

            /* 5. Gửi dữ liệu sang WiFi Task để đẩy lên Server */
            if (xQueueSend(g_wifi_queue, &out_msg, pdMS_TO_TICKS(10)) != pdPASS)
            {
                /* Log lỗi hoặc xử lý khi hàng đợi đầy (Overflow) */
            }
        }
    }
}

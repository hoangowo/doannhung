#ifndef APP_TYPES_H_
#define APP_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* --------------------------------------------------------------------------
 * 1. ĐỊNH NGHĨA CẤU TRÚC DỮ LIỆU CẢM BIẾN
 * Chứa các thông số thô đọc từ ZMOD4410
 * -------------------------------------------------------------------------- */
typedef struct {
    float eco2;         // Đơn vị: ppm
    float tvoc;         // Đơn vị: ppb
    float iaq;          // Chỉ số chất lượng không khí (IAQ Index)
} SensorData_t;

/* --------------------------------------------------------------------------
 * 2. ĐỊNH NGHĨA GÓI TIN AI (EDGE MESSAGE)
 * Bao gồm dữ liệu thực tế và kết quả dự đoán của mô hình AI
 * -------------------------------------------------------------------------- */
typedef struct {
    SensorData_t current_data;      // Dữ liệu hiện tại
    float predicted_future_tvoc;    // Kết quả "tiên tri" TVOC từ mô hình AI
} EdgeMessage_t;

/* --------------------------------------------------------------------------
 * 3. KHAI BÁO BIẾN TOÀN CỤC (EXTERN)
 * Giúp các Thread có thể truy cập chung vào hàng đợi (Queue)
 * -------------------------------------------------------------------------- */

// Hàng đợi truyền từ Sensor Task -> AI Task
extern QueueHandle_t g_sensor_queue;

// Hàng đợi truyền từ AI Task -> WiFi Task
extern QueueHandle_t g_wifi_queue;

// Semaphore đồng bộ ngắt IRQ cho cảm biến
extern SemaphoreHandle_t g_zmod_semaphore;

/* --------------------------------------------------------------------------
 * 4. CẤU HÌNH HỆ THỐNG
 * -------------------------------------------------------------------------- */
#define SENSOR_READ_INTERVAL_MS    (2000) // 2 giây đọc cảm biến 1 lần
#define UART_BUFFER_SIZE           (256)  // Kích thước bộ đệm chuỗi JSON

#endif /* APP_TYPES_H_ */

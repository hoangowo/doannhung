/* generated common source file - do not edit */
#include "common_data.h"
icu_instance_ctrl_t g_external_irq0_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_external_irq0_ext_cfg = { .filter_src =
		EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV, };

const external_irq_cfg_t g_external_irq0_cfg = { .channel = 1, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING, .filter_enable = true, .clock_source_div =
		EXTERNAL_IRQ_CLOCK_SOURCE_DIV_8, .p_callback = rm_zmod4xxx_irq_callback,
/** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
		.p_context = (void*) &NULL,
#endif
		.p_extend = (void*) &g_external_irq0_ext_cfg, .ipl = (4),
#if defined(VECTOR_NUMBER_ICU_IRQ1)
    .irq                 = VECTOR_NUMBER_ICU_IRQ1,
#else
		.irq = FSP_INVALID_VECTOR,
#endif
		};
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 = { .p_ctrl =
		&g_external_irq0_ctrl, .p_cfg = &g_external_irq0_cfg, .p_api =
		&g_external_irq_on_icu };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c0_ctrl;
const sci_i2c_extended_cfg_t g_i2c0_cfg_extend =
		{
				/* Actual calculated bitrate: 399780. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value =
						0, .clock_settings.brr_value = 3,
				.clock_settings.mddr_value = 131,
				.clock_settings.bitrate_modulation = true,
				.clock_settings.cycles_value = 30, .clock_settings.snfr_value =
						(1), };

const i2c_master_cfg_t g_i2c0_cfg = { .channel = 1,
		.rate = I2C_MASTER_RATE_FAST, .slave = 0x00, .addr_mode =
				I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
		.p_transfer_tx = NULL,
#else
    .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
		.p_transfer_rx = NULL,
#else
    .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
		.p_callback = rm_comms_i2c_callback, .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI1_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI1_RXI,
#else
		.rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI1_TXI,
#else
		.txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI1_TEI,
#else
		.tei_irq = FSP_INVALID_VECTOR,
#endif
		.ipl = (4), /* (BSP_IRQ_DISABLED) is unused */
		.p_extend = &g_i2c0_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c0 = { .p_ctrl = &g_i2c0_ctrl, .p_cfg =
		&g_i2c0_cfg, .p_api = &g_i2c_master_on_sci };
#if BSP_CFG_RTOS
#if BSP_CFG_RTOS == 1
#if !defined(g_comms_i2c_bus0_recursive_mutex)
TX_MUTEX g_comms_i2c_bus0_recursive_mutex_handle;
CHAR g_comms_i2c_bus0_recursive_mutex_name[] = "g_comms_i2c_bus0 recursive mutex";
#endif
#if !defined(g_comms_i2c_bus0_blocking_semaphore)
TX_SEMAPHORE g_comms_i2c_bus0_blocking_semaphore_handle;
CHAR g_comms_i2c_bus0_blocking_semaphore_name[] = "g_comms_i2c_bus0 blocking semaphore";
#endif
#elif BSP_CFG_RTOS == 2
#if !defined(g_comms_i2c_bus0_recursive_mutex)
SemaphoreHandle_t g_comms_i2c_bus0_recursive_mutex_handle;
StaticSemaphore_t g_comms_i2c_bus0_recursive_mutex_memory;
#endif
#if !defined(g_comms_i2c_bus0_blocking_semaphore)
SemaphoreHandle_t g_comms_i2c_bus0_blocking_semaphore_handle;
StaticSemaphore_t g_comms_i2c_bus0_blocking_semaphore_memory;
#endif
#endif

#if !defined(g_comms_i2c_bus0_recursive_mutex)
/* Recursive Mutex for I2C bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus0_recursive_mutex =
{
    .p_mutex_handle = &g_comms_i2c_bus0_recursive_mutex_handle,
#if BSP_CFG_RTOS == 1 // ThradX
    .p_mutex_name = &g_comms_i2c_bus0_recursive_mutex_name[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    .p_mutex_memory = &g_comms_i2c_bus0_recursive_mutex_memory,
#endif
};
#endif

#if !defined(g_comms_i2c_bus0_blocking_semaphore)
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus0_blocking_semaphore =
{
    .p_semaphore_handle = &g_comms_i2c_bus0_blocking_semaphore_handle,
#if BSP_CFG_RTOS == 1 // ThreadX
    .p_semaphore_name = &g_comms_i2c_bus0_blocking_semaphore_name[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    .p_semaphore_memory = &g_comms_i2c_bus0_blocking_semaphore_memory,
#endif
};
#endif
#endif

/* Shared I2C Bus */
#define RA_NOT_DEFINED (1)
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus0_extended_cfg = {
#if !defined(RA_NOT_DEFINED)
    .p_driver_instance      = (void*)&RA_NOT_DEFINED,
#elif !defined(g_i2c0)
		.p_driver_instance = (void*) &g_i2c0,
#elif !defined(RA_NOT_DEFINED)
    .p_driver_instance      = (void*)&RA_NOT_DEFINED,
#endif
		.p_current_ctrl = NULL, .bus_timeout = 0xFFFFFFFF,
#if BSP_CFG_RTOS
#if !defined(g_comms_i2c_bus0_blocking_semaphore)
    .p_blocking_semaphore = &g_comms_i2c_bus0_blocking_semaphore,
#if !defined(g_comms_i2c_bus0_recursive_mutex)
    .p_bus_recursive_mutex = &g_comms_i2c_bus0_recursive_mutex,
#else
    .p_bus_recursive_mutex = NULL,
#endif
#else
    .p_bus_recursive_mutex = NULL,
    .p_blocking_semaphore = NULL,
#endif
#else
#endif

#if (0)
    .p_elc = (void*)&g_elc,
    .p_timer = (void*)&g_timer,
#else
		.p_elc = NULL, .p_timer = NULL,
#endif
		};
#undef RA_NOT_DEFINED
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport = { .p_api = &g_ioport_on_ioport, .p_ctrl =
		&g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
QueueHandle_t g_sensor_queue;
#if 1
StaticQueue_t g_sensor_queue_memory;
uint8_t g_sensor_queue_queue_memory[12 * 5];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_wifi_queue;
#if 1
StaticQueue_t g_wifi_queue_memory;
uint8_t g_wifi_queue_queue_memory[16 * 5];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
SemaphoreHandle_t g_zmod_semaphore;
#if 1
StaticSemaphore_t g_zmod_semaphore_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void) {
	g_sensor_queue =
#if 1
			xQueueCreateStatic(
#else
                xQueueCreate(
                #endif
					5, 12
#if 1
					, &g_sensor_queue_queue_memory[0], &g_sensor_queue_memory
#endif
					);
	if (NULL == g_sensor_queue) {
		rtos_startup_err_callback(g_sensor_queue, 0);
	}
	g_wifi_queue =
#if 1
			xQueueCreateStatic(
#else
                xQueueCreate(
                #endif
					5, 16
#if 1
					, &g_wifi_queue_queue_memory[0], &g_wifi_queue_memory
#endif
					);
	if (NULL == g_wifi_queue) {
		rtos_startup_err_callback(g_wifi_queue, 0);
	}
	g_zmod_semaphore =
#if 1
			xSemaphoreCreateCountingStatic(
#else
                xSemaphoreCreateCounting(
                #endif
					256, 0
#if 1
					, &g_zmod_semaphore_memory
#endif
					);
	if (NULL == g_zmod_semaphore) {
		rtos_startup_err_callback(g_zmod_semaphore, 0);
	}
}

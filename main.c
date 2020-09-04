/**
 * @brief GNSS parser (tested with UBLOX NEO-6M)
*/

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "boards.h"
#include "app_timer.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_timer.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_uart.h"
#include "nrf_uart.h"
#include "nrf_delay.h"

#include "gnss.h"

#define DEAD_BEEF                       0xDEADBEEF

/* Timer instance */
const nrf_drv_timer_t TIMER1 = NRF_DRV_TIMER_INSTANCE(1);

static struct gps_ctx_t gps_ctx;

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

static void init_system(void)
{
    // Init logs
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    /* Init timers */
    APP_ERROR_CHECK(app_timer_init());

    /* Init power management */
    APP_ERROR_CHECK(nrf_pwr_mgmt_init());
}

static void timer1_handler(nrf_timer_event_t event_type, void* p_context)
{
    /* GNSS infos could be used or displayed here */
    NRF_LOG_INFO("UTC time %d", gps_ctx.gga.utc);
}


static void init_main_timer(void)
{
    uint32_t time_ticks, time_ms = 1000;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = NRF_TIMER_FREQ_31250Hz;

    APP_ERROR_CHECK(nrf_drv_timer_init(&TIMER1, &timer_cfg, timer1_handler));

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER1, time_ms);
    nrf_drv_timer_extended_compare(&TIMER1, NRF_TIMER_CC_CHANNEL0,
            time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER1);
}


int main(void)
{
    init_system();
    init_main_timer();
    gnss_init(&gps_ctx);

    NRF_LOG_INFO("======= INIT SUCCEED =======");

    for (;;) {
        gnss_get_msg(&gps_ctx);
        if (NRF_LOG_PROCESS() == false)
            nrf_pwr_mgmt_run();
    }
}

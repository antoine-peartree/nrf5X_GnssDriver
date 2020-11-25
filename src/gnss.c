#include <string.h>
#include <stdint.h>

#include "boards.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "app_uart.h"
#include "nrf_uart.h"
#include "nrf_delay.h"

#include "gnss.h"


static bool gnss_compare_crc(char *msg)
{
    if (strlen(msg) < 4 || msg[strlen(msg) - 3] != '*')
        return false;

    int msg_crc = 0, calc_crc = 0;

    sscanf(msg + strlen(msg) - 2, "%x", &msg_crc);
    for (int i = 1; i < strlen(msg) - 3; i++)
        calc_crc ^= msg[i];

    return msg_crc == calc_crc;
}


void gnss_get_msg(struct gps_ctx_t *gps_ctx)
{
    uint8_t cr = '\0';
    if (app_uart_get(&cr) != NRF_SUCCESS || cr != '$')
        return;

    if (!gps_ctx->is_initialized) {
        NRF_LOG_INFO("First NMEA msg received from GNSS module");
        gps_ctx->is_initialized = 1;
    }

    uint8_t cnt = 0;
    static char nmea_msg[NMEA_MSG_MAX_LENGTH];
    memset(nmea_msg, '\0', sizeof(nmea_msg));
    nmea_msg[cnt++] = cr;
    while(cnt < NMEA_MSG_MAX_LENGTH) {
        while (app_uart_get(&cr) != NRF_SUCCESS);
        if (cr == '\r')
            break;
        nmea_msg[cnt++] = cr;
    }

    if (gnss_compare_crc(nmea_msg))
        gnss_parse(nmea_msg, gps_ctx);
}

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
        NRF_LOG_INFO("Comm error");
    } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
        NRF_LOG_INFO("Fifo error");
    } else if (p_event->evt_type == APP_UART_DATA ||
                p_event->evt_type == APP_UART_DATA_READY) {
        // Data received, no error to log
    } else if (p_event->evt_type == APP_UART_DATA) {
        // Tx completed, no error to log
    } else {
        NRF_LOG_INFO("Unknown UART error");
    }
}


static void gnss_init_uart(void)
{
    uint32_t err_code;
    const app_uart_comm_params_t comm_params = {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          NRF_UART_HWFC_DISABLED,
          false,
          NRF_UART_BAUDRATE_9600
    };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);
    nrf_delay_ms(20);

    APP_ERROR_CHECK(err_code);
}

void gnss_init(struct gps_ctx_t *gps_ctx)
{
    gnss_init_uart();
    memset(gps_ctx, 0, sizeof(gps_ctx));
}

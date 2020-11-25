#pragma once

#include "gnss_parser.h"

/* GNSS specific constants */
#define UART_TX_BUF_SIZE 255
#define UART_RX_BUF_SIZE 8096

#define NMEA_MSG_MAX_LENGTH 82


void gnss_get_msg(struct gps_ctx_t *gps_ctx);

void gnss_init(struct gps_ctx_t *gps_ctx);

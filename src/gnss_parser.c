#include <string.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "../includes/gnss_parser.h"


static void gnss_parse_gga(char *nmea_msg, struct gga_t *gga)
{
    strtok(nmea_msg, ",");
    gga->utc = atoi(strtok(NULL, ","));
    gga->lat = atof(strtok(NULL, ","));
    gga->lat_dir = strtok(NULL, ",")[0];
    gga->lon = atof(strtok(NULL, ","));
    gga->lon_dir = strtok(NULL, ",")[0];
    gga->quality = atoi(strtok(NULL, ","));
    gga->nb_sat = atoi(strtok(NULL, ","));
    gga->hdop = atof(strtok(NULL, ","));
    gga->height = atof(strtok(NULL, ","));
    gga->height_unit = strtok(NULL, ",")[0];
    gga->geoid_separation = atof(strtok(NULL, ","));
    gga->geoid_unit = strtok(NULL, ",")[0];
    gga->diff_gps_age = atof(strtok(NULL, ","));
    gga->station_ID = atoi(strtok(NULL, ","));
}


static void gnss_parse_gll(char *nmea_msg, struct gll_t *gll)
{
    strtok(nmea_msg, ",");
    gll->lat = atof(strtok(NULL, ","));
    gll->lat_dir = strtok(NULL, ",")[0];
    gll->lon = atof(strtok(NULL, ","));
    gll->lon_dir = strtok(NULL, ",")[0];
    gll->utc = atoi(strtok(NULL, ","));
    gll->data_valid = strtok(NULL, ",")[0];
}


static void gnss_parse_gsv(char *nmea_msg, struct gsv_t *gsv)
{
    strtok(nmea_msg, ",");
    gsv->nb_gsv_msgs_in_cycle = atoi(strtok(NULL, ","));
    gsv->msg_number = atoi(strtok(NULL, ","));
    gsv->nb_sv_in_view = atoi(strtok(NULL, ","));
    for (int i = 0; i < 4; i++) {
        gsv->sv_info[i].prn = atoi(strtok(NULL, ","));
        gsv->sv_info[i].elevation = atoi(strtok(NULL, ","));
        gsv->sv_info[i].azimuth = atoi(strtok(NULL, ","));
        gsv->sv_info[i].snr = atoi(strtok(NULL, ","));
    }
}


static void gnss_parse_vtg(char *nmea_msg, struct vtg_t *vtg)
{
    strtok(nmea_msg, ",");
    vtg->true_track = atof(strtok(NULL, ","));
    vtg->true_indic = strtok(NULL, ",")[0];
    vtg->track_magnetic = atof(strtok(NULL, ","));
    vtg->magnetic_indic = strtok(NULL, ",")[0];
    vtg->speed_knots = atof(strtok(NULL, ","));
    vtg->knots_indic = strtok(NULL, ",")[0];
    vtg->speed_kph = atof(strtok(NULL, ","));
    vtg->kph_indic = strtok(NULL, ",")[0];
    vtg->data_valid = strtok(NULL, ",")[0];
}


static void gnss_parse_rmc(char *nmea_msg, struct rmc_t *rmc)
{
    strtok(nmea_msg, ",");
    rmc->utc = atoi(strtok(NULL, ","));
    rmc->data_valid = strtok(NULL, ",")[0];
    rmc->lat = atof(strtok(NULL, ","));
    rmc->lat_dir = strtok(NULL, ",")[0];
    rmc->lon = atof(strtok(NULL, ","));
    rmc->lon_dir = strtok(NULL, ",")[0];
    rmc->speed_knots = atof(strtok(NULL, ","));
    rmc->true_course = atof(strtok(NULL, ","));
    rmc->date = atoi(strtok(NULL, ","));
    rmc->variation = atof(strtok(NULL, ","));
    rmc->variation_dir = strtok(NULL, ",")[0];
    rmc->pos_mode = strtok(NULL, ",")[0];
}


static void gnss_parse_gsa(char *nmea_msg, struct gsa_t *gsa)
{
    strtok(nmea_msg, ",");
    gsa->selection = strtok(NULL, ",")[0];
    gsa->fix_value = atoi(strtok(NULL, ","));
    for (int i = 0; i < 12; i++)
        gsa->prns[i] = atoi(strtok(NULL, ","));
    gsa->pdop = atof(strtok(NULL, ","));
    gsa->hdop = atof(strtok(NULL, ","));
    gsa->vdop = atof(strtok(NULL, ","));
}


void gnss_parse(char *nmea_msg, struct gps_ctx_t *gps_ctx)
{
    if (!strncmp(nmea_msg + 3, "GGA", 3)) {
        gnss_parse_gga(nmea_msg, &gps_ctx->gga);
    } else if (!strncmp(nmea_msg + 3, "GLL", 3)) {
        gnss_parse_gll(nmea_msg, &gps_ctx->gll);
    } else if (!strncmp(nmea_msg + 3, "GSV", 3)) {
        gnss_parse_gsv(nmea_msg, &gps_ctx->gsv);
    } else if (!strncmp(nmea_msg + 3, "VTG", 3)) {
        gnss_parse_vtg(nmea_msg, &gps_ctx->vtg);
    } else if (!strncmp(nmea_msg + 3, "RMC", 3)) {
        gnss_parse_rmc(nmea_msg, &gps_ctx->rmc);
    } else if (!strncmp(nmea_msg + 3, "GSA", 3)) {
       gnss_parse_gsa(nmea_msg, &gps_ctx->gsa);
    } else if (!strncmp(nmea_msg + 3, "TXT", 3)) {
        NRF_LOG_INFO("Txt msg received : %s", nmea_msg);
    } else {
        NRF_LOG_INFO("%s msgs non recognized", strtok(nmea_msg, ","));
    }
    NRF_LOG_FLUSH();
}

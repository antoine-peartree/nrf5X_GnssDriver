#pragma once

/* Global Positioning System Fix Data */
struct gga_t {
    uint32_t utc;   /* UTC hour in hhmmss format */
    double lat;     /* Latitude (degrees) */
    char lat_dir;   /* Latitude direction */
    double lon;     /* Longitude (degrees) */
    char lon_dir;   /* Longitude direction */
    uint32_t quality;
    uint32_t nb_sat;
    float hdop;
    float height;
    char height_unit;
    float geoid_separation;
    char geoid_unit;
    float diff_gps_age;
    uint32_t station_ID;
};

/* Geographic Position (latitude/longitude) and time */
struct gll_t {
    double lat;         /* Latitude (degrees) */
    char lat_dir;       /* Latitude direction */
    double lon;         /* Longitude (degrees) */
    char lon_dir;       /* Longitude direction */
    uint32_t utc;       /* UTC hour in hhmmss format */
    char data_valid;    /* validity - A=OK */
};

/* GNSS satellites in view */
struct gsv_t {
    uint32_t nb_gsv_msgs_in_cycle; /* Total number of GSV msgs in this cycle */
    uint32_t msg_number;
    uint32_t nb_sv_in_view;
    struct sv_infos_t {
        uint32_t prn;       /* SV PRN (identification number) */
        uint32_t elevation; /* Elevation in degrees, 90 maximum */
        uint32_t azimuth;   /* Azimuth, degrees from true north, 000 to 359 */
        uint32_t snr;       /* Signal/Noise 00-99 dB (null when no tracking) */
    } sv_info[4];
    struct sv_infos_t second_sv;
    struct sv_infos_t third_sv;
    struct sv_infos_t fourth_sv;
};

/* Recommended minimum specific GPS/Transit data */
struct rmc_t {
    uint32_t utc;       /* UTC hour in hhmmss format */
    char data_valid;    /* validity - A-ok, V-invalid */
    double lat;         /* Latitude (degrees) */
    char lat_dir;       /* Latitude direction */
    double lon;         /* Longitude (degrees) */
    char lon_dir;       /* Longitude direction */
    double speed_knots; /* Speed over ground (knots) */
    double true_course; /* Course Made Good, True */
    uint32_t date;      /* date in ddmmaa format */
    double variation;   /* Magnetic variation */
    char variation_dir; /* Magnetic variation direction */
    char pos_mode;      /* A = autonom, D=DGPS, E=DR */
};

/* GNSS DOP and active satellites */
struct gsa_t {
    char selection;     /* Selection of 2D/3D fix (A=auto M=manual) */
    uint32_t fix_value; /* 1=No fix 2=2D fix 3=3D fix*/
    uint32_t prns[12];  /* PRN numbers */
    float pdop;         /* PDOP: 0.5 through 99.9 */
    float hdop;         /* HDOP: 0.5 through 99.9 */
    float vdop;         /* VDOP: 0.5 through 99.9 */
};

/* Track Made Good and Ground Speed */
struct vtg_t {
    double true_track;      /* track made good (degrees true) */
    char true_indic;        /* T: track is relative to true north */
    double track_magnetic;  /* Track made good (degrees magnetic) */
    char magnetic_indic;    /* M: track is relative to magnetic north */
    double speed_knots;     /* Ground speed, knots */
    char knots_indic;       /* N: speed is measured in knots */
    double speed_kph;       /* Ground speed, Kilometers per hour */
    char kph_indic;         /* K: speed over ground is measured in kph */
    char data_valid;        /* validity - A=OK */
};

/* GNSS context. Used to keep last GNSS infos from GNSS module msgs*/
struct gps_ctx_t {
    uint8_t is_initialized;
    struct gga_t gga;
    struct gll_t gll;
    struct gsv_t gsv;
    struct rmc_t rmc;
    struct gsa_t gsa;
    struct vtg_t vtg;
};

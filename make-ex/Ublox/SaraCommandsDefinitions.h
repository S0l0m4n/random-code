/**
 * @file SaraCommandsDefinitions.h
 *
 * @brief Defines constants and types used in parsing responses to AT commands
 *
 * https://www.wagz.com
 */

/* --- GNSS power management: +UGPS --- */
/* mode */
#define UGPS_MODE_OFF (0)
#define UGPS_MODE_ON (1)

/* aid_mode */
#define UGPS_AID_MODE_OFF (0)
#define UGPS_AID_MODE_LOCAL (1)
#define UGPS_AID_MODE_ASSISTNOW_AUTONOMOUS (8)

/* GNSS_systems */
#define UGPS_GNSS_SYSTEM_GPS (1)
#define UGPS_GNSS_SYSTEM_SBAS (2)
#define UGPS_GNSS_SYSTEM_GALILEO (4)
#define UGPS_GNSS_SYSTEM_GLONASS (64)

typedef struct
{
    unsigned int mode;
    unsigned int aidMode;
    unsigned int gnssSystems;
} SaraGnssStatus;

/* --- Radio Access Technology (RAT) selection: +URAT --- */
/* AcT */
#define URAT_ACT_GSM (0)
#define URAT_ACT_LTE (3)
#define URAT_ACT_LTE_CAT_M1 (7)
#define URAT_ACT_NB_IOT (8)

/* --- GNSS profile configuration: +UGPRF --- */
/* GNSS_IO_configuration */
#define UGPRF_GNSS_IO_NONE (0)
#define UGPRF_GNSS_IO_USB_OR_UART (1)
#define UGPRF_GNSS_IO_MUX (2)

/* --- Serial interfaces configuration selection: +USIO --- */
/* variant */
#define USIO_VARIANT_0 (0)
#define USIO_VARIANT_2 (2)
#define USIO_VARIANT_4 (4)

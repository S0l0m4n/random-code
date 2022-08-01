/**
 * @file AtCommandResultType.h
 *
 * https://www.wagz.com
 */

#pragma once

typedef enum
{
    CR_done = 0x00,       // Modem response has been fully processed
    CR_error,             // Modem responded with an error
    CR_continue,          // Modem responded with expected data, but operation needs more data to consider it finished
    CR_unexpected,        // Modem responded with an unexpected data
    CR_timeout,           // Modem didn't respond with anything useful
    CR_otaDataSizeMismatch,  // Modem failed to OTA
    CR_EnumMax
} AtCommandResult;

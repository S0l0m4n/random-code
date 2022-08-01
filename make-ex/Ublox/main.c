/**
 * @brief Test out AtCmdRespFieldParser.c
 */

#include <stdint.h>
#include <stdio.h>
#include "AtCmdRespFieldParser.h"
#include "AtCommandResultType.h"
#include "CellResponseCodeType.h"
#include "Debug.h"
#include "SaraCommandsDefinitions.h"

#define NO_HEADER (NULL)

#define SIZEOF(x) sizeof(x)/sizeof(x[0])

AtCommandResult SaraCommands_cbGetFwVersion(
        CellResponseCode eResponse, const char* pResponse, void* pResult)
{
    AtCommandResult eResult = CR_continue;

    switch (eResponse)
    {
        case GRC_unknown:
        {
            // Try to parse the field(s)
            char* pFwVersion = (char*) pResult;
            struct AtTextField fields[] =
            {
                { .val = pFwVersion, .type = STRING_FIELD },
            };

            int n = SIZEOF(fields);

            int num_parsed = AT_parseTextFields(pResponse, NO_HEADER, fields, n);

            if (n == num_parsed)
            {
                LOG_INFO("| <fw_ver> = %s\n", pFwVersion);
            }
            break;
        }

        case GRC_ok:
            // Response should have been fully received
            if ('\0' != ((char*) pResult)[0])
            {
                eResult = CR_done;
            }
            else
            {
                eResult = CR_error;
            }
            break;

        case GRC_error:
        case GRC_cmeError:
            // An error occurred
            eResult = CR_error;
            break;

        default:
            // Ignore other messages and continue
            eResult = CR_continue;
            break;
    }

    return eResult;
}

static AtCommandResult cbGetGnssStatus(
        CellResponseCode eResponse, const char* pResponse, void* pResult)
{
    AtCommandResult eResult = CR_continue;

    switch (eResponse)
    {
        case GRC_unknown:
        {
            // Try to parse the field(s)
            SaraGnssStatus* pStatus = (SaraGnssStatus*) pResult;
            struct AtTextField fields[] =
            {
                { .val = &pStatus->mode, .type = NUMBER_FIELD },
                { .val = &pStatus->aidMode, .type = NUMBER_FIELD },
                { .val = &pStatus->gnssSystems, .type = NUMBER_FIELD },
            };

            const char* header = "+UGPS:";
            int n = SIZEOF(fields);

            int num_parsed = AT_parseTextFields(pResponse, header, fields, n);

            if (n == num_parsed)
            {
                LOG_INFO("| <mode> = %u\n", pStatus->mode);
                LOG_INFO("| <aid_mode> = %u\n", pStatus->aidMode);
                LOG_INFO("| <GNSS_systems> = %u\n", pStatus->gnssSystems);
            }
            break;
        }

        case GRC_ok:
            eResult = CR_done;
            break;

        case GRC_error:
        case GRC_cmeError:
            // An error occurred
            eResult = CR_error;
            break;

        default:
            // Ignore other messages and continue
            eResult = CR_continue;
            break;
    }

    return eResult;
}

int main(void)
{
    const char* text;

    LOG_INFO("Test 1:\n");
    text = "02.06";
    char fwVersion[32];
    SaraCommands_cbGetFwVersion(GRC_unknown, text, fwVersion);

    LOG_INFO("Test 2:\n");
    text = "+UGPS: 1,0,1";
    SaraGnssStatus gnssStatus;
    cbGetGnssStatus(GRC_unknown, text, &gnssStatus);

    return 0;
}

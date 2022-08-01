/**
  * Test calculation of an array size at run time, given two different arrays
  */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STATIC_ASSERT(condition)  extern char __staticAssert_[(condition) ? 1 : -1]

#define NUM_ELEMENTS(x) (sizeof(x)/sizeof(x[0]))

typedef enum
{
    GRC_ok,
    GRC_at,
    GRC_ate0,
    GRC_error,
    GRC_cmeError,
    GRC_simReady,
    GRC_phoneReady,
    GRC_gsmRegStatus,
    GRC_lteRegStatus,
    GRC_gprsRegStatus,
    GRC_tcpStackIdle,
    GRC_tcpStackStart,
    GRC_tcpStackConfig,
    GRC_tcpStackActivating,
    GRC_tcpStackGprsActivated,
    GRC_tcpStackIpAddress,
    GRC_tcpStackTcpConnecting,
    GRC_tcpStackTcpClosed,
    GRC_tcpStackTcpConnected,
    GRC_tcpStackGprsDeactivated,
    GRC_sslCertIncorrect,
    GRC_sslCertCorrect,
    GRC_connectionState,
    GRC_dataReceived,
    GRC_dataReceivedForTcp,
    GRC_sendOk,
    GRC_sendFail,
    GRC_closeOk,
    GRC_deactOk,
    GRC_connect,
    GRC_cnum,
    GRC_multislotClass,
    GRC_ccid,
    GRC_cgsn,
    GRC_fwVer,
    GRC_qnitz,
    GRC_ctzu,
    GRC_length,
    GRC_unknown,  // Indication that response from modem is not known to our system
    GRC_none      // Indication that response was not received at all
} CellResponseCode;

typedef struct
{
    const CellResponseCode eCode;
    const char* pString;
} CellResponseTable;

const CellResponseTable responseTable[] =
{
    {GRC_ok, "OK"},
    {GRC_at, "AT\r"},
    {GRC_ate0, "ATE0"},
    {GRC_error, "ERROR"},
    {GRC_cmeError, "+CME ERROR:"},
    {GRC_simReady, "+CPIN: READY"},
    {GRC_phoneReady, "+CFUN:1"},
    {GRC_gsmRegStatus, "+CREG:"},
    {GRC_lteRegStatus, "+CEREG:"},
    {GRC_gprsRegStatus, "+CGREG:"},
    {GRC_tcpStackIdle, "STATE: IP INITIAL"},
    {GRC_tcpStackStart, "STATE: IP START"},
    {GRC_tcpStackConfig, "STATE: IP CONFIG"},
    {GRC_tcpStackActivating, "STATE: IP IND"},
    {GRC_tcpStackGprsActivated, "STATE: IP GPRSACT"},
    {GRC_tcpStackIpAddress, "STATE: IP STATUS"},
    {GRC_tcpStackTcpConnecting, "STATE: TCP CONNECTING"},
    {GRC_tcpStackTcpClosed, "STATE: IP CLOSE"},
    {GRC_tcpStackTcpConnected, "STATE: CONNECT OK"},
    {GRC_tcpStackGprsDeactivated, "STATE: PDP DEACT"},
    {GRC_sslCertIncorrect, "+QSECREAD: 0,"},
    {GRC_sslCertCorrect, "+QSECREAD: 1,"},
    {GRC_connectionState, "+QSSLSTATE: "},
    {GRC_dataReceived, "+QSSLRECV: "},
    {GRC_dataReceivedForTcp, "+QIRD: "},
    {GRC_sendOk, "SEND OK"},
    {GRC_sendFail, "SEND FAIL"},
    {GRC_closeOk, "CLOSE OK"},
    {GRC_deactOk, "DEACT OK"},
    {GRC_connect, "CONNECT"},
    {GRC_cnum, "+CNUM:"},
    {GRC_multislotClass, "MULTISLOT CLASS:"},
    {GRC_ccid, "+QCCID:"},
    {GRC_cgsn, "AT+CGSN"},
    {GRC_fwVer, "AT+QGMR"},
    {GRC_qnitz, "+QNITZ:"},
    {GRC_ctzu, "+CTZU:"},
};
STATIC_ASSERT(NUM_ELEMENTS(responseTable) == GRC_length);

const CellResponseTable ubloxResponseTable[] =
{
    {GRC_ok, "OK"},
    {GRC_error, "ERROR"},
    {GRC_cmeError, "+CME ERROR:"},
};

int main(void)
{
    const CellResponseTable* pResponseTable = responseTable;
    unsigned int arraySize = NUM_ELEMENTS(responseTable);

    if (1)
    {
        pResponseTable = ubloxResponseTable;
        arraySize = NUM_ELEMENTS(ubloxResponseTable);
    }

    for (int i = 0; i < arraySize; i++)
    {
        printf("%02u. Response code is %s\n", i, pResponseTable[i].pString);
    }
}

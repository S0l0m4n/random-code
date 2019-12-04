/**
  * struct init test
  */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define MAX_DEVICE_ENTRIES      (32)

#define NUM_STATS               (10)


typedef struct
{
  uint32_t pair_rx_req; /* # requests received */
  uint32_t pair_tx_chal; /* # challenges sent */
  uint32_t pair_rx_rsp_good; /* # good answers received */
  uint32_t pair_rx_rsp_bad; /* # bad answers received */
  uint32_t pair_tx_unlk; /* # unlocks sent (device accepted) */
  uint32_t pair_rx_dup; /* # duplicate pair reqs recv (device already paired) */
  uint32_t srep_tx;
  uint32_t srep_rx;
  uint32_t srep_rx_unack;
  uint32_t srep_rx_ack;
  uint32_t srep_rx_drop;
} stats_st;


uint32_t serial_numbers[MAX_DEVICE_ENTRIES];
stats_st stats[MAX_DEVICE_ENTRIES];
uint32_t stats_array[MAX_DEVICE_ENTRIES][NUM_STATS];


int main(void)
{
  uint8_t i = 0;
  uint8_t j = 0;

  for (i = 0; i < MAX_DEVICE_ENTRIES; i++)
  {
    memset(&serial_numbers[i], 0xFFFFFF00, 4);
  }

  for (i = 0; i < MAX_DEVICE_ENTRIES; i +=8)
  {
    printf("sn = %08x\n", serial_numbers[i]);
    printf("pair_rx_req = %d pair_tx_unlk = %d srep_tx = %d srep_rx_drop = %d\n",
        stats[i].pair_rx_req, stats[i].pair_tx_unlk, stats[i].srep_tx,
        stats[i].srep_rx_drop);
  }

  memset(stats_array, 0x22, sizeof(stats_array));

  for (i = 0; i < MAX_DEVICE_ENTRIES; i++)
  {
    printf("stats_row[%02d] = ", i);

    for (j = 0; j < NUM_STATS; j++)
    {
      printf("%x ", stats_array[i][j]);
    }

    printf("\n");
  }

  return 0;
}

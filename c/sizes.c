/**
  * check sizes of different types
  */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_DEVICES 8


typedef enum
{
  /* 00 */ ROOM_UNKNOWN,
  /* 01 */ ROOM_HALLWAY,
  /* 02 */ ROOM_KITCHEN,
  /* 03 */ ROOM_LIVING,
  /* 04 */ ROOM_BED1,
  /* 05 */ ROOM_BED2,
  /* 06 */ ROOM_BED3,
  /* 07 */ ROOM_BATH1,
  /* 08 */ ROOM_BATH2,
  /* 09 */ ROOM_09,
  /* 10 */ ROOM_10,
  NUM_ROOMS
} room_e;

typedef struct
{
  uint32_t sn; 
  room_e room;
  uint8_t type;
  uint8_t manf;
  uint8_t flags;
} device_st;


device_st paired_devices[NUM_DEVICES];


int main(void)
{
	uint16_t len_device_st = sizeof(device_st);
	uint16_t len_device_ar = sizeof(paired_devices);

	printf("len of one device = %d | len of array = %d\n",
      len_device_st, len_device_ar);

  return 0;
}

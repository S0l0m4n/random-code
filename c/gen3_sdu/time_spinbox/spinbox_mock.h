/**
  * !!! MOCKED VERSION !!!
  */


#ifndef SPINBOX_MOCK_H
#define SPINBOX_MOCK_H


#include <stdbool.h>
#include <stdint.h>


typedef struct
{
  uint8_t hr;
  uint8_t min;
} time_st;


typedef struct
{
  uint8_t lo;
  uint8_t hi;
  uint8_t step;
  uint8_t value;
  bool focused;
} GUI_spinbox_t;


extern void GUI_ELEM_spinbox_setValue( GUI_spinbox_t *spinbox, uint8_t value );
extern uint8_t GUI_ELEM_spinbox_inc( GUI_spinbox_t *spinbox );
extern uint8_t GUI_ELEM_spinbox_dec( GUI_spinbox_t *spinbox );


#endif

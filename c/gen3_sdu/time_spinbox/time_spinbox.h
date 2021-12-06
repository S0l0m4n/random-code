/**
  * !!! COPIED FOR TESTING !!!
  */


#ifndef TIME_SPINBOX_H
#define TIME_SPINBOX_H


#include <stdbool.h>
#include <stdint.h>
#include "spinbox_mock.h"


typedef struct
{
  GUI_spinbox_t hour;
  GUI_spinbox_t minute;
  struct
  {
    uint8_t hour;
    uint8_t minute;
    bool is_am;
  } value;
} GUI_time_spinbox_st;


extern void GUI_ELEM_timeSpinbox_incHour( GUI_time_spinbox_st *spinbox );
extern void GUI_ELEM_timeSpinbox_incMinute( GUI_time_spinbox_st *spinbox );
extern void GUI_ELEM_timeSpinbox_decHour( GUI_time_spinbox_st *spinbox );
extern void GUI_ELEM_timeSpinbox_decMinute( GUI_time_spinbox_st *spinbox );
extern void GUI_ELEM_timeSpinbox_refreshValue( GUI_time_spinbox_st *spinbox );


#endif

/**
  *
  */


#include "gen3_sdu_defs.h"


void GUI_SCH_getDayTimeFromSpinbox(
    uint8_t *day_hour, uint8_t *day_min, GUI_time_spinbox_st *spinbox )
{
  uint8_t hour = spinbox->value.hour;

  if (spinbox->value.is_am && (12 == hour))
  {
    /* time is between 12:00 am and 12:59 am */
    hour = 0;
  }
  else if (false == spinbox->value.is_am && (12 != hour))
  {
    /* time is 1 pm or beyond (not between 12:00 pm and 12:59 pm) */
    hour += 12;
  }

  *day_hour = hour;
  *day_min = spinbox->value.minute;
}

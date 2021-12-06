/**
  * !!! COPIED FOR TESTING !!!
  */


#include "spinbox_mock.h"
#include "time_spinbox.h"


/* --- mocked static functions --- */
void setAmPmLabel( GUI_time_spinbox_st *spinbox, bool is_am )
{
  spinbox->value.is_am = is_am;
};
/* --- */


void GUI_ELEM_timeSpinbox_incHour( GUI_time_spinbox_st *spinbox )
{
  uint8_t hour = GUI_ELEM_spinbox_inc(&(spinbox->hour));
  spinbox->value.hour = hour;
  if (12 == hour)
  {
    /* we just ticked over from 11 to 12, so toggle am/pm label accordingly */
    spinbox->value.is_am = !spinbox->value.is_am;
    setAmPmLabel(spinbox, spinbox->value.is_am);
  }
}


void GUI_ELEM_timeSpinbox_incMinute( GUI_time_spinbox_st *spinbox )
{
  uint8_t minute = spinbox->value.minute;

  if (minute >= 45)
  {
    /* increment to the next hour */
    GUI_ELEM_timeSpinbox_incHour(spinbox);
    /* and clear the minute value */
    minute = 0;
  }
  else if (minute >= 30)
  {
    minute = 45;
  }
  else if (minute >= 15)
  {
    minute = 30;
  }
  else if (minute >= 0)
  {
    minute = 15;
  }

  /* set the new minute value */
  GUI_ELEM_spinbox_setValue(&(spinbox->minute), minute);
  spinbox->value.minute = minute;
}


void GUI_ELEM_timeSpinbox_decHour( GUI_time_spinbox_st *spinbox )
{
  uint8_t hour = GUI_ELEM_spinbox_dec(&(spinbox->hour));
  spinbox->value.hour = hour;
  if (11 == hour)
  {
    /* we just ticked over from 12 to 11, so toggle am/pm label accordingly */
    spinbox->value.is_am = !spinbox->value.is_am;
    setAmPmLabel(spinbox, spinbox->value.is_am);
  }
}


void GUI_ELEM_timeSpinbox_decMinute( GUI_time_spinbox_st *spinbox )
{
  uint8_t minute = spinbox->value.minute;

  if (minute > 45)
  {
    minute = 45;
  }
  else if (minute > 30)
  {
    minute = 30;
  }
  else if (minute > 15)
  {
    minute = 15;
  }
  else if (minute > 0)
  {
    minute = 0;
  }
  else if (0 == minute)
  {
    /* decrement to the next hour */
    GUI_ELEM_timeSpinbox_decHour(spinbox);
    /* and drop the minute value to 45 */
    minute = 45;
  }

  /* set the new minute value */
  GUI_ELEM_spinbox_setValue(&(spinbox->minute), minute);
  spinbox->value.minute = minute;
}


void GUI_ELEM_timeSpinbox_refreshValue( GUI_time_spinbox_st *spinbox )
{
  (void)spinbox;
}


void GUI_ELEM_spinbox_setValueFrom24hTime(
    GUI_time_spinbox_st *spinbox, time_st time )
{
  if (time.hr > 12)
  {
    spinbox->value.hour = time.hr % 12;
    spinbox->value.is_am = false;
  }
  else
  {
    spinbox->value.hour = time.hr;
    spinbox->value.is_am = true;
  }
  spinbox->value.minute = time.min;
}

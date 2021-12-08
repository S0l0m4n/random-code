/**
  * !!! MOCKED VERSION !!!
  */


#include "spinbox_mock.h"


void GUI_ELEM_spinbox_setValue( GUI_spinbox_t *spinbox, uint8_t value )
{
  spinbox->value = value;
}


uint8_t GUI_ELEM_spinbox_inc( GUI_spinbox_t *spinbox )
{
  uint8_t value = spinbox->value;
  value += spinbox->step;
  if (value > spinbox->hi)
  {
    value = spinbox->lo;
  }
  spinbox->value = value;
  return value;
}


uint8_t GUI_ELEM_spinbox_dec( GUI_spinbox_t *spinbox )
{
  uint8_t value = spinbox->value;
  if (spinbox->lo == value)
  {
    /* wrap around  to highest value */
    value = spinbox->hi;
  }
  else if (value < spinbox->lo + spinbox->step)
  {
    /* clear to lowest value */
      value = spinbox->lo;
  }
  else
  {
    /* decrement by step value */
    value -= spinbox->step;
  }
  spinbox->value = value;
  return value;
}

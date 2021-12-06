/**
  * TEST FUNCTION
  *
  * Ensure that the end time does not become invalid relative to the start time
  * during editing.
  */


#include <stdio.h>
#include <stdlib.h>
#include "gen3_sdu_defs.h"
#include "time_spinbox.h"


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


typedef enum
{
  INC_HR,
  INC_MIN,
  DEC_HR,
  DEC_MIN,
} test_op_e;


typedef struct
{
  time_st start_time;
  time_st initial_end_time;
  test_op_e op;
  time_st desired_end_time;
} test_case_st;


const char *op_names[] =
{
  "INC_HR",
  "INC_MIN",
  "DEC_HR",
  "DEC_MIN",
};


test_case_st test_cases[] =
{
  /* [  0 ] */  { { .hr = 12, .min = 47 }, { .hr = 13, .min =  0 },
                  INC_MIN,                 { .hr = 13, .min = 15 } },
  /* [  1 ] */  { { .hr = 12, .min = 47 }, { .hr = 13, .min =  0 },
                  DEC_MIN,                 { .hr = 13, .min =  0 } },
};


static GUI_time_spinbox_st time_spinbox =
{
  .hour.lo = 1, .hour.hi = 12, .hour.step = 1, .hour.focused = false,
  .minute.focused = false,
};
static SCH_edit_st edit_info_obj;
static SCH_edit_st *edit_info;


static void updateEditInfoPeriod( void );
static void incEndTime( void );
static void decEndTime( void );


int main( void )
{
  uint8_t i = 0;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  test_case_st tc;
  day_time_st actual_end_time;

  /* set up global pointer */
  edit_info = &edit_info_obj;

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* --- set up test --- */
    tc = test_cases[i];
    GUI_ELEM_spinbox_setValueFrom24hTime(&time_spinbox, tc.initial_end_time);
    /* [prepare for inc/dec function] */
    switch (tc.op)
    {
      case INC_HR:
      case DEC_HR:
        time_spinbox.hour.focused = true;
        time_spinbox.minute.focused = false;
        break;

      case INC_MIN:
      case DEC_MIN:
        time_spinbox.hour.focused = false;
        time_spinbox.minute.focused = true;
        break;

      default:
        break;
    }
    /* [initialise edit info pointer] */
    edit_info->period.start.hour = tc.start_time.hr;
    edit_info->period.start.min = tc.start_time.min;
    edit_info->period.end.hour = tc.initial_end_time.hr;
    edit_info->period.end.min = tc.initial_end_time.min;
    /* --- */

    /* run test */
    switch (tc.op)
    {
      case INC_HR:
      case INC_MIN:
        incEndTime();
        break;

      case DEC_HR:
      case DEC_MIN:
        decEndTime();
        break;

      default:
        break;
    }

    /* check result */
    GUI_SCH_setDayTimeFromSpinbox(&actual_end_time, &time_spinbox);
    if ((tc.desired_end_time.hr != actual_end_time.hour)
    ||  (tc.desired_end_time.min != actual_end_time.min))
    {
      pass = false;
    }

    /* print result */
    printf("test %2d: start: %02d:%02d | end: %02d:%02d -> (%s) -> "
           "%02d:%02d -> %s",
        i,
        tc.start_time.hr, tc.start_time.min,
        tc.initial_end_time.hr, tc.initial_end_time.min,
        op_names[tc.op],
        tc.desired_end_time.hr, tc.desired_end_time.min,
        pass ? "PASS" : "FAIL");

    if (pass)
    {
      printf("\n");
    }
    else /* fail */
    {
      /* print actual result */
      printf(" (%02d:%02d)\n",
        actual_end_time.hour, actual_end_time.min);
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}


static void updateEditInfoPeriod( void )
{
}


static void incEndTime( void )
{
  uint8_t end_hour = time_spinbox.value.hour;
  uint8_t start_hour = edit_info->period.start.hour;
  uint8_t end_minute = time_spinbox.value.minute;

  if (time_spinbox.hour.focused)
  {
    /* do not roll over to the next day at 23:xx */
    if (end_hour < 23)
    {
      GUI_ELEM_timeSpinbox_incHour(&time_spinbox);
    }
  }
  else if (time_spinbox.minute.focused)
  {
    if (false ==
        (((23 == end_hour) || (start_hour == end_hour)) && (45 <= end_minute)))
    {
      /* do not roll over to the next day at 23:45, or reset the minute to zero
       * if the start hour and end hour are the same */
      GUI_ELEM_timeSpinbox_incMinute(&time_spinbox);
    }
  }
  updateEditInfoPeriod();
}


static void decEndTime( void )
{
  uint8_t end_hour = time_spinbox.value.hour;
  uint8_t start_hour = edit_info->period.start.hour;
  uint8_t end_minute = time_spinbox.value.minute;
  uint8_t start_minute = edit_info->period.start.min;

  if (time_spinbox.hour.focused)
  {
    /* do not become less than the start hour */
    if (end_hour > start_hour)
    {
      GUI_ELEM_timeSpinbox_decHour(&time_spinbox);
    }

    /* adjust the end minute if it ends up being the same as the start minute
     * when the start hour and the end hour are the same */
    if ((start_hour == time_spinbox.value.hour) && (end_minute < start_minute))
    {
      time_spinbox.value.minute = start_minute;
      GUI_ELEM_timeSpinbox_refreshValue(&time_spinbox);
    }
  }
  else if (time_spinbox.minute.focused)
  {
    if (false ==
        (((23 == end_hour) || (start_hour == end_hour)) && (45 <= end_minute)))
    {
      /* do not roll over to the next day at 23:45, or reset the minute to zero
       * if the start hour and end hour are the same */
      GUI_ELEM_timeSpinbox_decMinute(&time_spinbox);
    }
  }
  updateEditInfoPeriod();
}

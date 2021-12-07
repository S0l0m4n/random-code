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
  "INC_HR ",
  "INC_MIN",
  "DEC_HR ",
  "DEC_MIN",
};


test_case_st test_cases[] =
{
  /* [  0 ] */  { { .hr = 12, .min = 47 }, { .hr = 13, .min =  0 },
                  DEC_MIN,                 { .hr = 13, .min =  0 } },
  /* [  1 ] */  { { .hr = 12, .min = 44 }, { .hr = 13, .min =  0 },
                  DEC_MIN,                 { .hr = 12, .min = 45 } },
  /* [  2 ] */  { { .hr = 17, .min = 30 }, { .hr = 18, .min =  0 },
                  DEC_MIN,                 { .hr = 17, .min = 45 } },
  /* [  3 ] */  { { .hr = 17, .min = 30 }, { .hr = 17, .min = 45 },
                  DEC_MIN,                 { .hr = 17, .min = 45 } },
  /* [  4 ] */  { { .hr = 23, .min = 30 }, { .hr = 23, .min = 45 },
                  DEC_MIN,                 { .hr = 23, .min = 45 } },
  /* [  5 ] */  { { .hr = 23, .min = 15 }, { .hr = 23, .min = 45 },
                  DEC_MIN,                 { .hr = 23, .min = 30 } },
  /* [  6 ] */  { { .hr = 21, .min = 45 }, { .hr = 22, .min =  5 },
                  DEC_MIN,                 { .hr = 22, .min =  0 } },
  /* [  7 ] */  { { .hr = 21, .min = 45 }, { .hr = 22, .min =  0 },
                  DEC_MIN,                 { .hr = 22, .min =  0 } },
  /* [  8 ] */  { { .hr = 21, .min = 30 }, { .hr = 22, .min =  0 },
                  DEC_MIN,                 { .hr = 21, .min = 45 } },
  /* [  9 ] */  { { .hr = 21, .min = 30 }, { .hr = 22, .min =  0 },
                  DEC_HR,                  { .hr = 21, .min = 30 } },
  /* [ 10 ] */  { { .hr = 21, .min = 30 }, { .hr = 21, .min = 30 },
                  DEC_HR,                  { .hr = 21, .min = 30 } },
  /* [ 11 ] */  { { .hr = 20, .min = 30 }, { .hr = 22, .min =  0 },
                  DEC_HR,                  { .hr = 21, .min =  0 } },
  /* [ 12 ] */  { { .hr = 12, .min = 47 }, { .hr = 13, .min =  0 },
                  INC_MIN,                 { .hr = 13, .min = 15 } },
  /* [ 13 ] */  { { .hr = 20, .min = 15 }, { .hr = 23, .min = 30 },
                  INC_MIN,                 { .hr = 23, .min = 45 } },
  /* [ 14 ] */  { { .hr = 20, .min = 15 }, { .hr = 23, .min = 45 },
                  INC_MIN,                 { .hr = 23, .min = 45 } },
  /* [ 15 ] */  { { .hr = 20, .min = 15 }, { .hr = 20, .min = 45 },
                  INC_HR,                  { .hr = 21, .min = 45 } },
  /* [ 16 ] */  { { .hr = 20, .min = 15 }, { .hr = 23, .min = 15 },
                  INC_HR,                  { .hr = 23, .min = 15 } },
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
  uint8_t actual_end_hour;
  uint8_t actual_end_min;

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
    GUI_SCH_getDayTimeFromSpinbox(&actual_end_hour, &actual_end_min, &time_spinbox);
    if ((tc.desired_end_time.hr != actual_end_hour)
    ||  (tc.desired_end_time.min != actual_end_min))
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
        actual_end_hour, actual_end_min);
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
  uint8_t end_hour = 0;
  uint8_t end_minute = 0;

  GUI_SCH_getDayTimeFromSpinbox(&end_hour, &end_minute, &time_spinbox);

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
    if (false == ((23 == end_hour) && (45 <= end_minute)))
    {
      /* do not roll over to the next day at 23:45 */
      GUI_ELEM_timeSpinbox_incMinute(&time_spinbox);
    }
  }
  updateEditInfoPeriod();
}


static void decEndTime( void )
{
  uint8_t end_hour = 0;
  uint8_t end_minute = 0;
  uint8_t start_hour = edit_info->period.start.hour;
  uint8_t start_minute = edit_info->period.start.min;

  GUI_SCH_getDayTimeFromSpinbox(&end_hour, &end_minute, &time_spinbox);

  if (time_spinbox.hour.focused)
  {
    /* do not become less than the start hour */
    if (end_hour > start_hour)
    {
      GUI_ELEM_timeSpinbox_decHour(&time_spinbox);
    }
    /* adjust the end minute if it ends up being the same as the start minute
     * when the start hour and the end hour are the same */
    GUI_SCH_getDayTimeFromSpinbox(&end_hour, &end_minute, &time_spinbox);
    if ((start_hour == end_hour) && (end_minute < start_minute))
    {
      time_spinbox.value.minute = start_minute;
      GUI_ELEM_timeSpinbox_refreshValue(&time_spinbox);
    }
  }
  else if (time_spinbox.minute.focused)
  {
    /* do not decrease the end minute if it is not more than 15 min above the
     * start minute, when the start hour and end hour are the same */
    if ((false ==
        ((start_hour == end_hour) && (end_minute <= start_minute + 15)))
    /* do not decrease the end minute from zero if the start time is not more
     * than 15 min less than the end time */
    &&  (false ==
        ((start_hour + 1 == end_hour) && (0==end_minute) && (start_minute>=45)))
    )
    {
      GUI_ELEM_timeSpinbox_decMinute(&time_spinbox);
    }
  }
  updateEditInfoPeriod();
}

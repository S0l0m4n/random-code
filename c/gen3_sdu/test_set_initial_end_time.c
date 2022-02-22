/**
  * TEST FUNCTION
  *
  * Set initial end time. If the end time is NOT a valid time greater than the
  * start time, then set it to be 1 hour after the start time (if possible).
  */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gen3_sdu_defs.h"


#define MINS_IN_DAY (24 * 60)


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


typedef enum
{
  EDIT = SCH_PERIOD_EDIT,
  ADD = SCH_PERIOD_ADD,
  REM = SCH_PERIOD_REMOVE,
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
  "EDIT",
  "ADD",
  "REM",
};


test_case_st test_cases[] =
{
  /* [  0 ] */  { { .hr = 12, .min = 47 }, { .hr = 13, .min = 45 },
                  EDIT,                    { .hr = 13, .min = 45 } },
  /* [  1 ] */  { { .hr = 19, .min = 36 }, { .hr = 13, .min = 45 },
                  EDIT,                    { .hr = 20, .min = 36 } },
  /* [  2 ] */  { { .hr = 19, .min = 30 }, { .hr = 19, .min = 45 },
                  EDIT,                    { .hr = 19, .min = 45 } },
  /* [  3 ] */  { { .hr = 23, .min = 45 }, { .hr = 13, .min = 45 },
                  EDIT,                    { .hr = 23, .min = 45 } },
  /* [  4 ] */  { { .hr = 14, .min =  0 }, { .hr = 15, .min = 45 },
                  EDIT,                    { .hr = 15, .min = 45 } },
  /* [  5 ] */  { { .hr = 14, .min =  0 }, { .hr = 12, .min = 45 },
                  EDIT,                    { .hr = 15, .min =  0 } },
  /* [  6 ] */  { { .hr =  2, .min =  0 }, { .hr =  0, .min =  0 },
                  EDIT,                    { .hr =  3, .min =  0 } },
  /* [  7 ] */  { { .hr =  2, .min =  0 }, { .hr = 99, .min =  0 },
                  EDIT,                    { .hr =  3, .min =  0 } },
  /* [  8 ] */  { { .hr = 13, .min =  0 }, { .hr = 13, .min =  0 },
                  EDIT,                    { .hr = 14, .min =  0 } },
};


static SCH_edit_st edit_info_obj;
static SCH_edit_st *edit_info;


static void setEditPeriodEndTime( void );


int main( void )
{
  uint8_t i = 0;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  test_case_st tc;

  /* set up global pointer */
  edit_info = &edit_info_obj;

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* --- set up test --- */
    tc = test_cases[i];
    edit_info->period.start.hour = tc.start_time.hr;
    edit_info->period.start.min = tc.start_time.min;
    edit_info->period.end.hour = tc.initial_end_time.hr;
    edit_info->period.end.min = tc.initial_end_time.min;
    edit_info->action = tc.op;

    /* run test */
    setEditPeriodEndTime();

    /* check result */
    if ((tc.desired_end_time.hr != edit_info->period.end.hour)
    ||  (tc.desired_end_time.min != edit_info->period.end.min))
    {
      pass = false;
    }

    /* print result */
    printf("test %2d: start %02d:%02d | end: %02d:%02d -> (%s) -> "
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
        edit_info->period.end.hour, edit_info->period.end.min);
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}


static void setEditPeriodEndTime( void )
{
  SCH_edit_st *e = edit_info;
  uint16_t start_time_day_min = e->period.start.hour * 60 + e->period.start.min;
  uint16_t end_time_day_min = e->period.end.hour * 60 + e->period.end.min;

  if ((end_time_day_min >= start_time_day_min + 15)
  &&  (end_time_day_min < MINS_IN_DAY))
  {
    /* end time is valid, so we won't change it */
    return;
  }

  if (e->period.start.hour < 23)
  {
    e->period.end.hour = e->period.start.hour + 1;
  }
  else
  {
    /* leave the end hour the same as the start hour */
    e->period.end.hour = e->period.start.hour;
  }

  /* ensure the end minute is the same as the start minute */
  e->period.end.min = e->period.start.min;
}

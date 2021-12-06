/**
  * TEST FUNCTION
  */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


/* --- from gen3_sdu --- */
typedef struct
{
  uint8_t hour;
  uint8_t min;
} day_time_st;


typedef struct
{
  uint8_t day; /* period day (for same-day periods, equals first matching day */
  day_time_st start; /* start time */
  day_time_st end; /* end time */
  uint8_t value; /* used for TT (always 1 for HW) */
} period_st;


typedef enum
{
  SCH_PERIOD_EDIT,
  SCH_PERIOD_ADD,
  SCH_PERIOD_REMOVE,
  NUM_OF_ACTIONS
} action_e;


typedef struct
{
//schedule_st schedule; /* the schedule being edited */
//SCH_show_st block; /* the current block being edited */
//uint8_t period_index; /* the index of the period being edited */
  period_st period; /* the edited period, or the period to add */
  action_e action; /* the action to perform on the period (edit, add, remove) */
//uint8_t selected_days; /* bit x if set indicates that day x is selected */
//bool changes_pending; /* true if schedule has differed from the original */
//uint32_t good_marker; /* validates this info in flash */
} SCH_edit_st;
/* --- */


typedef struct
{
  uint8_t hr;
  uint8_t min;
} time_st;


typedef enum
{
  EDIT = SCH_PERIOD_EDIT,
  ADD = SCH_PERIOD_ADD,
  REM = SCH_PERIOD_REMOVE,
} test_op_e;


typedef struct
{
  time_st start_time;
  test_op_e op;
  time_st end_time;
} test_case_st;


const char *op_names[] =
{
  "EDIT",
  "ADD",
  "REM",
};


test_case_st test_cases[] =
{
  /* [  0 ] */  { { .hr = 12, .min = 47 }, EDIT,  { .hr = 13, .min = 47 } },
  /* [  1 ] */  { { .hr = 19, .min = 36 }, EDIT,  { .hr = 20, .min = 36 } },
  /* [  2 ] */  { { .hr =  8, .min = 15 }, EDIT,  { .hr =  9, .min = 15 } },
  /* [  3 ] */  { { .hr = 23, .min = 16 }, EDIT,  { .hr = 23, .min = 16 } },
  /* [  4 ] */  { { .hr = 23, .min = 45 }, EDIT,  { .hr = 23, .min = 45 } },
  /* [  5 ] */  { { .hr =  0, .min = 59 }, EDIT,  { .hr =  1, .min = 59 } },
};


static void setEditPeriodEndTime( SCH_edit_st *e );


int main( void )
{
  uint8_t i = 0;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  SCH_edit_st edit_info;
  test_case_st tc;

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* set up test */
    tc = test_cases[i];
    edit_info.period.start.hour = tc.start_time.hr;
    edit_info.period.start.min = tc.start_time.min;
    edit_info.action = tc.op;

    /* run test */
    setEditPeriodEndTime(&edit_info);

    /* check result */
    if ((tc.end_time.hr != edit_info.period.end.hour)
    ||  (tc.end_time.min != edit_info.period.end.min))
    {
      pass = false;
    }

    /* print result */
    printf("test %2d: %02d:%02d -> (%s) -> %02d:%02d -> %s",
        i, 
        tc.start_time.hr, tc.start_time.min,
        op_names[tc.op],
        tc.end_time.hr, tc.end_time.min,
        pass ? "PASS" : "FAIL");

    if (pass)
    {
      printf("\n");
    }
    else /* fail */
    {
      /* print actual result */
      printf(" (%02d:%02d)\n",
        edit_info.period.end.hour, edit_info.period.end.min);
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}


static void setEditPeriodEndTime( SCH_edit_st *e )
{
  if (e->period.start.hour < 23)
  {
    e->period.end.hour = e->period.start.hour + 1;
  }
  else
  {
    /* leave the end time the same as the start time */
    e->period.end.hour = e->period.start.hour;
  }
  /* minute remains the same always */
  e->period.end.min = e->period.start.min;
}

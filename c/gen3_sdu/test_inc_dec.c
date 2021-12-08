/**
  * TEST FUNCTION
  */


#include <stdio.h>
#include <stdlib.h>
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
  time_st old_time;
  test_op_e op;
  time_st new_time;
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
  /* [  0 ] */  { { .hr = 12, .min = 47 }, INC_MIN, { .hr =  1, .min =  0 } },
  /* [  1 ] */  { { .hr = 12, .min = 36 }, INC_MIN, { .hr = 12, .min = 45 } },
  /* [  2 ] */  { { .hr = 12, .min = 30 }, INC_MIN, { .hr = 12, .min = 45 } },
  /* [  3 ] */  { { .hr = 12, .min = 19 }, INC_MIN, { .hr = 12, .min = 30 } },
  /* [  4 ] */  { { .hr = 12, .min =  8 }, INC_MIN, { .hr = 12, .min = 15 } },
  /* [  5 ] */  { { .hr = 12, .min =  0 }, INC_MIN, { .hr = 12, .min = 15 } },
  /* [  6 ] */  { { .hr = 11, .min = 59 }, INC_MIN, { .hr = 12, .min =  0 } },
  /* [  7 ] */  { { .hr = 11, .min = 59 }, DEC_MIN, { .hr = 11, .min = 45 } },
  /* [  8 ] */  { { .hr = 11, .min = 46 }, DEC_MIN, { .hr = 11, .min = 45 } },
  /* [  9 ] */  { { .hr = 11, .min = 45 }, DEC_MIN, { .hr = 11, .min = 30 } },
  /* [ 10 ] */  { { .hr = 11, .min = 32 }, DEC_MIN, { .hr = 11, .min = 30 } },
  /* [ 11 ] */  { { .hr = 11, .min = 26 }, DEC_MIN, { .hr = 11, .min = 15 } },
  /* [ 12 ] */  { { .hr = 11, .min = 15 }, DEC_MIN, { .hr = 11, .min =  0 } },
  /* [ 13 ] */  { { .hr = 11, .min =  5 }, DEC_MIN, { .hr = 11, .min =  0 } },
  /* [ 14 ] */  { { .hr = 11, .min =  0 }, DEC_MIN, { .hr = 10, .min = 45 } },
};


int main( void )
{
  uint8_t i = 0;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  GUI_time_spinbox_st time_spinbox =
  {
    .hour.lo = 1, .hour.hi = 12, .hour.step = 1,
  };
  test_case_st tc;

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* set up test */
    tc = test_cases[i];
    time_spinbox.hour.value = tc.old_time.hr;
    time_spinbox.value.hour = tc.old_time.hr;
    time_spinbox.value.minute = tc.old_time.min;

    /* run test */
    switch (tc.op)
    {
      case INC_HR:
        GUI_ELEM_timeSpinbox_incHour(&time_spinbox);
        break;

      case INC_MIN:
        GUI_ELEM_timeSpinbox_incMinute(&time_spinbox);
        break;

      case DEC_HR:
        GUI_ELEM_timeSpinbox_decHour(&time_spinbox);
        break;

      case DEC_MIN:
        GUI_ELEM_timeSpinbox_decMinute(&time_spinbox);
        break;

      default:
        break;
    }

    /* check result */
    if ((tc.new_time.hr != time_spinbox.value.hour)
    ||  (tc.new_time.min != time_spinbox.value.minute))
    {
      pass = false;
    }

    /* print result */
    printf("test %2d: %02d:%02d -> (%s) -> %02d:%02d -> %s",
        i, 
        tc.old_time.hr, tc.old_time.min,
        op_names[tc.op],
        tc.new_time.hr, tc.new_time.min,
        pass ? "PASS" : "FAIL");

    if (pass)
    {
      printf("\n");
    }
    else /* fail */
    {
      /* print actual result */
      printf(" (%02d:%02d)\n",
        time_spinbox.value.hour, time_spinbox.value.minute);
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}

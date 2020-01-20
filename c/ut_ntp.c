/**
  * Unit testing for these functions:
  *   - convertNtpTimeToDateTime
  *   - convertDateTimeToNtpTime
  */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* ### Copy-paste of constants from ntp.c ### */

/* Useful constants for time manipulations */
#define DIFF_SEC_1900_2000            ((uint32_t) 3155673600)

#define SECS_PER_YEAR                 31536000
#define SECS_PER_DAY                  86400
#define SECS_PER_HOUR                 3600
#define SECS_PER_MINUTE               60

#define RTC_SUNDAY                    0x07

/* ### Custom definitions for this unit test ### */

const char *MonthNames[] =
{
  /* 00 */ "---",
  /* 01 */ "Jan",
  /* 02 */ "Feb",
  /* 03 */ "Mar",
  /* 04 */ "Apr",
  /* 05 */ "May",
  /* 06 */ "Jun",
  /* 07 */ "Jul",
  /* 08 */ "Aug",
  /* 09 */ "Sep",
  /* 10 */ "Oct",
  /* 11 */ "Nov",
  /* 12 */ "Dec",
};

typedef struct
{
   uint8_t weekday;
   uint8_t day;
   uint8_t month;
   uint8_t year;
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
} RTC_InfoStruct;

/* ### Implementation of convertNtpTimeToDateTime from ntp.c ### */

/* Convert from the NTP epoch time (seconds only, not the fractional part) into
 * a date and time as per the RTC info struct */
static bool convertNtpTimeToDateTime( uint32_t ntp_time, RTC_InfoStruct *rtc )
{
  bool ret = false;

  uint8_t year, month, day, hours, minutes;

  bool isleap;

  uint32_t secs_per_year, secs_per_month;

  /* The remainder starts from the given NTP epoch time (in seconds) */
  uint32_t remainder = ntp_time;

  /* Put the date/time calculated from the NTP epoch time into this struct */
  RTC_InfoStruct rtc_info;

  /* Confirm time is after 01 Jan 2000 */
  if (remainder >= DIFF_SEC_1900_2000)
  {
    /* Start from 01 Jan 2000 (a leap year) */
    remainder -= DIFF_SEC_1900_2000;
    isleap = true;

    /* Calculate number of years */
    year = 0;
    secs_per_year = SECS_PER_YEAR + SECS_PER_DAY;
    while (remainder >= secs_per_year)
    {
      year++;

      /* Subtract a year's worth of seconds */
      remainder -= secs_per_year;

      if (year % 4 == 0)
      {
        isleap = true;
        secs_per_year = SECS_PER_YEAR + SECS_PER_DAY;
      }
      else
      {
        isleap = false;
        secs_per_year = SECS_PER_YEAR;
      }
    }

    /* Calculate number of months */
    month = 1;
    secs_per_month = 31 * SECS_PER_DAY;
    while (remainder >= secs_per_month)
    {
      month++;

      /* Subtract a month's worth of seconds */
      remainder -= secs_per_month;

      /* Determine the number of seconds in the next month */
      switch (month)
      {
        /* Jan, Mar, May, Jul, Aug, Oct, Dec */
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          secs_per_month = 31 * SECS_PER_DAY;
          break;

        /* Feb */
        case 2:
          secs_per_month = (isleap ? 29 : 28) * SECS_PER_DAY;
          break;

        /* Apr, Jun, Sep, Nov */
        case 4:
        case 6:
        case 9:
        case 11:
          secs_per_month = 30 * SECS_PER_DAY;
          break;

        default:
          break;
      }
    }

    /* Calculate number of days */
    day = 1;
    while (remainder >= SECS_PER_DAY)
    {
      day++;
      remainder -= SECS_PER_DAY;
    }

    /* Calculate number of hours */
    hours = 0;
    while (remainder >= SECS_PER_HOUR)
    {
      hours++;
      remainder -= SECS_PER_HOUR;
    }

    /* Calculate number of minutes */
    minutes = 0;
    while (remainder >= SECS_PER_MINUTE)
    {
      minutes++;
      remainder -= SECS_PER_MINUTE;
    }

    /* NOTE: Remainder is the number of seconds */

    rtc_info.year = year;
    rtc_info.month = month;
    rtc_info.day = day;
    rtc_info.hours = hours;
    rtc_info.minutes = minutes;
    rtc_info.seconds = remainder;
    rtc_info.weekday = RTC_SUNDAY; /* not used */

    /* Assign the RTC info struct pointer to the calculated info */
    *rtc = rtc_info;

    ret = true;
  }

  return ret;
}

/* ### Implementation of convertDateTimeToNtpTime from ntp.c ### */

/* Convert from the date and time in the RTC info struct to the NTP epoch time
 * in seconds (no fractional part) */
static bool convertDateTimeToNtpTime( RTC_InfoStruct *rtc, uint32_t *ntp_time )
{
  return false;
}

/* ### Test cases ### */

uint32_t TestTimes[] =
{
  3155673600,                         /* [00] 00:00:00 01 Jan 2000 */
  3155673600 + 3600,                  /* [01] 01:00:00 01 Jan 2000 */
  3155673600 + 86400,                 /* [02] 00:00:00 02 Jan 2000 */
  3155673600 + 90000,                 /* [03] 01:00:00 02 Jan 2000 */
  3155673600 + 31536000,              /* [04] 00:00:00 31 Dec 2000 */
  3155673600 + 31622400,              /* [05] 00:00:00 01 Jan 2001 */
  3761917500,                         /* [06] 17:05:00 18 Mar 2019 */
  4294967295,                         /* [07] 06:28:15 07 Feb 2036 [MAXTIME] */
  3155673599,                         /* [08] 23:59:59 31 Dec 1999 [INVALID] */
  0,                                  /* [09] 00:00:00 01 Jan 1900 [INVALID] */
  3155673600 + 60,                    /* [10] 00:01:00 01 Jan 2000 */
  3155673600 + 126230399,             /* [11] 23:59:59 31 Dec 2003 */
  3155673600 + 126230400,             /* [12] 00:00:00 01 Jan 2004 */
  3155673600 + 126230401,             /* [13] 00:00:01 01 Jan 2004 */
  4028012072,                         /* [14] 12:14:32 23 Aug 2027 */
};

RTC_InfoStruct TestRtcInfo[] =
{
/*[00]*/ { .hours=0,  .minutes=0,  .seconds=0,  .day=1,  .month=1,  .year=0  },
/*[01]*/ { .hours=1,  .minutes=0,  .seconds=0,  .day=1,  .month=1,  .year=0  },
/*[02]*/ { .hours=0,  .minutes=0,  .seconds=0,  .day=2,  .month=1,  .year=0  },
/*[03]*/ { .hours=1,  .minutes=0,  .seconds=0,  .day=2,  .month=1,  .year=0  },
/*[04]*/ { .hours=0,  .minutes=0,  .seconds=0,  .day=31, .month=12, .year=0  },
/*[05]*/ { .hours=0,  .minutes=0,  .seconds=0,  .day=1,  .month=1,  .year=1  },
/*[06]*/ { .hours=17, .minutes=5,  .seconds=0,  .day=18, .month=3,  .year=19 },
/*[07]*/ { .hours=6,  .minutes=28, .seconds=15, .day=7,  .month=2,  .year=36 },
/*[08]*/ { .hours=0   /* dummy value: no comparison, as time is invalid */   },
/*[09]*/ { .hours=0   /* ditto */                                            },
/*[10]*/ { .hours=0,  .minutes=1,  .seconds=0,  .day=1,  .month=1,  .year=0  },
/*[11]*/ { .hours=23, .minutes=59, .seconds=59, .day=31, .month=12, .year=3  },
/*[12]*/ { .hours=0,  .minutes=0,  .seconds=0,  .day=1,  .month=1,  .year=4  },
/*[13]*/ { .hours=0,  .minutes=0,  .seconds=1,  .day=1,  .month=1,  .year=4  },
/*[14]*/ { .hours=12, .minutes=14, .seconds=32, .day=23, .month=8,  .year=27 },
};

bool TestResults[] =
{
/*[00]*/ true,
/*[01]*/ true,
/*[02]*/ true,
/*[03]*/ true,
/*[04]*/ true,
/*[05]*/ true,
/*[06]*/ true,
/*[07]*/ true,
/*[08]*/ false,
/*[09]*/ false,
/*[10]*/ true,
/*[11]*/ true,
/*[12]*/ true,
/*[13]*/ true,
/*[14]*/ true,
};

/* ### Helper functions used in unit testing ### */

/* ATTENTION: Does not check weekday field */
bool checkDateTime(RTC_InfoStruct *info1, RTC_InfoStruct *info2)
{
  bool ret = false;

  if (info1->hours == info2->hours
      && info1->minutes == info2->minutes
      && info1->seconds == info2->seconds
      && info1->day == info2->day
      && info1->month == info2->month
      && info1->year == info2->year)
  {
    ret = true;
  }

  return ret;
}

void printDateTime(RTC_InfoStruct *rtcinfo)
{
  printf("%02d:%02d:%02d %02d %s 20%02d ",
      rtcinfo->hours, rtcinfo->minutes, rtcinfo->seconds,
      rtcinfo->day, MonthNames[rtcinfo->month], rtcinfo->year);
};

/* ### Main function for running unit test of convertNtpTimeToDateTime ### */

bool runTest_TimeToRtcInfo(uint8_t i)
{
  bool result, ispass;

  /* Define left and right sides of test conversion, "time" and "rtcinfo" */
  uint32_t time = TestTimes[i];
  RTC_InfoStruct rtcinfo = TestRtcInfo[i];

  /* This holds the calculated RTC info */
  RTC_InfoStruct test_rtcinfo;

  /* RUN TEST: convert function should yield test_rtcinfo == rtcinfo */
  printf("Test %02d: time = %10u --> ", i, time);
  result = convertNtpTimeToDateTime(time, &test_rtcinfo);

  if (result == TestResults[i])
  {
    /* Test seems to have passed: confirm the result */
    if (true == result)
    {
      printDateTime(&test_rtcinfo);
      if (true == checkDateTime(&test_rtcinfo, &rtcinfo))
      {
        /* Success! */
        ispass = true;
      }
      else
      {
        /* Failure: something is wrong with convert function */
        ispass = false;
      }
    }
    else /* false == result */
    {
      /* Success: convert function detected bad input data */
      printf("%-21s", "OUT OF BOUNDS");
      ispass = true;
    }
  }
  else
  {
    ispass = false;
  }

  if (ispass)
  {
    printf("PASS\n");
  }
  else
  {
    printf("FAIL\n");
  }

  return ispass;
};

/* ### Main function for running unit test of convertNtpTimeToDateTime ### */

bool runTest_RtcInfoToTime(uint8_t i)
{
  bool result, ispass;

  /* Define left and right sides of test conversion, "rtcinfo" and "time" */
  RTC_InfoStruct rtcinfo = TestRtcInfo[i];
  uint32_t time = TestTimes[i];

  /* This holds the calculated time */
  uint32_t test_time;

  /* RUN TEST: convert function should yield test_time == time */
  printf("Test %02d: ", i);
  printDateTime(&rtcinfo);
  printf(" --> ");

  result = convertDateTimeToNtpTime(&rtcinfo, &test_time);

  if (result == TestResults[i])
  {
    /* Test seems to have passed: confirm the result */
    if (true == result)
    {
      printf("time = %10u ", test_time);
      if (time == test_time)
      {
        /* Success! */
        ispass = true;
      }
      else
      {
        /* Failure: something is wrong with convert function */
        ispass = false;
      }
    }
    else /* false == result */
    {
      /* Success: convert function detected bad input data */
      printf("%-17s", "OUT OF BOUNDS");
      ispass = true;
    }
  }
  else
  {
    ispass = false;
  }

  if (ispass)
  {
    printf("PASS\n");
  }
  else
  {
    printf("FAIL\n");
  }

  return ispass;
}

int main(void)
{
  int i;

  /* Test conversion function: time -> rtcinfo */
  printf("### TEST time -> rtcinfo\n");
  for (i = 0; i < sizeof(TestTimes)/sizeof(*TestTimes); i++)
  {
    runTest_TimeToRtcInfo(i);
  }

  /* Test conversion function: rtcinfo -> time */
  printf("\n### TEST rtcinfo -> time\n");
  for (i = 0; i < sizeof(TestTimes)/sizeof(*TestTimes); i++)
  {
    runTest_RtcInfoToTime(i);
  }

  return 0;
};

/**
  * TEST FUNCTION
  *
  * Ensure that a block's days field is updated to contain only the remaining
  * days after some other days have been removed.
  */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFF_LEN (20)


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


typedef struct
{
  uint8_t days[7];
  bool week[7];
} SCH_show_st;


/**
  * Days are numbered 0 - 6 (Mon - Sun). If a day's number is listed in the
  * array, then in it is included in the block. Unused entries in the array
  * (there are 7 entries in total, for the 7 days) are populated with 8, meaning
  * an invalid day. Note that present days are listed first in the array, and
  * then the remaining entries are filled with 8.
  *
  * e.g. a block containing days Mon - Wed, and Fri, will have this array:
  *     { 0, 1, 2, 4, 8, 8, 8 }
  */
typedef struct
{
  uint8_t original_days[7];
  uint8_t remove_days[7];
  uint8_t final_days[7];
} test_case_st;


test_case_st test_cases[] =
{
                  /* original   */ /*   remove   */ /*    final     */
  /* [  0 ] */  { {0,1,2,4,8,8,8}, {1,8,8,8,8,8,8}, {0,2,4,8,8,8,8} },
  /* [  1 ] */  { {0,1,2,4,8,8,8}, {1,4,8,8,8,8,8}, {0,2,8,8,8,8,8} },
  /* [  2 ] */  { {0,1,2,4,8,8,8}, {0,2,4,8,8,8,8}, {1,8,8,8,8,8,8} },
  /* [  3 ] */  { {0,1,2,4,5,8,8}, {0,2,4,8,8,8,8}, {1,5,8,8,8,8,8} },
  /* [  4 ] */  { {0,1,2,3,4,5,6}, {0,2,4,8,8,8,8}, {1,3,5,6,8,8,8} },
  /* [  5 ] */  { {5,6,8,8,8,8,8}, {5,8,8,8,8,8,8}, {6,8,8,8,8,8,8} },
  /* [  6 ] */  { {0,1,2,3,4,5,6}, {8,8,8,8,8,8,8}, {0,1,2,3,4,5,6} },
  /* [  7 ] */  { {3,4,5,8,8,8,8}, {4,8,8,8,8,8,8}, {3,5,8,8,8,8,8} },
  /* [  8 ] */  { {3,4,5,1,6,2,0}, {4,6,8,8,8,8,8}, {3,5,1,2,0,8,8} },
};


static char buff0[BUFF_LEN];
static char buff1[BUFF_LEN];
static char buff2[BUFF_LEN];
static char *buffs[] = { buff0, buff1, buff2 };


static void updateBlockAfterRemovingDays(
    SCH_show_st *block, bool *remove_days );
static char *formatDays( uint8_t *days, uint8_t buff_idx );


int main( void )
{
  uint8_t i, j;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  test_case_st tc;
  SCH_show_st block;
  /* true if day i should be removed from the block */
  bool remove_days[7];

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* --- set up test --- */
    tc = test_cases[i];
    /* prepare block with original days */
    memcpy(block.days, tc.original_days, ARRAY_SIZE(tc.original_days));
    /* - prepare remove days array - */
    /* first set all days to false (won't be removed) */
    memset(remove_days, false, 7);
    /* then determine which days will be removed */
    for (j = 0; j < 7; j++)
    {
      if (tc.remove_days[j] < 7)
      {
        /* remove this day */
        remove_days[tc.remove_days[j]] = true;
      }
    }
    /* --- */

    /* run test */
    updateBlockAfterRemovingDays(&block, remove_days);

    /* check result */
    for (j = 0; (j < 7) && pass; j++)
    {
      if ((block.days[j] < 7) && (tc.final_days[j] != block.days[j]))
      {
        pass = false;
      }
    }

    /* print result */
    printf("test %2d: original: %-20s -> remove %-20s -> final: %-20s -> %s",
        i,
        formatDays(tc.original_days, 0),
        formatDays(tc.remove_days, 1),
        formatDays(tc.final_days, 2),
        pass ? "PASS" : "FAIL");

    if (pass)
    {
      printf("\n");
    }
    else /* fail */
    {
      /* print actual result */
      printf(" %s\n", formatDays(block.days, 0));
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}


static void updateBlockAfterRemovingDays(
    SCH_show_st *block, bool *remove_days )
{
  uint8_t i = 0, j = 0, k = 0;
  for (i = 0; i < 7; i++)
  {
    if (remove_days[i])
    {
      block->week[i] = false;
      /* find day i in the block and remove it */
      for (j = 0; j < 7; j++)
      {
        if (i == block->days[j])
        {
          /* found it; now remove it, shuffling entries up by one */
          for (k = j; k < 7 - 1; k++)
          {
            block->days[k] = block->days[k + 1];
          }
          block->days[6] = 0xFF;
        }
      }
    }
  }
}


/**
  * Format the days array into a string, using the specified buffer.
  * @param days  the days array (an array of uint8_t)
  * @buff_idx  the index of the buffer to use (0, 1 or 2)
  * @return a pointer to the selected buffer, containing the formatted string
  */
static char *formatDays( uint8_t *days, uint8_t buff_idx )
{
  char *buff = buffs[buff_idx];
  uint8_t n = 0;
  uint8_t i;

  /* first clear the buffer */
  memset(buff, '\0', BUFF_LEN);

  buff[n++] = '{';

  for (i = 0; i < 7; i++)
  {
    if (days[i] < 7)
    {
      buff[n++] = '0' + days[i];
      buff[n++] = ',';
    }
    else
    {
      /* no more days */
      break;
    }
  }

  buff[n++] = '}';
  buff[n] = '\0';

  return buff;
}

/**
  *
  */


#ifndef __GEN3_SDU_DEFS
#define __GEN3_SDU_DEFS


#include <stdbool.h>
#include <stdint.h>
#include "time_spinbox.h"


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


extern void GUI_SCH_getDayTimeFromSpinbox(
    uint8_t *day_hour, uint8_t *day_min, GUI_time_spinbox_st *spinbox );


#endif

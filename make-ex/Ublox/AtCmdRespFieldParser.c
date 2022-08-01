/**
 * @file AtCmdRespFieldParser.c
 *
 * @brief Provides parsing functions to extract fields from responses to AT commands
 *
 * https://www.wagz.com
 */

#include <string.h>
#include "AtCmdRespFieldParser.h"
#include "Debug.h"

#define AT_FIELD_LEN            (64)

#define CHARISNUM(x)            ((x) >= '0' && (x) <= '9')
#define CHAR2NUM(x)             ((x) - '0')

/* States of the AT command response field parser state machine */
enum PARSE_ATTEXT_STATE
{
  ATTEXT_HEADER,
  ATTEXT_FIND,
  ATTEXT_GET,
};

static void assignField(struct AtTextField *field, char *data, uint8_t len);
static int32_t parseNumber(char *ptr, uint8_t *cnt);
static char* safe_strcpy(char *dest, char *src, size_t n);

int AT_parseTextFields(const char* text,
                       const char *header,
                       struct AtTextField fields[],
                       uint8_t max_fields)
{
  const char *ptr = text;
  const char *end = ptr + strlen(text);
  const char *hdr = header;
  char buff[AT_FIELD_LEN];
  uint8_t num_fields = 0; /* return value */
  uint8_t count = 0; /* number of chars in a field */
  struct AtTextField *f = fields;
  enum PARSE_ATTEXT_STATE state;

  state = (NULL != header) ? ATTEXT_HEADER : ATTEXT_FIND;

  while (ptr <= end && num_fields < max_fields)
  {
    switch (state)
    {
      case ATTEXT_HEADER:
        if (*ptr == *hdr)
        {
          /* Processing header */
          ptr++;
          hdr++;
        }
        else if ('\0' == *hdr)
        {
          /* Found header. Do NOT advance the pointer, as the text field may
           * start immediately afterwards. */
          state = ATTEXT_FIND;
        }
        else
        {
          /* Header has not started yet */
          ptr++;
        }
        break;

      case ATTEXT_FIND:
        if (' ' != *ptr)
        {
          /* Next field starts now */
          f = fields + num_fields;
          buff[0] = *ptr;
          count = 1;
          state = ATTEXT_GET;
        }
        ptr++;
        break;

      case ATTEXT_GET:
        if (',' != *ptr && '\0' != *ptr)
        {
          /* Got one more character in the field */
          if (count < AT_FIELD_LEN - 1)
          {
            buff[count] = *ptr;
            count++;
          }
          else
          {
            /* Buffer about to overflow. Terminate it to be safe and stop. */
            buff[AT_FIELD_LEN-1] = '\0';
            assignField(f, buff, AT_FIELD_LEN);
            num_fields++;
            ptr = end; /* force the while loop to quit on the next run */
          }
        }
        else
        {
          /* Field complete */
          buff[count] = '\0';
          count++;
          assignField(f, buff, count);
          num_fields++;
          state = ATTEXT_FIND;
        }

        /* Move to the next character */
        ptr++;
        break;

      default:
        break;
    }
  }

  if (max_fields != num_fields)
  {
    LOG_WARNING("NOTE: Fields parsed = %d (expected %d)", num_fields, max_fields);
  }

  return num_fields;
}

/**
 * Assign the parsed data to the given field. This field is one member of an
 * array of fields from the parsed text response. If the particular field
 * represents a numerical value, not a string, convert it into an (int32_t)
 * number using `parseNumber`. If the field is a string, then string copy is
 * sufficient, as the parsed data is already a string.
 */
static void assignField(struct AtTextField *field, char *data, uint8_t len)
{
  if (field->type == NUMBER_FIELD)
  {
    unsigned int *number = field->val;
    *number = (unsigned int) parseNumber(data, &len);
  }
  else
  {
    /* STRING_FIELD */
    safe_strcpy((char *) field->val, data, len);
  }
}

/**
 * Convert the ASCII representation of the number referenced by ptr into an
 * actual uint32_t number. The `cnt` variable indicates the number of chars in
 * the ASCII representation. Source: ST Micro.
 */
static int32_t parseNumber(char *ptr, uint8_t *cnt)
{
  uint8_t minus = 0, i = 0;
  int32_t sum = 0;

  /* Check for minus character */
  if (*ptr == '-')
  {
    minus = 1;
    ptr++;
    i++;
  }

  /* Parse number */
  while (CHARISNUM(*ptr))
  {
    sum = 10 * sum + CHAR2NUM(*ptr);
    ptr++;
    i++;
  }

  if (cnt != NULL)
  {
    /* Save number of chars used */
    *cnt = i;
  }

  if (minus)
  {
    /* Minus detected */
    return 0 - sum;
  }

  return sum;
}

/**
  * Safe version of `strcpy` and `strncpy`. Only copy up to `n` chars from `src`
  * into `dest`, INCLUDING the NULL byte. If there are less than `n` chars in
  * `src`, only these chars will be copied. Note that:
  * - `strcpy` is not safe because `src` may be bigger than `dest` (buffer overflow)
  * - `strncpy` is not safe because it doesn't automatically include the NULL byte
  */
static char* safe_strcpy(char *dest, char *src, size_t n)
{
  size_t i = 0;

  while (('\0' != src[i]) && (i < n-1))
  {
    dest[i] = src[i];
    i++;
  }

  dest[i] = '\0';

  return dest;
}

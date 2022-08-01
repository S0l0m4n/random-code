/**
 * @file AtCmdRespFieldParser.h
 *
 * https://www.wagz.com
 */

#include <stdint.h>

enum FIELD_TYPE
{
  NUMBER_FIELD,
  STRING_FIELD,
};

struct AtTextField
{
  void *val; /* pointer to string (char *) or number (uint8_t *) */
  enum FIELD_TYPE type; /* NUMBER or STRING */
};

/**
 * @brief Parses fields from the AT text response
 * @param text  buffer containing response text (null-terminated)
 * @param header  prefix of the text response, or NULL if none
 * @param fields  array of TextField structures, to be filled with parsed fields
 * @param max_fields  number of elements in the 'fields' array
 * @return the actual number of fields found in the text response
 *
 * @note Every STRING-type field has a max length of AT_FIELD_LEN
 *
 * @desc A pointer starts at the text string and moves along it until the end.
 * First the header is detected. Then each field, delimited by a comma, is
 * extracted and the respective element in the `fields` array is made to point at
 * it. White space at the start of each field is skipped over before extraction,
 * if it exists. Any numeric field is further converted to a number using
 * parseNumber. When the pointer reaches the end of the string, the function will
 * terminate. A count of extracted fields is kept along the way, and this value
 * is returned.
 */
int AT_parseTextFields(const char* text,
                       const char *header,
                       struct AtTextField fields[],
                       uint8_t max_fields);

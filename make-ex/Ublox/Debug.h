/**
 * @file Debug.h
 *
 * @note Mocked version!
 */

#include <stdio.h>

#define LOG_INFO(x, ...) printf(x, ##__VA_ARGS__)
#define LOG_WARNING(x, ...) printf(x, ##__VA_ARGS__)

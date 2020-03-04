#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "defines.h"

bool strIsValidNumber(char *str) {
  if (str == NULL) {
    return false;
  }
  if (str[0] == '-') {
    str++;
  }
  while (*str) {
    if (*str < '0' || '9' < *str) {
      return false;
    }
    str++;
  }
  return true;
}

int strGetYear(char *str) {
  if (!strIsValidNumber(str)) {
    return 0;
  }

  long long year = strtoll(str, NULL, 10);
  if (errno == ERANGE || year > MAX_YEAR || year < MIN_YEAR) {
    errno = 0;
    year = 0;
  }
  return (int)year;
}

unsigned strGetRouteId(char *str) {
  if (!strIsValidNumber(str)) {
    return 0;
  }

  long long routeId = strtoll(str, NULL, 10);
  if (errno == ERANGE || routeId < 0 || routeId > MAX_ROUTE_ID) {
    errno = 0;
    routeId = 0;
  }
  return (unsigned)routeId;
}

unsigned strGetLength(char *str) {
  if (!strIsValidNumber(str)) {
    return 0;
  }

  long long length = strtoll(str, NULL, 10);
  if (errno == ERANGE || length < 0 || length > MAX_LENGTH) {
    errno = 0;
    length = 0;
  }
  return (unsigned)length;
}

bool isValidCityName(const char *city) {
  if (city == NULL) {
    return false;
  }
  if (*city == '\0') {
    return false;
  }
  while (*city) {
    if ((0 <= *city && *city <= 31) || *city == ';') {
      return false;
    }
    city++;
  }
  return true;
}

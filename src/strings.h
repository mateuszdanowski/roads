/** @file
 * Pomocne funkcje sprawdzające stringi.
 */

#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stdbool.h>

/** @brief Sprawdza, czy napis przechowuje poprawną liczbę całkowitą int.
 * @param[in] str  – wskaźnik na napis.
 * @return Wartość, przechowywana przez napis, bądź 0 jeśli jest niepoprawna.
 */
bool strIsValidNumber(char *str);

/** @brief Zamienia stringa na inta.
 * Jeżeli string jest poprawną liczbą int, zamienia go na tą liczbę.
 * Wpp zamienia go na liczbę 0.
 * @param[in] str  – wskaźnik na napis.
 * @return Wartość, przechowywana przez napis, bądź 0 jeśli jest niepoprawna.
 */
int strGetYear(char *str);

/** @brief Zamienia stringa na unsigned pomiędzy 0 a 999.
 * Jeżeli string jest poprawnym routeId, zamienia go na tą liczbę.
 * Wpp zamienia go na liczbę 0.
 * @param[in] str  – wskaźnik na napis.
 * @return Wartość, przechowywana przez napis, bądź 0 jeśli jest niepoprawna.
 */
unsigned strGetRouteId(char *str);

/** @brief Zamienia stringa na unsigned.
 * Jeżeli string jest poprawną liczbą unsigned, zamienia go na tą liczbę.
 * Wpp zamienia go na liczbę 0.
 * @param[in] str  – wskaźnik na napis.
 * @return Wartość, przechowywana przez napis, bądź 0 jeśli jest niepoprawna.
 */
unsigned strGetLength(char *str);

/** @brief Sprawdza poprawność nazwy miasta.
 * Sprawdza, czy nazwa miasta nie zawiera niedozwolonych znaków.
 * @param[in] city  – wskaźnik na napis.
 * @return Wartość @p true, jesli nazwa miasta jest dopuszczalna.
 * Wartość @p false wpp.
 */
bool isValidCityName(const char *city);

#endif  // __STRINGS_H

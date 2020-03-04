/** @file
 * Interfejs klasy drogi krajowej
 */

#ifndef __NATIONAL_ROUTE_H__
#define __NATIONAL_ROUTE_H__

#include <stdbool.h>

#include "cities_list.h"

/**
 * Struktura przechowująca drogę krajową.
 */
typedef struct NationalRoute {
  int id;            ///< id drogi krajowej
  CitiesList *list;  ///< lista miast na drodze krajowej
} NationalRoute;

/** @brief Tworzy strukturę.
 * Tworzy nową, pustą drogę krajową niezawierającą żadnych miast.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
NationalRoute *newNationalRoute();

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p nationalRoute.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] nationalRoute – wskaźnik na usuwaną strukturę.
 */
void deleteNationalRoute(NationalRoute *nationalRoute);

/** @brief Usuwa strukturę.
 * Usuwa wszystkie struktury wskazywane przez @p nationalRoute.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] nationalRoute – wskaźnik na usuwaną strukturę.
 */
void deleteNationalRoutes(NationalRoute **nationalRoute);

/** @brief Dodaje segment drogi krajowej.
 * Dodaje element do drogi krajowej wskazywanej przez @p nationalRoute
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] nationalRoute – wskaźnik na strukturę.
 * @param[in] city - wskaźnik na odpowiadający miastu węzeł.
 */
bool addNationalRouteSection(NationalRoute *nationalRoute, void *city);

/** @brief Dodaje listę wierzchołków do drogi krajowej.
 * Dodaje listę za elementem @p node.
 * Nic nie robi, jeśli lista jest pusta.
 * @param[in] node – wskaźnik na wierzchołek w liście drogi krajowej;
 * @param[in] list - wskaźnik na dodawaną listę.
 */
void addAfterRouteSection(CitiesListNode *node, CitiesList *list);

#endif  // __NATIONAL_ROUTE_H__

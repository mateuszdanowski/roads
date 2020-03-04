/** @file
 * Interfejs klasy przechowującej listę dróg krajowych
 */

#ifndef __ROUTES_LIST_H__
#define __ROUTES_LIST_H__

#include <stdbool.h>

/**
 * Element węzła listy dróg krajowych.
 */
typedef struct RoutesListElement {
  unsigned routeId;  ///< numer drogi krajowej
} RoutesListElement;

/**
 * Węzeł listy dróg krajowych.
 */
typedef struct RoutesListNode {
  RoutesListElement elem;       ///< element przechowywany przez węzeł
  struct RoutesListNode *prev;  ///< wskaźnik na poprzedni element w liście
  struct RoutesListNode *next;  ///< wskaźnik na następny element w liście
} RoutesListNode;

/**
 * Lista dróg, reprezentowana przez głowę i ogon.
 */
typedef struct RoutesList {
  RoutesListNode *head;  ///< wskaźnik na głowę listy
  RoutesListNode *tail;  ///< wskaźnik na ogon listy
} RoutesList;

/** @brief Tworzy nowy element węzła listy.
 * @param[in] routeId – numer drogi krajowej
 * @return Element węzła.
 */
RoutesListElement newRoutesListElem(unsigned routeId);

/** @brief Tworzy nowy węzeł listy.
 * Tworzy nowy węzeł, którego sąsiadami są prev i next.
 * @param[in] prev  – wskaźnik na poprzedni element w liście
 * @param[in] elem  – element węzła;
 * @param[in] next  – wskaźnik na następny element w liście.
 * @return Wskaźnik na węzeł.
 */
RoutesListNode *newRoutesListNode(RoutesListNode *prev, RoutesListElement elem,
                                  RoutesListNode *next);

/** @brief Tworzy nową listę.
 * @return Wskaźnik na listę.
 */
RoutesList *newRoutesList();

/** @brief Sprawdza, czy dany wierzchołek jest poprawnym wierzchołkiem listy.
 * @param[in] node - wskaźnik na dany wierzchołek
 * @return Wartość @p true, jeśli wierzchołek jest poprawny.
 * Wartość @p false wpp.
 */
bool isValidRoutesListNode(RoutesListNode *node);

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł wskazywany przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node – wskaźnik na usuwany wierzchołek.
 */
void removeRoutesListNode(RoutesListNode *node);

/** @brief Usuwa ostatni węzeł listy.
 * Usuwa ostatni węzeł listy wskazywanej przez @p list.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void popBackRoutesList(RoutesList *list);

/** @brief Usuwa listę.
 * Usuwa listę wskazywanej przez @p list.
 * Nic nie robi, jeśli @p list ma wartość NULL.
 * @param[in] list – wskaźnik na listę.
 */
void deleteRoutesList(RoutesList *list);

/** @brief Tworzy i dodaje wierzchołek do listy.
 * @param[in] list  – wskaźnik na listę.
 * @param[in] routeId  – numer drogi krajowej
 * @return Wartość @p true, jesli udało się dodać wierzchołek;
 * Wartość @p false wpp.
 */
bool addRoutesListNode(RoutesList *list, unsigned routeId);

/** @brief Usuwa węzeł listy o wartości @p routeId.
 * Wyszukuje węzeł z listy, który ma wartość @p routeId i go usuwa.
 * Nic nie robi, jeśli lista jest pusta.
 * @param[in] list – wskaźnik na usuwany wierzchołek;
 * @param[in] routeId – numer drogi krajowej.
 */
void removeRoutesListNodeById(RoutesList *list, unsigned routeId);

#endif  // __ROUTES_LIST_H__

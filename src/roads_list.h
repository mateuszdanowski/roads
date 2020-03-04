/** @file
 * Interfejs klasy przechowującej listę odcinków dróg
 */

#ifndef __ROADS_LIST_H__
#define __ROADS_LIST_H__

#include "routes_list.h"

/**
 * Element węzła listy dróg.
 */
typedef struct RoadsListElement {
  void *city;          ///< wskaźnik na odpowiadający miastu węzeł w drzewie
  unsigned length;     ///< długość odcinka drogi
  int builtYear;       ///< rok budowy odcinka drogi
  RoutesList *routes;  ///< lista dróg krajowych, w których jest dany odcinek
} RoadsListElement;

/**
 * Węzeł listy dróg.
 */
typedef struct RoadsListNode {
  RoadsListElement elem;       ///< element przechowywany przez węzeł
  struct RoadsListNode *prev;  ///< wskaźnik na poprzedni element w liście
  struct RoadsListNode *next;  ///< wskaźnik na następny element w liście

} RoadsListNode;

/**
 * Lista dróg, reprezentowana przez głowę i ogon.
 */
typedef struct RoadsList {
  RoadsListNode *head;  ///< wskaźnik na głowę listy
  RoadsListNode *tail;  ///< wskaźnik na ogon listy
} RoadsList;

/** @brief Tworzy nowy element węzła listy.
 * @param[in] city      – wskaźnik na odpowiadający miastu węzeł;
 * @param[in] length    – długość odcinka drogi;
 * @param[in] builtYear – rok budowy odcinka drogi.
 * @return Element węzła.
 */
RoadsListElement newRoadsListElem(void *city, unsigned length, int builtYear);

/** @brief Tworzy nowy węzeł listy.
 * Tworzy nowy węzeł, którego sąsiadami są prev i next.
 * @param[in] prev  – wskaźnik na poprzedni element w liście
 * @param[in] elem  – element węzła;
 * @param[in] next  – wskaźnik na następny element w liście.
 * @return Wskaźnik na węzeł.
 */
RoadsListNode *newRoadsListNode(RoadsListNode *prev, RoadsListElement elem,
                                RoadsListNode *next);

/** @brief Tworzy nową listę.
 * @return Wskaźnik na listę.
 */
RoadsList *newRoadsList();

/** @brief Sprawdza, czy dany wierzchołek jest poprawnym wierzchołkiem listy.
 * @param[in] node - wskaźnik na dany wierzchołek
 * @return Wartość @p true, jeśli wierzchołek jest poprawny.
 * Wartość @p false wpp.
 */
bool isValidRoadsListNode(RoadsListNode *node);

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł wskazywany przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node – wskaźnik na usuwany wierzchołek.
 */
void removeRoadsListNode(RoadsListNode *node);

/** @brief Usuwa ostatni węzeł listy.
 * Usuwa ostatni węzeł listy wskazywanej przez @p list.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void popBackRoadsList(RoadsList *list);

/** @brief Usuwa listę.
 * Usuwa listę wskazywanej przez @p list.
 * Nic nie robi, jeśli @p list ma wartość NULL.
 * @param[in] list – wskaźnik na listę.
 */
void deleteRoadsList(RoadsList *list);

/** @brief Tworzy i dodaje wierzchołek do listy.
 * @param[in] list  – wskaźnik na listę.
 * @param[in] city  – wskaźnik na odpowiadający miastu węzeł;
 * @param[in] length  – długość odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jesli udało się dodać wierzchołek.
 * Wartość @p false wpp.
 */
bool addRoadsListNode(RoadsList *list, void *city, unsigned length,
                      int builtYear);

#endif  // __ROADS_LIST_H__

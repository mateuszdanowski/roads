/** @file
 * Interfejs klasy przechowującej listę wskaźników do miast
 */

#ifndef __CITIES_LIST_H__
#define __CITIES_LIST_H__

#include <stdbool.h>

/**
 * Element węzła listy miast.
 */
typedef struct CitiesListElement {
  void *city;  ///< wskaźnik na odpowiadający miastu węzeł w mapie
} CitiesListElement;

/**
 * Węzeł listy miast.
 */
typedef struct CitiesListNode {
  CitiesListElement elem;       ///< element przechowywany przez węzeł
  struct CitiesListNode *prev;  ///< wskaźnik na poprzedni element w liście
  struct CitiesListNode *next;  ///< wskaźnik na następny element w liście
} CitiesListNode;

/**
 * Lista miast, reprezentowana przez głowę i ogon.
 */
typedef struct CitiesList {
  CitiesListNode *head;  ///< wskaźnik na głowę listy
  CitiesListNode *tail;  ///< wskaźnik na ogon listy
} CitiesList;

/** @brief Tworzy nowy element węzła listy.
 * @param[in] city – wskaźnik na odpowiadający miastu węzeł.
 * @return Element węzła.
 */
CitiesListElement makeNewCitiesListElem(void *city);

/** @brief Tworzy nowy węzeł listy.
 * Tworzy nowy węzeł, którego sąsiadami są prev i next.
 * @param[in] prev – wskaźnik na poprzedni element w liście
 * @param[in] elem – element węzła;
 * @param[in] next – wskaźnik na następny element w liście.
 * @return Wskaźnik na węzeł.
 */
CitiesListNode *makeNewCitiesListNode(CitiesListNode *prev,
                                      CitiesListElement elem,
                                      CitiesListNode *next);

/** @brief Tworzy nową listę.
 * @return Wskaźnik na listę.
 */
CitiesList *makeNewCitiesList();

/** @brief Sprawdza, czy dany wierzchołek jest poprawnym wierzchołkiem listy.
 * @param[in] node - wskaźnik na dany wierzchołek
 * @return Wartość @p true, jeśli wierzchołek jest poprawny.
 * Wartość @p false wpp.
 */
bool isValidCitiesListNode(CitiesListNode *node);

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł wskazywany przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node – wskaźnik na usuwany wierzchołek.
 */
void removeCitiesListNode(CitiesListNode *node);

/** @brief Usuwa ostatni węzeł listy.
 * Usuwa ostatni węzeł listy wskazywanej przez @p list.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void popBackCitiesList(CitiesList *list);

/** @brief Usuwa listę.
 * Usuwa listę wskazywanej przez @p list.
 * Nic nie robi, jeśli @p list ma wartość NULL.
 * @param[in] list – wskaźnik na listę.
 */
void deleteCitiesList(CitiesList *list);

/** @brief Tworzy i dodaje wierzchołek do listy.
 * @param[in] list – wskaźnik na listę.
 * @param[in] city – wskaźnik na odpowiadający miastu węzeł;
 * @return Wartość @p true, jesli udało się dodać wierzchołek.
 * Wartość @p false wpp.
 */
bool addCitiesListNode(CitiesList *list, void *city);

/** @brief Sprawdza, czy lista jest pusta.
 * @param[in] list – wskaźnik na listę.
 * @return Wartość @p true, jesli lista jest pusta.
 * Wartość @p false wpp.
 */
bool isEmptyCitiesList(CitiesList *list);

/** @brief Zwraca wierzchołek z początku listy.
 * @param[in] list – wskaźnik na listę.
 * @return Wskaźnik na węzeł z początku listy.
 */
CitiesListNode *getCitiesListFront(CitiesList *list);

/** @brief Usuwa pierwszy węzeł listy.
 * Usuwa pierwszy węzeł listy wskazywanej przez @p list.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void popFrontCitiesList(CitiesList *list);

/** @brief Odwraca listę.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void reverseCitiesList(CitiesList *list);

#endif  // __CITIES_LIST_H__

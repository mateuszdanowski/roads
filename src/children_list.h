/** @file
 * Interfejs klasy przechowującej listę dzieci w mapie
 */

#ifndef __CHILDREN_LIST_H__
#define __CHILDREN_LIST_H__

#include <stdbool.h>

/**
 * Element węzła listy dzieci.
 */
typedef struct ChildrenListElement {
  char character;  ///< znak, z którym identyfikowany jest węzeł w drzewie
  struct Trie *child;  ///< wskaźnik na dziecko w drzewie
} ChildrenListElement;

/**
 * Węzeł listy dzieci.
 */
typedef struct ChildrenListNode {
  ChildrenListElement elem;       ///< element przechowywany przez węzeł
  struct ChildrenListNode *prev;  ///< wskaźnik na poprzedni element w liście
  struct ChildrenListNode *next;  ///< wskaźnik na następny element w liście
} ChildrenListNode;

/**
 * Lista dzieci, reprezentowana przez głowę i ogon.
 */
typedef struct ChildrenList {
  ChildrenListNode *head;  ///< wskaźnik na głowę listy
  ChildrenListNode *tail;  ///< wskaźnik na ogon listy

} ChildrenList;

/** @brief Tworzy nowy element węzła listy.
 * @param[in] child – wskaźnik na dziecko w drzewie
 * @param[in] character – znak, z którym identyfikowany jest węzeł w drzewie
 * @return Element węzła.
 */
ChildrenListElement newChildrenListElem(void *child, char character);

/** @brief Tworzy nowy węzeł listy.
 * Tworzy nowy węzeł, którego sąsiadami są prev i next.
 * @param[in] prev – wskaźnik na poprzedni element w liście
 * @param[in] elem – element węzła;
 * @param[in] next – wskaźnik na następny element w liście.
 * @return Wskaźnik na węzeł.
 */
ChildrenListNode *newChildrenListNode(ChildrenListNode *prev,
                                      ChildrenListElement elem,
                                      ChildrenListNode *next);

/** @brief Tworzy nową listę.
 * @return Wskaźnik na listę.
 */
ChildrenList *newChildrenList();

/** @brief Sprawdza, czy dany wierzchołek jest poprawnym wierzchołkiem listy.
 * @param[in] node - wskaźnik na dany wierzchołek
 * @return Wartość @p true, jeśli wierzchołek jest poprawny.
 * Wartość @p false wpp.
 */
bool isValidChildrenListNode(ChildrenListNode *node);

/** @brief Usuwa węzeł listy.
 * Usuwa węzeł wskazywany przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node – wskaźnik na usuwany wierzchołek.
 */
void removeChildrenListNode(ChildrenListNode *node);

/** @brief Usuwa ostatni węzeł listy.
 * Usuwa ostatni węzeł listy wskazywanej przez @p list.
 * Nic nie robi, jeśli lista nie ma żadnych elementów.
 * @param[in] list – wskaźnik na listę.
 */
void popBackChildrenList(ChildrenList *list);

/** @brief Usuwa listę.
 * Usuwa listę wskazywanej przez @p list.
 * Nic nie robi, jeśli @p list ma wartość NULL.
 * @param[in] list – wskaźnik na listę.
 */
void deleteChildrenList(ChildrenList *list);

/** @brief Tworzy i dodaje wierzchołek do listy.
 * @param[in] list – wskaźnik na listę.
 * @param[in] character – znak, z którym identyfikowany jest węzeł w drzewie.
 * @return Wartość @p true, jesli udało się dodać wierzchołek.
 * Wartość @p false wpp.
 */
bool addChildrenListNode(ChildrenList *list, char character);

#endif  // __CHILDREN_LIST_H__

/** @file
 * Interfejs drzewa Trie, przechowującego nazwy miast.
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>

#include "children_list.h"
#include "roads_list.h"

/**
 * Struktura drzewa Trie przechowująca napisy, reprezentujące miasta.
 */
typedef struct Trie {
  ChildrenList *children;  ///< lista dzieci wierzchołka trie
  bool isLeaf;             ///< informacja o tym, czy węzeł jest liściem
  int id;                  ///< numer wierzchołka
  RoadsList *roads;  ///< lista odcinków dróg, które wychodzą z danego miasta
  char character;    ///< odpowiadający węzłowi znak
  struct Trie *parent;  ///< przodek węzła
} Trie;

/** @brief Tworzy nową strukturę.
 * Tworzy nowy wierzchołek drzewa Trie.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Trie *newTrieNode();

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node – wskaźnik na usuwaną strukturę.
 */
void deleteTrie(Trie *node);

/** @brief Zwraca wskaźnik do węzła reprezentującego miasto.
 * Jeśli miasto istnieje, zwraca do niego wskaźnik;
 * w przeciwnym razie zwraca NULL.
 * @param[in,out] root - wskaźnik na korzeń Trie;
 * @param[in] city – wskaźnik na napis.
 * @return Wskaźnik do miasta lub NULL, gdy miasto nie istnieje.
 */
Trie *getNodePtr(Trie *root, const char *city);

/** @brief Dodaje do drzewa Trie nowe miasto.
 * Jeśli miasto już istnieje, nic nie robi.
 * @param[in,out] root – wskaźnik na korzeń Trie;
 * @param[in] city – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] id – numer miasta.
 * @return Wartość @p true, jeśli miasto zostało dodane.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool insertStr(Trie *root, const char *city, int id);

/** @brief Sprawdza czy miasta są sąsiadami.
 * Sprawdza, czy @p neighbour występuje wśród sąsiadów @p city.
 * @param[in] city – wskaźnik na miasto.
 * @param[in] neighbour - wskaźnik na miasto sąsiednie.
 * @return Wartość @p true, jeśli @p neighbour jest sąsiadem @p city.
 * Wartość @p false wpp.
 */
bool isNeighbour(Trie *city, Trie *neighbour);

/** @brief Dodaje drogę pomiędzy @p city1 a @p city2.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość wynik funkcji @ref addRoadsListNode
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoadSection(Trie *city1, Trie *city2, unsigned length, int builtYear);

/** @brief Zwraca rok budowy lub naprawy odcinka drogi między
 * @p city a @p neighbour. Oba miasta muszą ze sobą sąsiadować.
 * @param[in] city  – wskaźnik na pierwsze miasto.
 * @param[in] neighbour - wskaźnik na drugie miasto.
 * @return Rok budowy lub remontu odcinka drogi między podanymi miastami.
 */
int getRepairYear(Trie *city, Trie *neighbour);

/** @brief Zwraca długość odcinka drogi między
 * @p city a @p neighbour. Oba miasta muszą ze sobą sąsiadować.
 * @param[in] city  – wskaźnik na pierwsze miasto.
 * @param[in] neighbour - wskaźnik na drugie miasto.
 * @return Długość odcinka drogi między podanymi miastami.
 */
unsigned getRoadLength(Trie *city, Trie *neighbour);

/** @brief Zmienia wartość repairYear w liście dróg miasta city.
 * Oba miasta muszą ze sobą sąsiadować. Nie zmienia wartości
 * repairYear w liście dróg miasta neighbour.
 * @param[in] city  – wskaźnik na pierwsze miasto.
 * @param[in] neighbour - wskaźnik na drugie miasto.
 * @param[in] repairYear - rok budowy lub remontu
 */
void repairRoadSection(Trie *city, Trie *neighbour, int repairYear);

/** @brief Zwraca wskaźnik na drogę pomiędzy miastami.
 * Zwraca wskaźnik na drogę pomiędzy @p city i @p neighbour.
 * Miasto @p city musi mieć miasto @p neighbor jako sąsiada.
 * @param[in] city - wskaźnik na miasto
 * @param[in] neighbour – wskaźnik na sąsiada.
 * @return Wskaźnik do drogi między tymi miastami.
 */
RoadsListNode *getRoadBetweenCities(Trie *city, Trie *neighbour);

#endif  // __TRIE_H__

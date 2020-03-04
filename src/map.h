/** @file
 * Interfejs klasy przechowującej mapę dróg krajowych
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdbool.h>
#include <stdlib.h>

#include "national_route.h"
#include "trie.h"

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map {
  Trie *trie;  ///< struktura przechowująca nazwy miast oraz odcinki dróg
  NationalRoute **nationalRoutes;  ///< tablica przechowująca drogi krajowe
  int numOfCities;  ///< zmienna przechowująca liczbę miast dodanych do mapy
} Map;

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map *newMap(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map);

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2, unsigned length,
             int builtYear);

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear);

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2);

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city);

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2);

/** @brief Usuwa z mapy dróg drogę krajową o podanym numerze.
 * @param[in,out] map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId – numer drogi krajowej.
 * @return Wartość @p true, jeśli udało się usunąć drogę krajową.
 * Wpp wartość @p false.
 */
bool removeRoute(Map *map, unsigned routeId);

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const *getRouteDescription(Map *map, unsigned routeId);

/** @brief Zwraca wskaźnik do węzła reprezentującego miasto.
 * Jeśli miasto istnieje, zwraca do niego wskaźnik;
 * W przeciwnym razie zwraca NULL.
 * @param[in,out] map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city  – wskaźnik na napis.
 * @return Wskaźnik do miasta lub NULL, gdy miasto nie istnieje.
 */
Trie *getCityPtr(Map *map, const char *city);

/** @brief Dodaje miasto do mapy.
 * @param[in,out] map - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city  – wskaźnik na napis.
 * @return Wartość @p true, jeśli udało się dodać miasto.
 * Wpp wartość @p false.
 */
bool addCity(Map *map, const char *city);

/** @brief Łączy dwa stringi w jeden.
 * Do stringa wskazywanego przez @p str dodaje nazwę miasta
 * wskazywanego przez @p city.
 * @param[in,out] str - wskaźnik na napis;
 * @param[in] strLength  – długość napisu;
 * @param[in] pos  – numer ostatniego dodanego znaku;
 * @param[in] city  – wskaźnik na miasto.
 */
void concatenateCityNameToResult(char **str, size_t *strLength,
                                 unsigned int *pos, Trie *city);

/** @brief Łączy dwa stringi w jeden.
 * Do stringa wskazywanego przez @p str dodaje wartość @p val.
 * @param[in,out] str - wskaźnik na napis;
 * @param[in] strLength  – długość napisu;
 * @param[in] pos  – numer ostatniego dodanego znaku;
 * @param[in] val  – liczba.
 */
void concatenateIntToResult(char **str, size_t *strLength, unsigned int *pos,
                            int val);

/** @brief Łączy dwa stringi w jeden.
 * Do stringa wskazywanego przez @p str dodaje wartość @p val.
 * @param[in,out] str - wskaźnik na napis;
 * @param[in] strLength  – długość napisu;
 * @param[in] pos  – numer ostatniego dodanego znaku;
 * @param[in] val  – liczba.
 */
void concatenateUnsignedToResult(char **str, size_t *strLength,
                                 unsigned int *pos, unsigned val);

/** @brief Sprawdza, czy dane miasto jest częścią danej drogi krajowej.
 * @param[in] city  – wskaźnik na miasto;
 * @param[in] route  – wskaźnik na drogę krajową.
 * @return Wartość @p true, jeśli dane miasto jest częścią drogi krajowej.
 * Wpp wartość @p false.
 */
bool isCityInRoute(Trie *city, NationalRoute *route);

/** @brief Zamienia tablicę prev na listę miast.
 * @param[in] prev  – wskaźnik na tablicę;
 * @param[in] cityPtr  – wskaźnik na miasto.
 * @return Wskaźnik na listę miast lub NULL,
 * gdy działanie się nie powiodło.
 */
CitiesList *prevToCitiesList(Trie **prev, Trie *cityPtr);

/**
 * Struktura przechowująca wynik funkcji spfa.
 */
typedef struct SpfaResult {
  unsigned dist;   ///< dlugosc drogi
  bool isCorrect;  ///< informacja, czy droga jest poprawna
  int minYear;     ///< minimalny rok budowy na drodze
  Trie **prev;     ///< tablica przodków

} SpfaResult;

/** @brief Tworzy nową strukturę.
 * @return Wskaźnik na strukturę przechowującą wynik funkcji spfa.
 */
SpfaResult *makeNewSpfaResult();

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p result.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] result – wskaźnik na usuwaną strukturę.
 */
void deleteResult(SpfaResult *result);

/** @brief Wyszukuje najkrótszą drogę z miasta @p startCity
 * do miasta @p finalCity.
 * @param[in] m  – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId  – numer drogi krajowej;
 * @param[in] startCity – wskaźnik na miasto startowe;
 * @param[in] finalCity – wskaźnik na miasto końcowe.
 * @return Wskaźnik na wynik lub NULL, gdy nie udało się zaalokować pamięci.
 */
SpfaResult *spfa(Map *m, unsigned routeId, Trie *startCity, Trie *finalCity);

/** @brief Porównuje dwa wyniki działania funkcji spfa
 * i wybiera pośród nich najlepszy.
 * @param[in] fstResult  – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] sndResult  – numer drogi krajowej;
 * @return 0, gdy oba wyniki są nieprawidłowe;
 * 1, gdy pierwszy wynik jest lepszy;
 * 2, gdy drugi wynik jest lepszy;
 */
int getMinimalResult(SpfaResult *fstResult, SpfaResult *sndResult);

/** @brief Oznacza drogi między miastami na liście @p list,
 * jako należące do drogi krajowej o numerze @p routeId.
 * @param[in] list - wskaźnik na listę miast;
 * @param[in] routeId  – numer drogi krajowej.
 */
void markRoadsWithRoute(CitiesList *list, unsigned routeId);

/** @brief Odznacza drogi między miastami na liście @p list,
 * jako należące do drogi krajowej o numerze @p routeId.
 * @param[in] list - wskaźnik na listę miast;
 * @param[in] routeId  – numer drogi krajowej.
 */
void undoMarkRoadsWithRoute(CitiesList *list, unsigned routeId);

/** @brief Dodaje drogę krajową.
 * @param[in,out] m      – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] cityPtr    – wskaźnik na miasto końcowe;
 * @param[in] prev       – wskaźnik na tablicę poprzednich miast.
 * @return Wartość @p true, jeśli droga krajowa została dodana.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool addRoute(Map *m, unsigned routeId, Trie *cityPtr, Trie **prev);

/** @brief Sprawdza, czy dana droga krajowa przechodzi przez dane miasto.
 * @param[in] city  – wskaźnik na miasto;
 * @param[in] neighbour  – wskaźnik na sąsiednie miasto
 * @param[in] routeId  – numer drogi krajowej.
 * @return Wartość @p true, jeśli dana droga jest częścią drogi krajowej.
 * Wpp wartość @p false.
 */
bool isRouteinRoad(Trie *city, Trie *neighbour, unsigned routeId);

/** @brief Funkcja, sprawdzająca poprawność działania @ref markRoadsWithRoute.
 * @param[in,out] m – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId  – numer drogi krajowej.
 * @return Wartość @p true, jeśli @ref markRoadsWithRoute zadziałała poprawnie.
 * Wpp wartość @p false.
 */
bool checkRoute(Map *m, unsigned routeId);

/** @brief Sprawdza, czy usunięci danej drogi nie spowoduje zepsucia danej
 * drogi krajowej.
 * @param[in,out] m – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1  – wskaźnik na pierwsze miasto;
 * @param[in] city2  – wskaźnik na drugie miasto;
 * @param[in] routeId  – numer drogi krajowej.
 * @return Wartość @p true, jeśli usunięcie drogi nie zepuje drogi krajowej;
 * Wpp wartość @p false.
 */
bool isPossibleToReplaceRoadInRoute(Map *m, Trie *city1, Trie *city2,
                                    unsigned routeId);

/** @brief Uzupełnia drogę krajową pomiędzy miastami.
 * Wyszukuje najkrótszą drogę pomiędzy miastami @p city1 i @p city2;
 * Uzupełnia drogę krajową o numerze @p routeId nowo znalezioną drogą
 * pomiędzy podanymi miastami.
 * @param[in,out] m – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1  – wskaźnik na pierwsze miasto;
 * @param[in] city2  – wskaźnik na drugie miasto;
 * @param[in] routeId  – numer drogi krajowej.
 * @return Wartość @p false, jeśli nie udało się zaalokować pamięci;
 * Wpp wartość @p true.
 */
bool replaceRoadInRoute(Map *m, Trie *city1, Trie *city2, unsigned routeId);

/** @brief Usuwa drogę pomiędzy miastami.
 * Usuwa drogę wychodzącą z miasta @p city1 do miasta @p city2.
 * @param[in] city – wskaźnik na miasto;
 * @param[in] neighbour – wskaźnik na sąsiada.
 */
void removeRoadFromCity(Trie *city, Trie *neighbour);

#endif  // __MAP_H__

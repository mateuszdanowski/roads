#include "map.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cities_list.h"
#include "defines.h"
#include "national_route.h"
#include "strings.h"
#include "trie.h"

Map *newMap() {
  Map *map = (Map *)malloc(sizeof(Map));
  if (map == NULL) {
    return NULL;
  }

  map->trie = newTrieNode();
  if (map->trie == NULL) {
    free(map);
    map = NULL;
    return NULL;
  }

  map->nationalRoutes =
      (NationalRoute **)malloc(1000 * sizeof(NationalRoute *));
  if (map->nationalRoutes == NULL) {
    deleteTrie(map->trie);
    free(map);
    map = NULL;
    return NULL;
  }

  memset(map->nationalRoutes, 0, 1000 * sizeof(NationalRoute *));
  map->numOfCities = 0;
  return map;
}

void deleteMap(Map *map) {
  if (map == NULL) {
    return;
  }
  deleteTrie(map->trie);
  deleteNationalRoutes(map->nationalRoutes);
  free(map);
  map = NULL;
}

bool addCity(Map *map, const char *city) {
  return insertStr(map->trie, city, map->numOfCities++);
}

Trie *getCityPtr(Map *map, const char *city) {
  return getNodePtr(map->trie, city);
}

bool addRoad(Map *map, const char *city1, const char *city2, unsigned length,
             int builtYear) {
  if (map == NULL) {
    return false;
  }
  if (builtYear == 0 || length == 0) {
    return false;
  }
  if (!isValidCityName(city1) || !isValidCityName(city2)) {
    return false;
  }
  if (strcmp(city1, city2) == 0) {
    return false;
  }

  Trie *city1Ptr = getCityPtr(map, city1);
  Trie *city2Ptr = getCityPtr(map, city2);
  if (city1Ptr != NULL && city2Ptr != NULL) {
    if (isNeighbour(city1Ptr, city2Ptr)) {
      return false;
    }
  }

  bool res1 = true, res2 = true;
  if (city1Ptr == NULL) {
    res1 = addCity(map, city1);
  }
  if (city2Ptr == NULL) {
    res2 = addCity(map, city2);
  }

  if (!(res1 && res2)) {
    return false;
  }

  city1Ptr = getCityPtr(map, city1);
  city2Ptr = getCityPtr(map, city2);

  res1 = addRoadSection(city1Ptr, city2Ptr, length, builtYear);
  res2 = addRoadSection(city2Ptr, city1Ptr, length, builtYear);

  return res1 && res2;
}

bool repairRoad(Map *map, const char *city1, const char *city2,
                int repairYear) {
  if (map == NULL) {
    return false;
  }
  if (repairYear == 0) {
    return false;
  }
  if (strcmp(city1, city2) == 0) {
    return false;
  }
  if (!isValidCityName(city1) || !isValidCityName(city2)) {
    return false;
  }

  Trie *city1Ptr = getCityPtr(map, city1);
  Trie *city2Ptr = getCityPtr(map, city2);
  if (city1Ptr == NULL || city2Ptr == NULL) {
    return false;
  }
  if (!isNeighbour(city1Ptr, city2Ptr)) {
    return false;
  }

  int currentYear = getRepairYear(city1Ptr, city2Ptr);
  if (currentYear > repairYear) {
    return false;
  }

  repairRoadSection(city1Ptr, city2Ptr, repairYear);
  repairRoadSection(city2Ptr, city1Ptr, repairYear);
  return true;
}

void concatenateCityNameToResult(char **str, size_t *strLength,
                                 unsigned int *pos, Trie *city) {
  if (city->parent != NULL) {
    concatenateCityNameToResult(str, strLength, pos, city->parent);
  }
  if (city->character != '\0') {
    while (*pos + 1 >= *strLength) {
      *strLength *= 2;
      *str = realloc(*str, *strLength * sizeof(char));
      if (*str == NULL) {
        return;
      }
    }
    (*str)[(*pos)++] = city->character;
  }
}

void concatenateIntToResult(char **str, size_t *strLength, unsigned int *pos,
                            int val) {
  char *buffer = malloc(20 * sizeof(char));
  if (buffer == NULL) {
    return;
  }

  int n = sprintf(buffer, "%d;", val);
  while (*pos + n >= *strLength) {
    *strLength *= 2;
    *str = realloc(*str, *strLength * sizeof(char));
    if (*str == NULL) {
      free(buffer);
      return;
    }
  }

  for (int i = 0; i < n; i++) {
    (*str)[(*pos)++] = buffer[i];
  }
  free(buffer);
  buffer = NULL;
}

void concatenateUnsignedToResult(char **str, size_t *strLength,
                                 unsigned int *pos, unsigned val) {
  char *buffer = malloc(20 * sizeof(char));
  if (buffer == NULL) {
    return;
  }

  int n = sprintf(buffer, "%u;", val);
  while (*pos + n >= *strLength) {
    *strLength *= 2;
    *str = realloc(*str, *strLength * sizeof(char));
    if (*str == NULL) {
      free(buffer);
      buffer = NULL;
      return;
    }
  }

  for (int i = 0; i < n; i++) {
    (*str)[(*pos)++] = buffer[i];
  }
  free(buffer);
  buffer = NULL;
}

const char *getRouteDescription(Map *map, unsigned routeId) {
  char *result = NULL;

  size_t resultLength = INITIAL_LINE_LENGTH;
  result = (char *)malloc(resultLength * sizeof(char));

  memset(result, 0, resultLength * sizeof(char));
  if (result == NULL) {
    return result;
  }

  memset(result, 0, resultLength * sizeof(char));
  assert(result);

  if (map == NULL) {
    return result;
  }
  if (routeId > 999 || routeId == 0) {
    return result;
  }

  NationalRoute *nationalRoute = map->nationalRoutes[routeId];
  if (nationalRoute == NULL) {
    return result;
  }

  unsigned int pos = 0;
  concatenateIntToResult(&result, &resultLength, &pos, nationalRoute->id);
  if (result == NULL) {
    return result;
  }

  assert(nationalRoute->list);
  assert(nationalRoute->list->head);
  assert(nationalRoute->list->head->next);

  CitiesListNode *iter = nationalRoute->list->head->next;
  while (isValidCitiesListNode(iter) && isValidCitiesListNode(iter->next)) {
    concatenateCityNameToResult(&result, &resultLength, &pos, iter->elem.city);

    if (result == NULL) {
      return result;
    }
    if (pos + 1 >= resultLength) {
      resultLength *= 2;
      result = realloc(result, resultLength * sizeof(char));
    }
    if (result == NULL) {
      return result;
    }
    result[pos++] = ';';

    concatenateUnsignedToResult(
        &result, &resultLength, &pos,
        getRoadLength(iter->elem.city, iter->next->elem.city));

    if (result == NULL) {
      return result;
    }

    concatenateIntToResult(
        &result, &resultLength, &pos,
        getRepairYear(iter->elem.city, iter->next->elem.city));

    if (result == NULL) {
      return result;
    }
    iter = iter->next;
  }
  concatenateCityNameToResult(&result, &resultLength, &pos, iter->elem.city);

  if (result == NULL) {
    return result;
  }
  result[pos] = '\0';
  return result;
}

bool isCityInRoute(Trie *city, NationalRoute *route) {
  CitiesListNode *iter = route->list->head->next;
  while (isValidCitiesListNode(iter)) {
    if (city == iter->elem.city) {
      return true;
    }
    iter = iter->next;
  }
  return false;
}

SpfaResult *makeNewSpfaResult() {
  SpfaResult *spfaResult = (SpfaResult *)malloc(sizeof(SpfaResult));
  if (spfaResult == NULL) {
    return NULL;
  }
  spfaResult->dist = 0;
  spfaResult->isCorrect = false;
  spfaResult->prev = NULL;
  spfaResult->minYear = INF;
  return spfaResult;
}

void deleteResult(SpfaResult *result) {
  if (result == NULL) {
    return;
  }
  free(result->prev);
  free(result);
}

CitiesList *prevToCitiesList(Trie **prev, Trie *cityPtr) {
  CitiesList *list = makeNewCitiesList();
  if (list == NULL) {
    return NULL;
  }

  while (cityPtr != NULL) {
    addCitiesListNode(list, cityPtr);
    cityPtr = prev[cityPtr->id];
  }
  reverseCitiesList(list);
  return list;
}

SpfaResult *spfa(Map *m, unsigned routeId, Trie *startCity, Trie *finalCity) {
  SpfaResult *spfaResult = makeNewSpfaResult();
  CitiesList *queue = makeNewCitiesList();

  bool *vis = (bool *)malloc(m->numOfCities * sizeof(bool));
  int *minRepairYear = (int *)malloc(m->numOfCities * sizeof(int));
  unsigned *dist = (unsigned *)malloc(m->numOfCities * sizeof(unsigned));
  bool *isCorrect = (bool *)malloc(m->numOfCities * sizeof(bool));
  Trie **prev = (Trie **)malloc(m->numOfCities * sizeof(Trie *));

  if (spfaResult == NULL || queue == NULL || vis == NULL ||
      minRepairYear == NULL || dist == NULL || isCorrect == NULL ||
      prev == NULL) {
    deleteCitiesList(queue);
    free(vis);
    free(minRepairYear);
    free(dist);
    free(isCorrect);
    free(prev);
    deleteResult(spfaResult);
    return NULL;
  }

  for (int i = 0; i < m->numOfCities; i++) {
    vis[i] = false;
    minRepairYear[i] = INF;
    dist[i] = UNSIGNED_INF;
    isCorrect[i] = false;
    prev[i] = NULL;
  }

  dist[startCity->id] = 0;

  if (!addCitiesListNode(queue, startCity)) {
    deleteCitiesList(queue);
    free(vis);
    free(minRepairYear);
    free(dist);
    free(isCorrect);
    free(prev);
    deleteResult(spfaResult);
    return NULL;
  }

  vis[startCity->id] = true;
  isCorrect[startCity->id] = true;

  while (!isEmptyCitiesList(queue)) {
    CitiesListNode *city = getCitiesListFront(queue);
    Trie *currCity = city->elem.city;

    RoadsListNode *iter = currCity->roads->head->next;
    while (isValidRoadsListNode(iter)) {
      Trie *neighbour = iter->elem.city;

      bool flag = false;
      if (routeId != 0) {
        flag = isCityInRoute(neighbour, m->nationalRoutes[routeId]);
        if (flag && neighbour == finalCity && currCity != startCity) {
          flag = false;
        }
      }

      if (!flag) {
        if (dist[currCity->id] + iter->elem.length < dist[neighbour->id]) {
          dist[neighbour->id] = dist[currCity->id] + iter->elem.length;
          prev[neighbour->id] = currCity;
          isCorrect[neighbour->id] = isCorrect[currCity->id];

          if (iter->elem.builtYear < minRepairYear[currCity->id]) {
            minRepairYear[neighbour->id] = iter->elem.builtYear;
            isCorrect[neighbour->id] = true;
          } else {
            minRepairYear[neighbour->id] = minRepairYear[currCity->id];
          }

          if (!vis[neighbour->id]) {
            if (!addCitiesListNode(queue, neighbour)) {
              deleteCitiesList(queue);
              free(vis);
              free(minRepairYear);
              free(dist);
              free(isCorrect);
              free(prev);
              deleteResult(spfaResult);
              return NULL;
            }
            vis[neighbour->id] = true;
          }

        } else if (dist[currCity->id] + iter->elem.length ==
                   dist[neighbour->id]) {
          if (iter->elem.builtYear < minRepairYear[currCity->id]) {
            if (iter->elem.builtYear == minRepairYear[neighbour->id]) {
              isCorrect[neighbour->id] = false;
            } else if (iter->elem.builtYear > minRepairYear[neighbour->id]) {
              isCorrect[neighbour->id] = isCorrect[currCity->id];
              minRepairYear[neighbour->id] = iter->elem.builtYear;
              prev[neighbour->id] = currCity;
            }
          } else {
            if (minRepairYear[currCity->id] == minRepairYear[neighbour->id]) {
              isCorrect[neighbour->id] = false;
            } else if (minRepairYear[currCity->id] >
                       minRepairYear[neighbour->id]) {
              isCorrect[neighbour->id] = isCorrect[currCity->id];
              minRepairYear[neighbour->id] = minRepairYear[currCity->id];
              prev[neighbour->id] = currCity;
            }
          }
        }
      }
      iter = iter->next;
    }
    popFrontCitiesList(queue);
  }

  deleteCitiesList(queue);
  free(vis);

  spfaResult->dist = dist[finalCity->id];
  spfaResult->isCorrect = isCorrect[finalCity->id];
  spfaResult->prev = prev;
  spfaResult->minYear = minRepairYear[finalCity->id];

  free(minRepairYear);
  free(dist);
  free(isCorrect);

  return spfaResult;
}

int getMinimalResult(SpfaResult *fstResult, SpfaResult *sndResult) {
  if (fstResult->isCorrect && sndResult->isCorrect) {
    if (fstResult->dist < sndResult->dist) {
      return 1;
    } else if (fstResult->dist > sndResult->dist) {
      return 2;
    } else {
      if (fstResult->minYear > sndResult->minYear) {
        return 1;
      } else if (fstResult->minYear < sndResult->minYear) {
        return 2;
      } else {
        return 0;
      }
    }
  } else if (fstResult->isCorrect && !sndResult->isCorrect) {
    return 1;
  } else if (!fstResult->isCorrect && sndResult->isCorrect) {
    return 2;
  } else {
    return 0;
  }
}

void markRoadsWithRoute(CitiesList *list, unsigned routeId) {
  CitiesListNode *iter = list->head->next;
  while (isValidCitiesListNode(iter->next)) {
    Trie *city = iter->elem.city;
    Trie *neighbour = iter->next->elem.city;

    RoadsListNode *fstRoad = getRoadBetweenCities(city, neighbour);
    RoadsListNode *sndRoad = getRoadBetweenCities(neighbour, city);

    addRoutesListNode(fstRoad->elem.routes, routeId);
    addRoutesListNode(sndRoad->elem.routes, routeId);

    iter = iter->next;
  }
}

void undoMarkRoadsWithRoute(CitiesList *list, unsigned routeId) {
  assert(list);

  CitiesListNode *iter = list->head->next;
  assert(iter);
  assert(iter->next);

  while (isValidCitiesListNode(iter->next)) {
    Trie *city = iter->elem.city;
    Trie *neighbour = iter->next->elem.city;

    RoadsListNode *fstRoad = getRoadBetweenCities(city, neighbour);
    RoadsListNode *sndRoad = getRoadBetweenCities(neighbour, city);

    removeRoutesListNodeById(fstRoad->elem.routes, routeId);
    removeRoutesListNodeById(sndRoad->elem.routes, routeId);

    iter = iter->next;
  }
}

bool addRoute(Map *m, unsigned routeId, Trie *cityPtr, Trie **prev) {
  m->nationalRoutes[routeId] = newNationalRoute();
  if (m->nationalRoutes[routeId] == NULL) {
    return false;
  }

  NationalRoute *route = m->nationalRoutes[routeId];
  route->id = routeId;

  CitiesList *list = prevToCitiesList(prev, cityPtr);
  markRoadsWithRoute(list, routeId);
  addAfterRouteSection(route->list->head, list);

  return true;
}

bool isRouteinRoad(Trie *city, Trie *neighbour, unsigned routeId) {
  assert(city);
  assert(neighbour);

  RoadsListNode *road = getRoadBetweenCities(city, neighbour);
  assert(road->elem.routes);

  int cnt = 0;
  RoutesListNode *route = road->elem.routes->head->next;
  while (isValidRoutesListNode(route)) {
    if (route->elem.routeId == routeId) {
      cnt++;
    }
    route = route->next;
  }

  assert(cnt < 2);
  return cnt > 0;
}

bool checkRoute(Map *m, unsigned routeId) {
  assert(m);
  assert(m->nationalRoutes[routeId]);

  NationalRoute *route = m->nationalRoutes[routeId];

  CitiesListNode *iter = route->list->head->next;
  while (isValidCitiesListNode(iter->next)) {
    assert(isNeighbour(iter->elem.city, iter->next->elem.city));
    assert(isNeighbour(iter->next->elem.city, iter->elem.city));

    assert(isRouteinRoad(iter->elem.city, iter->next->elem.city, routeId));
    assert(isRouteinRoad(iter->next->elem.city, iter->elem.city, routeId));

    iter = iter->next;
  }

  return true;
}

bool newRoute(Map *map, unsigned routeId, const char *city1,
              const char *city2) {
  if (map == NULL) {
    return NULL;
  }
  if (routeId == 0 || routeId > 999) {
    return false;
  }
  if (map->nationalRoutes[routeId] != NULL) {
    return false;
  }
  if (!isValidCityName(city1) || !isValidCityName(city2)) {
    return false;
  }
  if (strcmp(city1, city2) == 0) {
    return false;
  }
  if (getCityPtr(map, city1) == NULL || getCityPtr(map, city2) == NULL) {
    return false;
  }

  Trie *startCity = getCityPtr(map, city1);
  Trie *finalCity = getCityPtr(map, city2);

  SpfaResult *result = spfa(map, 0, startCity, finalCity);
  if (result == NULL) {
    return false;
  }

  if (!result->isCorrect) {
    deleteResult(result);
    return false;
  }

  bool res = addRoute(map, routeId, finalCity, result->prev);

  assert(checkRoute(map, routeId));

  deleteResult(result);

  return res;
}

bool removeRoute(Map *map, unsigned routeId) {
  if (map == NULL) {
    return false;
  }
  if (routeId == 0 || routeId > 999) {
    return false;
  }
  if (map->nationalRoutes[routeId] == NULL) {
    return false;
  }

  undoMarkRoadsWithRoute(map->nationalRoutes[routeId]->list, routeId);

  deleteNationalRoute(map->nationalRoutes[routeId]);
  map->nationalRoutes[routeId] = NULL;

  return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
  if (map == NULL) {
    return false;
  }
  if (routeId == 0 || routeId > 999) {
    return false;
  }
  if (!isValidCityName(city)) {
    return false;
  }
  if (map->nationalRoutes[routeId] == NULL) {
    return false;
  }

  Trie *cityPtr = getCityPtr(map, city);
  if (cityPtr == NULL) {
    return false;
  }
  if (isCityInRoute(cityPtr, map->nationalRoutes[routeId])) {
    return false;
  }

  Trie *fstStartCity =
      map->nationalRoutes[routeId]->list->tail->prev->elem.city;
  Trie *fstFinalCity = getCityPtr(map, city);

  SpfaResult *fstResult = spfa(map, routeId, fstStartCity, fstFinalCity);
  if (fstResult == NULL) {
    return false;
  }

  Trie *sndStartCity = getCityPtr(map, city);
  Trie *sndFinalCity =
      map->nationalRoutes[routeId]->list->head->next->elem.city;

  SpfaResult *sndResult = spfa(map, routeId, sndStartCity, sndFinalCity);
  if (sndResult == NULL) {
    return false;
  }

  int resultCase = getMinimalResult(fstResult, sndResult);
  if (resultCase == 0) {
    deleteResult(fstResult);
    deleteResult(sndResult);
    return false;
  } else if (resultCase == 1) {
    CitiesList *list = prevToCitiesList(fstResult->prev, fstFinalCity);

    markRoadsWithRoute(list, routeId);

    popFrontCitiesList(list);

    addAfterRouteSection(map->nationalRoutes[routeId]->list->tail->prev, list);
  } else if (resultCase == 2) {
    CitiesList *list = prevToCitiesList(sndResult->prev, sndFinalCity);

    markRoadsWithRoute(list, routeId);

    popBackCitiesList(list);

    addAfterRouteSection(map->nationalRoutes[routeId]->list->head, list);
  } else {
    assert(false);
  }

  assert(checkRoute(map, routeId));

  deleteResult(fstResult);
  deleteResult(sndResult);

  return true;
}

bool isPossibleToReplaceRoadInRoute(Map *m, Trie *city1, Trie *city2,
                                    unsigned routeId) {
  assert(m != NULL);
  assert(m->nationalRoutes[routeId] != NULL);
  assert(city1 != NULL);
  assert(city2 != NULL);

  NationalRoute *route = m->nationalRoutes[routeId];

  CitiesListNode *iter = route->list->head->next;

  assert(iter);
  assert(iter->next);

  while (isValidCitiesListNode(iter->next)) {
    Trie *currCity = iter->elem.city;
    Trie *nextCity = iter->next->elem.city;

    if ((currCity == city1 && nextCity == city2) ||
        (nextCity == city1 && currCity == city2)) {
      SpfaResult *result = spfa(m, routeId, currCity, nextCity);

      if (result == NULL) {
        return false;
      }

      bool res = result->isCorrect;
      deleteResult(result);
      return res;
    }
    iter = iter->next;
  }

  assert(false);
  return false;
}

bool replaceRoadInRoute(Map *m, Trie *city1, Trie *city2, unsigned routeId) {
  assert(m);
  assert(m->nationalRoutes[routeId]);
  assert(city1);
  assert(city2);

  NationalRoute *route = m->nationalRoutes[routeId];

  CitiesListNode *iter = route->list->head->next;

  assert(iter);
  assert(iter->next);

  while (isValidCitiesListNode(iter->next)) {
    Trie *currCity = iter->elem.city;
    Trie *nextCity = iter->next->elem.city;

    if ((currCity == city1 && nextCity == city2) ||
        (nextCity == city1 && currCity == city2)) {
      SpfaResult *result = spfa(m, routeId, currCity, nextCity);

      if (result == NULL) {
        return false;
      }

      CitiesList *list = prevToCitiesList(result->prev, nextCity);

      markRoadsWithRoute(list, routeId);

      assert(checkRoute(m, routeId));

      popFrontCitiesList(list);
      popBackCitiesList(list);

      addAfterRouteSection(iter, list);

      deleteResult(result);

      return true;
    }
    iter = iter->next;
  }

  assert(false);
  return false;
}

void removeRoadFromCity(Trie *city, Trie *neighbour) {
  assert(city);
  assert(neighbour);
  assert(isNeighbour(city, neighbour));

  RoadsListNode *road = getRoadBetweenCities(city, neighbour);

  assert(road);
  removeRoadsListNode(road);
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
  if (map == NULL) {
    return false;
  }
  if (!isValidCityName(city1) || !isValidCityName(city2)) {
    return false;
  }
  if (strcmp(city1, city2) == 0) {
    return false;
  }
  if (getCityPtr(map, city1) == NULL || getCityPtr(map, city2) == NULL) {
    return false;
  }

  Trie *city1Ptr = getCityPtr(map, city1);
  Trie *city2Ptr = getCityPtr(map, city2);
  if (city1Ptr != NULL && city2Ptr != NULL) {
    if (!isNeighbour(city1Ptr, city2Ptr)) {
      return false;
    }
  } else {
    return false;
  }

  RoadsListNode *road = getRoadBetweenCities(city1Ptr, city2Ptr);
  assert(road);

  RoutesListNode *route = road->elem.routes->head->next;
  assert(route);

  while (isValidRoutesListNode(route)) {
    bool result = isPossibleToReplaceRoadInRoute(map, city1Ptr, city2Ptr,
                                                 route->elem.routeId);
    if (result == false) {
      return false;
    }
    route = route->next;
  }

  route = road->elem.routes->head->next;
  assert(route);

  while (isValidRoutesListNode(route)) {
    if (!replaceRoadInRoute(map, city1Ptr, city2Ptr, route->elem.routeId)) {
      return false;
    }
    route = route->next;
  }

  city1Ptr = getCityPtr(map, city1);
  city2Ptr = getCityPtr(map, city2);

  removeRoadFromCity(city1Ptr, city2Ptr);
  removeRoadFromCity(city2Ptr, city1Ptr);

  return true;
}

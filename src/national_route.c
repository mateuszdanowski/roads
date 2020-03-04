#include "national_route.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

NationalRoute *newNationalRoute() {
  NationalRoute *nationalRoute = (NationalRoute *)malloc(sizeof(NationalRoute));
  if (nationalRoute == NULL) {
    return NULL;
  }

  nationalRoute->id = 0;
  nationalRoute->list = makeNewCitiesList();
  if (nationalRoute->list == NULL) {
    free(nationalRoute);
    return NULL;
  }

  return nationalRoute;
}

void deleteNationalRoute(NationalRoute *nationalRoute) {
  if (nationalRoute == NULL) {
    return;
  }
  if (nationalRoute->list != NULL) {
    deleteCitiesList(nationalRoute->list);
  }
  free(nationalRoute);
}

void deleteNationalRoutes(NationalRoute **nationalRoutes) {
  if (nationalRoutes == NULL) {
    return;
  }
  for (int i = 0; i < 1000; i++) {
    deleteNationalRoute(nationalRoutes[i]);
  }
  free(nationalRoutes);
}

bool addNationalRouteSection(NationalRoute *nationalRoute, void *city) {
  return addCitiesListNode(nationalRoute->list, city);
}

void addAfterRouteSection(CitiesListNode *node, CitiesList *list) {
  if (isEmptyCitiesList(list)) {
    deleteCitiesList(list);
    return;
  }

  node->next->prev = list->tail->prev;
  list->tail->prev->next = node->next;

  node->next = list->head->next;
  list->head->next->prev = node;

  free(list->head);
  free(list->tail);
  free(list);
}

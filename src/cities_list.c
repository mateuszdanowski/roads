#include "cities_list.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

CitiesListElement makeNewCitiesListElem(void *city) {
  CitiesListElement elem;
  elem.city = city;
  return elem;
}

CitiesListNode *makeNewCitiesListNode(CitiesListNode *prev,
                                      CitiesListElement elem,
                                      CitiesListNode *next) {
  CitiesListNode *node = (CitiesListNode *)malloc(sizeof(CitiesListNode));
  if (node == NULL) {
    return NULL;
  }

  node->prev = prev;
  node->elem = elem;
  node->next = next;

  return node;
}

CitiesList *makeNewCitiesList() {
  CitiesList *list = (CitiesList *)malloc(sizeof(CitiesList));
  if (list == NULL) {
    return NULL;
  }

  list->head = makeNewCitiesListNode(NULL, makeNewCitiesListElem(NULL), NULL);

  if (list->head == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->tail =
      makeNewCitiesListNode(list->head, makeNewCitiesListElem(NULL), NULL);

  if (list->tail == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->head->next = list->tail;

  return list;
}

bool isValidCitiesListNode(CitiesListNode *node) {
  return node != NULL && node->next != NULL && node->prev != NULL;
}

void removeCitiesListNode(CitiesListNode *node) {
  assert(node);
  assert(node->next);
  assert(node->prev);

  node->next->prev = node->prev;
  node->prev->next = node->next;

  free(node);
}

void popBackCitiesList(CitiesList *list) {
  assert(list);
  assert(list->tail);

  if (isValidCitiesListNode(list->tail->prev)) {
    removeCitiesListNode(list->tail->prev);
  }
}

void popFrontCitiesList(CitiesList *list) {
  assert(list);
  assert(list->head);

  if (isValidCitiesListNode(list->head->next)) {
    removeCitiesListNode(list->head->next);
  }
}

void deleteCitiesList(CitiesList *list) {
  if (list == NULL) {
    return;
  }

  while (isValidCitiesListNode(list->tail->prev)) {
    popBackCitiesList(list);
  }

  free(list->head);
  free(list->tail);
  free(list);
  list = NULL;
}

bool addCitiesListNode(CitiesList *list, void *city) {
  CitiesListNode *node = makeNewCitiesListNode(
      list->tail->prev, makeNewCitiesListElem(city), list->tail);
  if (node == NULL) {
    return false;
  }

  list->tail->prev->next = node;
  list->tail->prev = node;

  return true;
}

bool isEmptyCitiesList(CitiesList *list) {
  assert(list);
  return !isValidCitiesListNode(list->head->next);
}

CitiesListNode *getCitiesListFront(CitiesList *list) {
  assert(list);
  assert(list->head);
  assert(list->head->next);
  return list->head->next;
}

void reverseCitiesList(CitiesList *list) {
  if (list == NULL) {
    return;
  }

  assert(list->head);

  CitiesListNode *iter = list->head;

  while (iter != NULL) {
    CitiesListNode *temp = iter->next;
    iter->next = iter->prev;
    iter->prev = temp;

    iter = temp;
  }

  CitiesListNode *temp = list->head;
  list->head = list->tail;
  list->tail = temp;
}

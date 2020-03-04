#include "roads_list.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

RoadsListElement newRoadsListElem(void *city, unsigned length, int builtYear) {
  RoadsListElement elem;
  elem.city = city;
  elem.length = length;
  elem.builtYear = builtYear;
  elem.routes = NULL;
  return elem;
}

RoadsListNode *newRoadsListNode(RoadsListNode *prev, RoadsListElement elem,
                                RoadsListNode *next) {
  RoadsListNode *node = (RoadsListNode *)malloc(sizeof(RoadsListNode));
  if (node == NULL) {
    return NULL;
  }

  node->prev = prev;
  node->elem = elem;
  node->next = next;
  return node;
}

RoadsList *newRoadsList() {
  RoadsList *list = (RoadsList *)malloc(sizeof(RoadsList));
  if (list == NULL) {
    return NULL;
  }

  list->head = newRoadsListNode(NULL, newRoadsListElem(NULL, 0, 0), NULL);
  if (list->head == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->tail = newRoadsListNode(list->head, newRoadsListElem(NULL, 0, 0), NULL);
  if (list->tail == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->head->next = list->tail;
  return list;
}

bool isValidRoadsListNode(RoadsListNode *node) {
  return node != NULL && node->next != NULL && node->prev != NULL;
}

void removeRoadsListNode(RoadsListNode *node) {
  assert(node != NULL);
  assert(node->next != NULL);
  assert(node->prev != NULL);
  assert(node->elem.routes);

  node->next->prev = node->prev;
  node->prev->next = node->next;

  deleteRoutesList(node->elem.routes);
  free(node);
  node = NULL;
}

void popBackRoadsList(RoadsList *list) {
  assert(list != NULL);
  assert(list->tail != NULL);

  if (isValidRoadsListNode(list->tail->prev)) {
    removeRoadsListNode(list->tail->prev);
  }
}

void deleteRoadsList(RoadsList *list) {
  if (list == NULL) {
    return;
  }
  while (isValidRoadsListNode(list->tail->prev)) {
    popBackRoadsList(list);
  }
  free(list->head);
  free(list->tail);
  free(list);
  list = NULL;
}

bool addRoadsListNode(RoadsList *list, void *city, unsigned length,
                      int builtYear) {
  RoadsListNode *node = newRoadsListNode(
      list->tail->prev, newRoadsListElem(city, length, builtYear), list->tail);
  if (node == NULL) {
    return false;
  }

  node->elem.routes = newRoutesList();
  if (node->elem.routes == NULL) {
    free(node);
    node = NULL;
    return false;
  }

  list->tail->prev->next = node;
  list->tail->prev = node;
  return true;
}

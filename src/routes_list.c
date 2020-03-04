#include "routes_list.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

RoutesListElement newRoutesListElem(unsigned routeId) {
  RoutesListElement elem;
  elem.routeId = routeId;
  return elem;
}

RoutesListNode *newRoutesListNode(RoutesListNode *prev, RoutesListElement elem,
                                  RoutesListNode *next) {
  RoutesListNode *node = (RoutesListNode *)malloc(sizeof(RoutesListNode));
  if (node == NULL) {
    return NULL;
  }
  node->prev = prev;
  node->elem = elem;
  node->next = next;
  return node;
}

RoutesList *newRoutesList() {
  RoutesList *list = (RoutesList *)malloc(sizeof(RoutesList));
  if (list == NULL) {
    return NULL;
  }

  list->head = newRoutesListNode(NULL, newRoutesListElem(0), NULL);
  if (list->head == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->tail = newRoutesListNode(list->head, newRoutesListElem(0), NULL);
  if (list->tail == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->head->next = list->tail;
  return list;
}

bool isValidRoutesListNode(RoutesListNode *node) {
  return node != NULL && node->next != NULL && node->prev != NULL;
}

void removeRoutesListNode(RoutesListNode *node) {
  assert(node != NULL);
  assert(node->next != NULL);
  assert(node->prev != NULL);

  node->next->prev = node->prev;
  node->prev->next = node->next;

  free(node);
  node = NULL;
}
void removeRoutesListNodeById(RoutesList *list, unsigned routeId) {
  assert(list);

  RoutesListNode *iter = list->head->next;
  assert(iter);

  while (isValidRoutesListNode(iter)) {
    if (iter->elem.routeId == routeId) {
      removeRoutesListNode(iter);
      return;
    }
    iter = iter->next;
  }
  assert(false);
}

void popBackRoutesList(RoutesList *list) {
  assert(list != NULL);
  assert(list->tail != NULL);

  if (isValidRoutesListNode(list->tail->prev)) {
    removeRoutesListNode(list->tail->prev);
  }
}

void deleteRoutesList(RoutesList *list) {
  if (list == NULL) {
    return;
  }
  while (isValidRoutesListNode(list->tail->prev)) {
    popBackRoutesList(list);
  }
  free(list->head);
  free(list->tail);
  free(list);
  list = NULL;
}

bool addRoutesListNode(RoutesList *list, unsigned route) {
  RoutesListNode *node =
      newRoutesListNode(list->tail->prev, newRoutesListElem(route), list->tail);
  if (node == NULL) {
    return false;
  }

  list->tail->prev->next = node;
  list->tail->prev = node;
  return true;
}

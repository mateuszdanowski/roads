#include "children_list.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ChildrenListElement newChildrenListElem(void *child, char character) {
  ChildrenListElement elem;
  elem.child = child;
  elem.character = character;

  return elem;
}

ChildrenListNode *newChildrenListNode(ChildrenListNode *prev,
                                      ChildrenListElement elem,
                                      ChildrenListNode *next) {
  ChildrenListNode *node = (ChildrenListNode *)malloc(sizeof(ChildrenListNode));

  if (node == NULL) {
    return NULL;
  }

  node->prev = prev;
  node->elem = elem;
  node->next = next;

  return node;
}

ChildrenList *newChildrenList() {
  ChildrenList *list = (ChildrenList *)malloc(sizeof(ChildrenList));

  if (list == NULL) {
    return NULL;
  }

  list->head = newChildrenListNode(NULL, newChildrenListElem(NULL, '\0'), NULL);

  if (list->head == NULL) {
    free(list);
    list = NULL;
    return NULL;
  }

  list->tail =
      newChildrenListNode(list->head, newChildrenListElem(NULL, '\0'), NULL);

  if (list->tail == NULL) {
    free(list->head);
    free(list);
    list = NULL;
    return NULL;
  }

  list->head->next = list->tail;

  return list;
}

bool isValidChildrenListNode(ChildrenListNode *node) {
  return node != NULL && node->next != NULL && node->prev != NULL;
}

void removeChildrenListNode(ChildrenListNode *node) {
  assert(node);
  assert(node->next);
  assert(node->prev);

  node->next->prev = node->prev;
  node->prev->next = node->next;

  free(node);
  node = NULL;
}

void popBackChildrenList(ChildrenList *list) {
  assert(list);
  assert(list->tail);

  if (isValidChildrenListNode(list->tail->prev)) {
    removeChildrenListNode(list->tail->prev);
  }
}

void deleteChildrenList(ChildrenList *list) {
  if (list == NULL) {
    return;
  }

  while (isValidChildrenListNode(list->tail->prev)) {
    popBackChildrenList(list);
  }

  free(list->head);
  free(list->tail);
  free(list);
  list = NULL;
}

bool addChildrenListNode(ChildrenList *list, char character) {
  ChildrenListNode *node = newChildrenListNode(
      list->tail->prev, newChildrenListElem(NULL, character), list->tail);

  if (node == NULL) {
    return false;
  }

  list->tail->prev->next = node;
  list->tail->prev = node;

  return true;
}

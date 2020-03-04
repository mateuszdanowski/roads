#include "trie.h"

#include <assert.h>
#include <stdlib.h>

#include "children_list.h"
#include "roads_list.h"

Trie *newTrieNode() {
  Trie *node = (Trie *)malloc(sizeof(Trie));
  if (node == NULL) {
    return NULL;
  }

  node->children = newChildrenList();
  node->roads = newRoadsList();
  node->isLeaf = false;
  node->id = -1;

  node->character = '\0';
  node->parent = NULL;

  if (node->children == NULL || node->roads == NULL) {
    deleteChildrenList(node->children);
    deleteRoadsList(node->roads);
    free(node->children);
    free(node->roads);
    free(node);
    return NULL;
  }

  return node;
}

void deleteTrie(Trie *node) {
  if (node == NULL) {
    return;
  }

  if (node->children != NULL) {
    ChildrenListNode *iter = node->children->head->next;
    while (isValidChildrenListNode(iter)) {
      deleteTrie(iter->elem.child);
      iter = iter->next;
    }
    deleteChildrenList(node->children);
  }
  if (node->roads != NULL) {
    deleteRoadsList(node->roads);
  }
  free(node);
}

Trie *getNodePtr(Trie *root, const char *city) {
  Trie *curr = root;

  while (*city) {
    Trie *next = NULL;
    ChildrenListNode *iter = curr->children->head->next;
    while (isValidChildrenListNode(iter)) {
      if (iter->elem.character == *city) {
        next = iter->elem.child;
        break;
      }
      iter = iter->next;
    }
    if (next == NULL) {
      return NULL;
    }
    curr = next;
    city++;
  }

  if (!curr->isLeaf) {
    return NULL;
  }
  return curr;
}

bool insertStr(Trie *root, const char *city, int id) {
  Trie *curr = root;

  while (*city) {
    Trie *next = NULL;
    ChildrenListNode *iter = curr->children->head->next;
    while (isValidChildrenListNode(iter)) {
      if (iter->elem.character == *city) {
        next = iter->elem.child;
        break;
      }
      iter = iter->next;
    }

    if (next == NULL) {
      addChildrenListNode(curr->children, *city);
      curr->children->tail->prev->elem.child = newTrieNode();

      if (curr->children->tail->prev->elem.child == NULL) {
        return false;
      }

      curr->children->tail->prev->elem.child->character = *city;
      curr->children->tail->prev->elem.child->parent = curr;
      next = curr->children->tail->prev->elem.child;
    }
    curr = next;
    city++;
  }
  curr->isLeaf = true;
  curr->id = id;
  return true;
}

bool isNeighbour(Trie *city, Trie *neighbour) {
  if (city == NULL || neighbour == NULL) {
    return false;
  }

  RoadsListNode *iter = city->roads->head->next;
  while (isValidRoadsListNode(iter)) {
    if (iter->elem.city == neighbour) {
      return true;
    }
    iter = iter->next;
  }
  return false;
}

bool addRoadSection(Trie *city1, Trie *city2, unsigned length, int builtYear) {
  return addRoadsListNode(city1->roads, city2, length, builtYear);
}

unsigned getRoadLength(Trie *city, Trie *neighbour) {
  assert(isNeighbour(city, neighbour));

  RoadsListNode *iter = city->roads->head->next;
  while (isValidRoadsListNode(iter)) {
    if (iter->elem.city == neighbour) {
      return iter->elem.length;
    }
    iter = iter->next;
  }

  assert(false);
  return 0;
}

int getRepairYear(Trie *city, Trie *neighbour) {
  assert(isNeighbour(city, neighbour));

  RoadsListNode *iter = city->roads->head->next;
  while (isValidRoadsListNode(iter)) {
    if (iter->elem.city == neighbour) {
      return iter->elem.builtYear;
    }
    iter = iter->next;
  }

  assert(false);
  return 0;
}

void repairRoadSection(Trie *city, Trie *neighbour, int repairYear) {
  RoadsListNode *iter = city->roads->head->next;
  while (isValidRoadsListNode(iter)) {
    if (iter->elem.city == neighbour) {
      iter->elem.builtYear = repairYear;
      return;
    }
    iter = iter->next;
  }
  assert(false);
}

RoadsListNode *getRoadBetweenCities(Trie *city, Trie *neighbour) {
  assert(city);
  assert(neighbour);
  assert(isNeighbour(city, neighbour));

  RoadsListNode *iter = city->roads->head->next;
  assert(iter);

  while (isValidRoadsListNode(iter)) {
    if (iter->elem.city == neighbour) {
      return iter;
    }
    iter = iter->next;
  }

  assert(false);
  return NULL;
}

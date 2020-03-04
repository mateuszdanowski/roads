#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "map.h"
#include "national_route.h"
#include "roads_list.h"
#include "strings.h"

// Wywoływana przed zakończeniem programu, zwalnia całą pamięć
void clean(char **line, Map **m) {
  free(*line);
  deleteMap(*m);
}

// Wywoływana na początek programu, alokuje potrzebną pamięć
void initialize(char **line, size_t *lineLength, Map **m) {
  *lineLength = INITIAL_LINE_LENGTH;
  *line = malloc(*lineLength * sizeof(char));
  *m = newMap();
  if (*line == NULL || *m == NULL) {
    clean(line, m);
    exit(0);
  }
}

// Wczytuje linię ze standardowego wejścia
int readLine(char **line, size_t *lineLength, Map **m) {
  int character;
  unsigned int pos = 0;

  while ((character = fgetc(stdin)) != '\n' && character != EOF) {
    if (pos + 1 >= *lineLength) {
      *lineLength *= 2;
      *line = realloc(*line, *lineLength * sizeof(char));
      if (*line == NULL) {
        clean(line, m);
        exit(0);
      }
    }

    // Jeśli na wejściu pojawi się '\0', zmieniam go na inny niepoprawny
    // znak po to, aby nie sygnalizował on końca stringa
    if (character == '\0') {
      character = (char)1;
    }

    (*line)[pos++] = (char)character;
  }

  // zastępuję ostatni znak w linii znakiem końca stringa
  (*line)[pos] = '\0';

  return character != EOF;
}

// Wykonuje operacje dla danej linii
void processLine(char *line, int lineNumber, Map *m) {
  size_t numOfCharacters = strlen(line) + 1;

  // komentarze lub pusty wiersz
  if (numOfCharacters == 1 || line[0] == '#') {
    return;
  }

  // wiersz zaczyna się średnikiem
  if (line[0] == ';') {
    fprintf(stderr, "ERROR %d\n", lineNumber);
    return;
  }

  char *command = strtok(line, ";\n");  // typ operacji

  unsigned routeId = strGetRouteId(command);
  if (0 < routeId && routeId < 1000) {
    size_t argsLength = INITIAL_LINE_LENGTH;
    char **args = malloc(argsLength * sizeof(char *));
    if (args == NULL) {
      clean(&line, &m);
      exit(0);
    }

    unsigned int pos = 0;

    char *arg;
    size_t expectedLength = strlen(command) + 1;

    while ((arg = strtok(NULL, ";\n")) != NULL) {
      expectedLength += strlen(arg) + 1;

      if (pos + 1 >= argsLength) {
        argsLength *= 2;
        args = realloc(args, sizeof(char *) * argsLength);

        if (args == NULL) {
          fprintf(stderr, "ERROR %d\n", lineNumber);
          return;
        }
      }

      args[pos++] = arg;
    }

    if (numOfCharacters != expectedLength) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
      free(args);
      return;
    }

    if (pos < 4 || (pos - 1) % 3 != 0) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
      free(args);
      return;
    }

    if (m->nationalRoutes[routeId] != NULL) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
      free(args);
      return;
    }

    // sprawdzanie poprawnosci nazw miast
    for (unsigned int i = 0; i < pos; i += 3) {
      if (!isValidCityName(args[i])) {
        fprintf(stderr, "ERROR %d\n", lineNumber);
        free(args);
        return;
      }
    }

    // sprawdzanie cyklu
    for (unsigned int i = 0; i < pos; i += 3) {
      for (unsigned j = 0; j < i; j += 3) {
        if (strcmp(args[i], args[j]) == 0) {
          fprintf(stderr, "ERROR %d\n", lineNumber);
          free(args);
          return;
        }
      }
    }

    // sprawdzanie poprawności dlugości i roku budowy
    for (unsigned int i = 0; i < pos - 3; i += 3) {
      unsigned length = strGetLength(args[i + 1]);
      int year = strGetYear(args[i + 2]);

      if (length == 0 || year == 0) {
        fprintf(stderr, "ERROR %d\n", lineNumber);
        free(args);
        return;
      }

      Trie *city1Ptr = getCityPtr(m, args[i]);
      Trie *city2Ptr = getCityPtr(m, args[i + 3]);
      if (isNeighbour(city1Ptr, city2Ptr)) {
        if (length != getRoadLength(city1Ptr, city2Ptr)) {
          fprintf(stderr, "ERROR %d\n", lineNumber);
          free(args);
          return;
        }
        if (year < getRepairYear(city1Ptr, city2Ptr)) {
          fprintf(stderr, "ERROR %d\n", lineNumber);
          free(args);
          return;
        }
      }
    }

    m->nationalRoutes[routeId] = newNationalRoute();
    if (m->nationalRoutes[routeId] == NULL) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
      free(args);
      return;
    }

    m->nationalRoutes[routeId]->id = routeId;
    for (unsigned int i = 0; i < pos - 3; i += 3) {
      unsigned length = strGetLength(args[i + 1]);
      int year = strGetYear(args[i + 2]);

      Trie *city1Ptr = getCityPtr(m, args[i]);
      Trie *city2Ptr = getCityPtr(m, args[i + 3]);
      if (isNeighbour(city1Ptr, city2Ptr)) {
        repairRoadSection(city1Ptr, city2Ptr, year);
        repairRoadSection(city2Ptr, city1Ptr, year);
      } else {
        addRoad(m, args[i], args[i + 3], length, year);
      }

      city1Ptr = getCityPtr(m, args[i]);
      city2Ptr = getCityPtr(m, args[i + 3]);
      assert(getRoadBetweenCities(city1Ptr, city2Ptr));

      addNationalRouteSection(m->nationalRoutes[routeId],
                              getCityPtr(m, args[i]));

      RoadsListNode *fstRoad = getRoadBetweenCities(city1Ptr, city2Ptr);
      RoadsListNode *sndRoad = getRoadBetweenCities(city2Ptr, city1Ptr);

      addRoutesListNode(fstRoad->elem.routes, routeId);
      addRoutesListNode(sndRoad->elem.routes, routeId);
    }

    addNationalRouteSection(m->nationalRoutes[routeId],
                            getCityPtr(m, args[pos - 1]));

    assert(checkRoute(m, routeId));
    free(args);
  } else {
    size_t commandLength = 0, arg1Length = 0, arg2Length = 0, arg3Length = 0,
           arg4Length = 0;

    char *arg1 = strtok(NULL, ";\n");  // pierwszy argument
    char *arg2 = strtok(NULL, ";\n");  // drugi argument
    char *arg3 = strtok(NULL, ";\n");  // trzeci argument
    char *arg4 = strtok(NULL, ";\n");  // czwarty argument
    char *rest = strtok(NULL, ";\n");  // jeśli jest coś jeszcze

    if (command != NULL) {
      commandLength = strlen(command) + 1;
    }
    if (arg1 != NULL) {
      arg1Length = strlen(arg1) + 1;
    }
    if (arg2 != NULL) {
      arg2Length = strlen(arg2) + 1;
    }
    if (arg3 != NULL) {
      arg3Length = strlen(arg3) + 1;
    }
    if (arg4 != NULL) {
      arg4Length = strlen(arg4) + 1;
    }

    size_t expectedLength =
        commandLength + arg1Length + arg2Length + arg3Length + arg4Length;

    if (rest != NULL || numOfCharacters != expectedLength) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
      return;
    }

    bool flag = true;
    if (strcmp(command, "addRoad") == 0) {
      unsigned length = strGetLength(arg3);
      int builtYear = strGetYear(arg4);
      flag = (!addRoad(m, arg1, arg2, length, builtYear));
    } else if (strcmp(command, "removeRoad") == 0) {
      flag = (arg3 != NULL);
      if (!flag) {
        flag = (!removeRoad(m, arg1, arg2));
      }
    } else if (strcmp(command, "repairRoad") == 0) {
      int repairYear = strGetYear(arg3);
      flag = (repairYear == 0 || arg4 != NULL);
      if (!flag) {
        flag = (!repairRoad(m, arg1, arg2, repairYear));
      }
    } else if (strcmp(command, "newRoute") == 0) {
      routeId = strGetRouteId(arg1);
      flag = (routeId == 0 || routeId > 999 || arg4 != NULL);
      if (!flag) {
        flag = (!newRoute(m, routeId, arg2, arg3));
      }
    } else if (strcmp(command, "removeRoute") == 0) {
      routeId = strGetRouteId(arg1);
      flag = (arg2 != NULL);
      if (!flag) {
        flag = (!removeRoute(m, routeId));
      }
    } else if (strcmp(command, "extendRoute") == 0) {
      routeId = strGetRouteId(arg1);
      flag = (arg3 != NULL);
      if (!flag) {
        flag = (!extendRoute(m, routeId, arg2));
      }
    } else if (strcmp(command, "getRouteDescription") == 0) {
      if (!strIsValidNumber(arg1) || arg1[0] == '-') {
        fprintf(stderr, "ERROR %d\n", lineNumber);
        return;
      }

      long long num = strtoll(arg1, NULL, 10);
      if (errno == ERANGE || num > MAX_ROUTE_ID) {
        errno = 0;
        fprintf(stderr, "ERROR %d\n", lineNumber);
        return;
      }

      routeId = strGetRouteId(arg1);

      flag = (arg2 != NULL || arg3 != NULL || arg4 != NULL);
      if (!flag) {
        const char *routeDescription = getRouteDescription(m, routeId);

        flag = (routeDescription == NULL);
        if (!flag) {
          fprintf(stdout, "%s\n", routeDescription);
          free((void *)routeDescription);
        }
      }
    }

    if (flag) {
      fprintf(stderr, "ERROR %d\n", lineNumber);
    }
  }
}

int main() {
  size_t lineLength;
  char *line;

  Map *m = newMap();
  initialize(&line, &lineLength, &m);

  int lineNumber = 1;
  while (readLine(&line, &lineLength, &m)) {
    processLine(line, lineNumber, m);
    lineNumber++;
  }

  clean(&line, &m);
  return 0;
}

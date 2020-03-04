#!/bin/bash

FILE=$1

if [ $# -le 1 ]; then
  echo "Potrzebne są dwa przynajmniej dwa argumenty: nazwa pliku i numery szukanych dróg krajowych"
  exit 1
fi

if ! [ -f "${FILE}" ]; then
  echo "Niepoprawny plik"
  exit 1
fi

NUM=0

for f in "$@"; do

  ((NUM += 1))

  if [ $NUM -eq 1 ]; then
    continue
  fi

  if ! [[ "${f//[0-9]/}" == "" && "$f" -gt 0 && "$f" -lt 1000 ]]; then
    echo "Niepoprawna wartosc $f"
    exit 1
  fi
done

NUM=0

for f in "$@"; do

  ((NUM += 1))

  if [ $NUM -eq 1 ]; then
    continue
  fi

  grep "^$f;" $FILE |
    awk -F ";" '{sum = 0; for (i = 3; $i; i += 3) sum += $i; print '$f' ";" sum}'
done

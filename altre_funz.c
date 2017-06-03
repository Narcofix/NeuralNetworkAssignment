#define _CRT_RAND_S
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Neuroprototipi.h"

char *narcostrstr(register char *buf, register char *sub,
                  register char *limit) {
  register char *bp;
  register char *sp;
  // caso stringa sub vuota
  if (!*sub) return buf;
  // caso stringa limit vuota
  if (limit == NULL) return strstr(buf, sub);
  // caso stringa sub piena
  while (*buf) {
    bp = buf;
    sp = sub;
    do {
      if (!*sp) return buf;
      if (bp == limit) return (char *)NULL;
    } while (*bp++ == *sp++);
    buf += 1;
  }
  return (char *)NULL;
}
tipopesoSinapsi casual() {
  tipopesoSinapsi casuale, max = (tipopesoSinapsi)0.1,
                           min = (tipopesoSinapsi)-0.1;
  unsigned int number;
  rand_s(&number);
  casuale = min + ((max - min) *
                   ((tipopesoSinapsi)number /
                    ((tipopesoSinapsi)(UINT_MAX) + (tipopesoSinapsi)1.0)));
  return casuale;
}

int quadrato(int x) {
  if (x == 1) return 4;  // SAREBBE 2 ALLA SECONDA
  x = x * x;
  return x;
}

tipoNeurone sigm_uni(tipoNeurone x) {
  return (tipoNeurone)1.0 /
         ((tipoNeurone)1.0 + (tipoNeurone)exp((tipoNeurone)-2.0 * x));
}

int *estrai_patt_rand(int k) {
  int *occ, i = 0, j = 0, h = 0, trovato = 0;
  unsigned int number;
  /*alloco arrays*/
  occ = calloc(k, sizeof(int));
  memset(occ, -1, sizeof(int));
  /*fine allocazione*/
  for (i = 0; i < k; i++) occ[i] = -1;
  i = 0;
  while (!trovato) {
    rand_s(&number);
    if (occ[j = (number % k)] == -1) {
      occ[j] = i;
      ++i;
    }
    if (i == k - 1) {
      while (occ[h] != -1) ++h;
      occ[h] = i;
      trovato = 1;
    }
  }
  return occ;
}

void SetHiddens(int Input, int Output, int Layer) {
  int tmp, fark, Hid1, Hid2, Hid3;
  fark = (int)(Input - Output);
  if (fark > 0) {
    if (Layer == 3) {
      tmp = fark / 2;
      Hid1 = tmp;
      Hid2 = 0;
      Hid3 = 0;
    } else if (Layer == 4) {
      tmp = fark / 3;
      Hid1 = Input - tmp;
      Hid2 = Hid1 - tmp;
      Hid3 = 0;
    } else if (Layer == 5) {
      tmp = fark / 4;
      Hid1 = Input - tmp;
      Hid2 = Hid1 - tmp;
      Hid3 = Hid2 - tmp;
    }
    if (Hid1 == 0) Hid1 = 1;
    if (Hid2 == 0) Hid2 = 1;
    if (Hid3 == 0) Hid3 = 1;
  } else {
    Hid1 = 1;
    Hid2 = 1;
    Hid3 = 1;
  }
}

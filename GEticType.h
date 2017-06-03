/* grafi etichettati: liste di adiacenza */

typedef char *TipoEticNodo;  // esempio: nodi etichettati da stringhe
typedef float TipoEticArco;  // esempio: archi etichettati da valori numerici

typedef struct arcoEtic {
  int v;
  TipoEticArco etic;
  struct arcoEtic *next;
} * linkEtic;

struct nodoEtic {
  TipoEticNodo etic;
  linkEtic next;
};

typedef struct nodoEtic *TipoGrafoEtic;
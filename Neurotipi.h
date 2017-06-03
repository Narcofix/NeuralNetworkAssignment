/*
uso queste "define" per scegliere il tipo del valore contenuto nel neurone
e il tipo dei valori usati per pesare i collegamenti tra i neuroni(arco_pesato)
*/
#define tipovals float
//#define tipo double
#define tipoNeurone tipovals
#define tipopesoSinapsi tipovals
#define tipoFattori tipovals
#define tipoDeltaVal tipovals
#define IRPROP 1
#define BACKPROP 2
#define FLOATvals 3
#define DOUBLEvals 4
#define NO_SPLIT 1

typedef struct NEURONE *Neuroni;
typedef struct SINAPSI {
  int S;                 /*indice strato neurone adiacente*/
  int Ni;                /*indice neurone adiacente*/
  Neuroni N;             /*neurone adiacente*/
  tipopesoSinapsi W;     /*valore peso Wi*/
  tipopesoSinapsi precW; /*valore peso Wi precedente */
  tipopesoSinapsi D;     /*valore derivata parziale peso Wi*/
  tipopesoSinapsi precD; /*valore derivata parziale peso Wi precedente*/
  tipopesoSinapsi delta; /*valore cambiamento*/
  struct SINAPSI
      *next; /*puntatore al prossimo arco uscente dal neurone nella lista*/
} Sinapsi, *SinapsiLink;

typedef struct
    BACKLINK { /*str.dati utilizzata per la backpropagation(ricorda i neuroni
                  precedenti)*/
  Neuroni N;   /*neurone precedente*/
  int Ni;      /*indice neurone precedente*/
  int S;       /*indice strato neurone precedente*/
  SinapsiLink back;      /*sinapsi corrispondente*/
  struct BACKLINK *next; /*puntatore al prossimo backlink nella lista*/
} BackLink, *BackLinks;

typedef struct NEURONE {
  tipoNeurone val_x;       /*valore X del neurone i*/
  tipoNeurone soglia;      /*valore soglia eccitazione*/
  tipoDeltaVal E;          /*valore delta nurone i*/
  tipoNeurone              /*funzione di attivazione*/
      (*funz_attivazione)  /*puntarore alla f.attiv.*/
      (tipoNeurone val_x); /*parametri*/
  SinapsiLink
      next; /*puntatore alla lista di sinapsi che vanno negli altri neuroni*/
  SinapsiLink coda;       /*puntatore alla coda della lista di sinapsi*/
  BackLinks inn;          /*puntatore alla lista dei neuroni precedenti(per la
                             backpropagation)*/
  BackLinks coda2;        /*puntatore alla coda della lista di backlinks*/
  tipoDeltaVal bias;      /*bias*/
  tipoDeltaVal precWbias; /*valore cambiamento bias*/
  tipopesoSinapsi bias_D; /*valore derivata parziale peso bias*/
  tipopesoSinapsi bias_precD; /*valore derivata parziale peso bias precedente*/
  tipopesoSinapsi bias_delta; /*valore cambiamento bias*/
} Neurone, *Neuroni;

typedef struct STRATO { /*struttura che rappresenta un singolo strato del
                           percettrone*/
  Neuroni neuro_el;     /*neuroni*/
  int num_strato;       /*numero si posizione dello strato*/
  int num_neuro_el;     /*numero neuroni*/
  char tipo_strato;     /*tipo dello strato(input,hidden,output)*/
  tipoDeltaVal bias_E;  /*valore delta bias strato i*/
} STrato, *Strato;

typedef struct RETE {   /*rete multistrato che consiste in un array di strati*/
  Strato *ptr_str;      /**/
  int nstrati;          /**/
  int epoche;           /**/
  tipoFattori momentum; /**/
  tipoFattori precisione;     /**/
  tipoFattori Fapp;           /**/
  tipoFattori Fapp_pos;       /**/
  tipoFattori Fapp_neg;       /**/
  tipoFattori delta_Fapp_min; /**/
  tipoFattori delta_Fapp_max; /**/
} * Rete;

typedef struct LISTANOMI LISTANOMI;
typedef struct LISTANOMI {
  char *nome;
  LISTANOMI *next;
} Listanomi;

typedef struct INFILES {
  char *estensione;
  int totFiles;
  Listanomi *HEAD;
} inFiles;

typedef struct RETESTR {  /*struttura della rete*/
  int *dim;               /*array di dimensioni dei layer*/
  int ndim;               /*numero di strati ricavato in modo automatico*/
  int tnet;               /*tipologia della rete*/
  tipopesoSinapsi **pesi; /*vettore dei pesi letto*/
} retestr, *Retestr;

typedef struct RETETRG { /*struttura del training set*/
  int *dim;              /*array di dimensioni dei patterns*/
  int ndim;              /*numero di dim trovate*/
  int dim_out; /*dimensione di output calcolata in base alla struttura dei
                  patern*/
  tipoNeurone **vals; /*vettore dei valori letti*/
  int npatt;          /*numero di pattern trovati*/
  int *pdim;          /*array delle dim dei pattern*/
  tipoNeurone mse;    /*valore errore globale sul funzionamento*/
} retetrg, *Retetrg;

typedef struct mydata mydata;
typedef mydata *MYDATA;
struct mydata {
  int i; /*indice neurone*/
  int j; /*indice strato*/
  Rete R;
};

// typedef struct PNGTRG{ /*struttura del training set*/
// int dim; /*dimensione PNG (H * L)*/
// int dim_out; /*dimensione di output calcolata in base alla struttura dei
// patern*/
// int *pdim; /*array delle dim dei pattern*/
// double **vals; /*vettore dei valori letti*/
// int npatt; /*numero di pattern trovati*/
//}retetrg,*Retetrg;*/
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Neuroprototipi.h"
//#include "nedmalloc.h"
#include <errno.h>
#include <tchar.h>
//#include "dirent.h"
#include <io.h>
#include <share.h>
#include <string.h>
#include <time.h>

inFiles *apri_dir(char *estensione) {
  int dim;
  char Buffer[1000], prova[1000], *Buff_ts;
  WIN32_FIND_DATA FindFileData;
  WCHAR Buffer2[1000];
  HANDLE hFind;
  // DIR *dir;
  size_t origsize;
  inFiles *INNF;
  Listanomi FileTrovati, *ptrFT, *ftrovato;

  /*procedura per la cattura della directory corrente*/
  _getcwd(Buffer, 1000);
  strcat_s(Buffer, 1000, "\\");
  /*routine ricerca file .ts nella directory*/
  Buff_ts = _strdup(Buffer);
  /*alloco struttura files*/
  INNF = (inFiles *)malloc(sizeof(inFiles));
  INNF->estensione = _strdup(estensione);
  INNF->totFiles = 0;
  dim = strlen(Buff_ts) + 1;
  dim += strlen(INNF->estensione) + 1;
  Buff_ts = (char *)realloc(Buff_ts, dim);
  strcat_s(Buff_ts, dim, INNF->estensione);
  printf("%s\n", Buff_ts);
  MultiByteToWideChar(CP_ACP, 0, Buff_ts, -1, Buffer2, 1000);
  /*preparo la lista*/
  FileTrovati.next = NULL;
  ptrFT = &FileTrovati;
  if ((hFind = FindFirstFile(Buffer2, &FindFileData)) == INVALID_HANDLE_VALUE) {
    printf("**files %s non trovati nella cartella input**\n", estensione);
    FindClose(hFind);
    system("pause");
    exit(1);
  }
  /*mi salvo il primo file trovato*/
  origsize = wcslen(FindFileData.cFileName) + 1;
  wcstombs_s(NULL, prova, origsize, FindFileData.cFileName, 1000);
  ftrovato = (Listanomi *)malloc(sizeof(Listanomi));
  ftrovato->nome = _strdup(prova);
  ftrovato->next = NULL;
  ptrFT->next = ftrovato;
  ptrFT = ptrFT->next;
  INNF->totFiles++;
  /*mi salvo gli altri file trovati*/
  while (FindNextFile(hFind, &FindFileData) != 0) {
    origsize = wcslen(FindFileData.cFileName) + 1;
    wcstombs_s(NULL, prova, origsize, FindFileData.cFileName, 1000);
    ftrovato = (Listanomi *)malloc(sizeof(Listanomi));
    ftrovato->nome = _strdup(prova);
    ftrovato->next = NULL;
    ptrFT->next = ftrovato;
    ptrFT = ptrFT->next;
    INNF->totFiles++;
  }
  printf("\n**Trovati %d Files con estensione %s**", INNF->totFiles,
         estensione);
  INNF->HEAD = FileTrovati.next;
  FindClose(hFind);
  ///*routine ricerca file .net nella directory*/
  // i=1;
  // Buff_net=_strdup(Buffer);
  // realloc(Buff_net,strlen(Buff_net)+6);
  // strcat_s(Buff_net,strlen(Buff_net)+6,"*.net");
  // MultiByteToWideChar(CP_ACP, 0, Buff_net, -1, Buffer2, 1000);
  // if((hFind1= FindFirstFile(Buffer2, &FindFileData))== INVALID_HANDLE_VALUE){
  // printf("\n**files .net non trovati nella cartella input**");
  // INNF->nnet=0;
  // closedir (dir);
  // FindClose(hFind1);
  // return INNF;
  //}
  ///*mi salvo il primo file trovato*/
  // origsize = wcslen(FindFileData.cFileName) + 1;
  // wcstombs_s(NULL, prova, origsize,FindFileData.cFileName , 1000 );
  // INNF->NET=malloc(sizeof(NM*));
  // INNF->NET[0]=_strdup(prova);
  ///*mi salvo gli altri file trovati*/
  // while(FindNextFile(hFind1,&FindFileData)!=0){
  // origsize = wcslen(FindFileData.cFileName) + 1;
  // wcstombs_s(NULL, prova, origsize,FindFileData.cFileName , 1000);
  // INNF->NET=realloc( INNF->NET ,(++i) * sizeof(NM*));
  // INNF->NET[i-1]=_strdup(prova);
  //}
  // INNF->nnet=i;/*numero file trovati*/
  // printf("\n**Trovati %d Files con estensione .net**",i);
  // FindClose(hFind1);25
  return INNF;
}

// int apri_files_net(inFiles * INNF){
int apri_files(inFiles *INNF) {
  int i, sel;
  char c[100], *ptr;
  Listanomi *ptrFT;
  if (INNF->totFiles < 1) {
    printf("\n**files %s non presenti**", INNF->estensione);
    return -1;
  }
  if (INNF->totFiles >= 1) {
    printf("\n** trovati piu' files %s: **", INNF->estensione);
    for (i = 1, ptrFT = INNF->HEAD; ptrFT != NULL; i++, ptrFT = ptrFT->next) {
      printf("\n**[ %d ] %s", i, ptrFT->nome);
    }
    printf("\n**[ -1 ] se non lo vuoi caricare");
    printf("\n**Scegli uno di questi Files:");
    while (1) {
      memset(c, 0, 100);
      scanf_s("%99s", c, 100);
      sel = atoi(c);
      fflush(stdin);
      ptr = c;
      i = 1;
      while ((*ptr == '-' || *ptr == '+') && i) {
        ptr++;
        i--;
      }
      while (isdigit(*ptr)) ptr++;
      if (sel == 0 || *ptr != '\0') {
        printf("\n**scelta errata1!**\nreinserisci:");
        continue;
      }
      if ((sel == -1 || *ptr != '\0') &&
          (strcmp(INNF->estensione, "*.net") == 0)) {
        printf("\n**hai scelto di non caricare il .net**\n");
        return -1;
      }
      /*if(sel==-2 || *ptr!='\0'){
      printf("\n**hai scelto di usare architettura mista**\n");
      return -2;
      }*/
      --sel;
      if (sel <= (INNF->totFiles - 1) && sel > -1) break;
      printf("\n**scelta errata!**\nreinserisci:");
    }
  }
  return sel;
}
/*
int apri_files_ts(inFiles * INNF){
int i,sel;
char c[100];
if(INNF->nts < 1){
printf("\n**files .ts non presenti**");
system("pause");
exit (1);
}
if(INNF->nts >1){
printf("\n** trovati piu' files .ts: **");
for(i=1; i< INNF->nts+1 ;i++){
printf("\n**[ %d ] %s",i,INNF->TS[i-1]);
}
printf("\n**Scegli uno di questi Files:");
while(1){
memset(c,0,100);
scanf_s("%99s",c,100);
sel=atoi(c);
fflush(stdin);
if(sel==0){
printf("\n**scelta errata1!**\nreinserisci:");
continue;
}
--sel;
if(sel <= (INNF->nts-1) && sel > -1)break;
printf("\n**scelta errata!**\nreinserisci:");
}
}else{
printf("\n**trovatoil file: %s",INNF->TS[0]);
sel=0;
}//caso un file solo
return sel;
}*/

char *apri_il_net(inFiles *INNF, int sel) {
  char *buff, c;
  int i = 0, count = 0;
  FILE *ptrfile;

  if ((fopen_s(&ptrfile, getSelezione(INNF, sel), "r")) != 0) {
    printf("errore file");
    system("pause");
    exit(1);
  }
  while ((c = fgetc(ptrfile)) != EOF) {
    count++;
  }
  fseek(ptrfile, 0, SEEK_SET);
  buff = malloc(count + 1 * sizeof(char));
  i = fread(buff, 1, count, ptrfile);
  if (i != count) {
    printf("errore bufferizzazine file");
    system("pause");
    exit(1);
  }
  buff[i] = '\0';
  _fcloseall();
  return buff;
}

char *apri_il_ts(inFiles *INNF, int sel) {
  char *buff, c;
  int i = 0, count = 0;
  FILE *ptrfile;

  buff = malloc(sizeof(char));
  if ((fopen_s(&ptrfile, getSelezione(INNF, sel), "r")) != 0) {
    printf("errore file");
    system("pause");
    exit(1);
  }
  while ((c = fgetc(ptrfile)) != EOF) {
    count++;
  }
  fseek(ptrfile, 0, SEEK_SET);
  buff = malloc(count + 1 * sizeof(char));
  i = fread(buff, 1, count, ptrfile);
  if (i != count) {
    printf("errore bufferizzazine file");
    system("pause");
    exit(1);
  }
  buff[i] = '\0';
  fclose(ptrfile);
  return buff;
}

Retestr analizza_net(char *buff) {
  int lay = 0, lay1 = 0, tot = 0;
  char *ptr, *ptr2, *ptr3, *ptr4;
  Retestr struttura;
  /*catturo tutte le dimensioni dei layer*/
  ptr2 = narcostrstr(buff, WORDKEY1, NULL);
  if (ptr2 == NULL) {
    printf("\n**tag /dim di chiusura non presente nel file net**");
    system("pause");
    exit(1);
  }
  ptr = narcostrstr(buff, WORDKEY, ptr2);
  if (ptr == NULL) {
    printf("\n**tag dim non presente nel file net**");
    system("pause");
    exit(1);
  }
  struttura = calloc(1, sizeof(retestr));  // preparo la struttura
  struttura->dim = calloc(1, sizeof(int));
  ptr += strlen(WORDKEY);
  while ((ptr) != ptr2) {
    while (!(isdigit(*ptr))) {  // ciclo fino al primo numero
      if ((ptr) == ptr2) break;
      ++ptr;
    }
    if ((ptr) == ptr2) break;
    struttura->ndim++;
    if (struttura->ndim > 1)
      struttura->dim = realloc(struttura->dim, struttura->ndim * sizeof(int));
    struttura->dim[struttura->ndim - 1] =
        (int)strtol(ptr, &ptr4, 10);  // salvo l' intero trovato
    ptr = ptr4;
  }
  /*inizializzo la matrice dei pesi catturati*/
  /*ha n-1 righe dove n � il num di strati*/
  /*ogni riga � grande come la dim letta da file*/
  struttura->pesi = malloc((struttura->ndim - 1) * sizeof(tipopesoSinapsi *));
  for (lay = 0; lay < (struttura->ndim - 1); lay++) {
    struttura->pesi[lay] =
        malloc(((struttura->dim[lay] + 1) * struttura->dim[lay + 1]) *
               sizeof(tipopesoSinapsi));
  } /*bias*/
  /*catturo la tipologia della rete*/
  ptr2 = narcostrstr(ptr2, WORDKEY3, NULL);
  if (ptr2 == NULL) {
    printf("\n**tag /dim di chiusura non presente nel file net**");
    system("pause");
    exit(1);
  }
  ptr = narcostrstr(ptr, WORDKEY2, ptr2);
  if (ptr == NULL) {
    printf(
        "\n**tag net non presente nel file net**\n**impossibile stabilire il "
        "tipo della rete**\n");
    system("pause");
    exit(1);
  }
  ptr += strlen(WORDKEY2);
  while ((ptr) != ptr2) {
    while (!(isdigit(*ptr))) {  // ciclo fino al primo numero
      if (ptr == ptr2) break;
      ++ptr;
    }
    if (ptr == ptr2) break;
    struttura->tnet = atoi(ptr);  // salvo l' intero trovato
    break;
  }
  if (struttura->tnet != 1) {
    printf(
        "\n**tipologia letta non consentita**\n**imposta 1 per il "
        "percettrone**\n");
    system("pause");
    exit(1);
  } else {
    printf("\n**hai scelto il percettrone multistrato**\n");
  }
  /*catturo i pesi delle sinapsi della rete*/
  lay = 0;
  ptr2 = narcostrstr(buff, WORDKEY5, NULL);
  if (ptr2 == NULL) {
    printf("\n**tag /pesi di chiusura non presente nel file net**");
    system("pause");
    exit(1);
  }
  ptr = narcostrstr(buff, WORDKEY4, ptr2);
  if (ptr == NULL) {
    printf(
        "\n**tag pesi non presente nel file net**\n**impossibile leggere i "
        "pesi**\n");
    system("pause");
    exit(1);
  }
  ptr += strlen(WORDKEY4);
  while ((ptr) != ptr2) {
    /*fase di ricerca del primo <*/
    while (*ptr != '<') {
      ++ptr;
      if (*ptr == '>') {
        printf("\n**2)valori dei pesi non settati bene!!**\n");
        system("pause");
        exit(1);
      }
    }
    if (ptr == ptr2) break;
    /*da qui mi trovo sicuramente su un <*/
    ++ptr;   /*lo supero*/
    ++lay1;  /*aumento il numero di layer scoperti*/
    lay = 0; /*azzero il numero di pesi scoperti*/
    if (lay1 >= (struttura->ndim)) {
      printf(
          "\n**valori pesi non settati bene(riga/e trovate maggiori di quelle "
          "attese)**\n");
      system("pause");
      exit(1);
    }
    while (*ptr != '>') {
      /*da qui mi trovo tra <> e quindi cerco i valori floats*/
      while (!isdigit(*ptr)) {
        if (*ptr == '>') break;  // caso uscita sul >
        ++ptr;
        if (ptr == ptr2) {
          printf("\n**3)valori dei pesi non settati bene!!**\n");
          system("pause");
          exit(1);
        }
      }
      if (*ptr == '>') break;  // caso uscita sul >
      ++lay;
      if (*(ptr - 1) == '-' || *(ptr - 1) == '+') ptr--;
      struttura->pesi[lay1 - 1][lay - 1] = (tipopesoSinapsi)strtod(ptr, &ptr3);
      // printf("%f ",struttura->pesi[lay1-1][lay-1]);
      /*da qui mi trovo dopo il double letto */
      ptr = ptr3;
    }
    /*da qui mi trovo sul > */
    /*controllo se i valori letti sono < di quelli aspettati*/
    if (lay != ((struttura->dim[lay1 - 1] + 1) * struttura->dim[lay1])) {
      printf(
          "\n**valori pesi non settati bene(pesi trovati diversi rispetto "
          "quantita' attesa)!!**\n");
      system("pause");
      exit(1);
    }
    ++ptr; /*vado oltre il >*/
           // printf("\n");
  }
  if (lay1 < (struttura->ndim - 1)) {
    printf("\n**valori pesi assenti,inizializzo in modo casuale**\n");
    struttura->pesi = NULL;
    return struttura;
  }
  printf("\n**file net caricato correttamente**\n");
  return struttura;
}

Retetrg analizza_ts(char *buff) {
  int lay = 0, lay1 = 0, lay2 = 0, tot = 0, i = 0;
  char *ptr, *ptr2, *ptr3, *ptr4;
  Retetrg struttura;
  /*catturo tutte la dimensione dell ' input*/
  ptr2 = narcostrstr(buff, WORDKEY1, NULL);
  if (ptr2 == NULL) {
    printf("\n**tag /dim di chiusura non presente nel file net**");
    system("pause");
    exit(1);
  }
  ptr = narcostrstr(buff, WORDKEY, ptr2);
  if (ptr == NULL) {
    printf("\n**tag dim non presente nel file net**");
    system("pause");
    exit(1);
  }
  struttura = calloc(1, sizeof(retetrg));  // preparo la struttura
  struttura->dim = calloc(1, sizeof(int));
  ptr += strlen(WORDKEY);
  while ((ptr) != ptr2) {
    while (!(isdigit(*ptr))) {  // ciclo fino al primo numero
      if ((ptr) == ptr2) break;
      ++ptr;
    }
    if ((ptr) == ptr2) break;
    struttura->ndim++;
    if (struttura->ndim > 1)
      struttura->dim = realloc(struttura->dim, struttura->ndim * sizeof(int));
    struttura->dim[struttura->ndim - 1] = atoi(ptr);  // salvo l' intero trovato
    ++ptr;
  }
  /*dimensione cresce con ogni valore letto*/
  /*catturo i valori di input della rete*/
  /*fase di ricerca del primo <*/
  ptr = ptr2 + strlen(WORDKEY1);
  while (*ptr != '<') {
    ++ptr;
    if (*ptr == '>') {
      printf("\n**1)valori non settati bene!!**\n");
      system("pause");
      exit(1);
    }
    if (*ptr == '\0') {
      printf("\n**valori non settati bene(forse manca chiusura'>')!!**\n");
      system("pause");
      exit(1);
    }
  }
  /*da qui mi trovo sicuramente su un <*/
  ptr4 = ++ptr; /*lo supero*/
  lay = 0;
  while (*ptr != '>') {
    while (*ptr != '\n') {
      if (*ptr == '>') break;
      ++ptr;
    }
    if (*ptr == '>') break;
    ++tot;
    ++ptr;
  }
  // struttura->npatt=tot;
  /*alloco la struttura che conterr� i valori per il funzionamento*/
  struttura->vals = calloc(tot, sizeof(tipoNeurone *));
  /*da qui mi trovo tra <> e quindi cerco i valori floats*/
  /*preparo i ptr*/
  ptr = ptr4;
  ptr3 = NULL;
  /*preparo i contatori*/
  lay1 = lay = 0;
  /*alloco..*/
  // struttura->vals = calloc(1, sizeof(tipoNeurone *));
  while (*ptr != '>') {
    while (*ptr != '\n') {
      if (*ptr == '>') break;
      if (*ptr == '\0' || *ptr == '<') {
        printf("\n**1)valori non settati bene!!**\n");
        system("pause");
        exit(1);
      }
      if (isdigit(*ptr)) {
        if (*(ptr - 1) == '-' || *(ptr - 1) == '+') ptr--;
        struttura->vals[lay1] =
            realloc(struttura->vals[lay1], (lay + 1) * sizeof(tipoNeurone));
        struttura->vals[lay1][lay] = (tipoNeurone)strtod(ptr, &ptr3);
        ++lay;
        ptr = ptr3;
      } else {
        ++ptr;
      }
    }
    if (*ptr == '>') break;
    if (lay != 0) {
      struttura->pdim = realloc(struttura->pdim, (lay1 + 1) * sizeof(int));
      struttura->pdim[lay1] = lay;  // mi salvo la dim del pattern*/
      ++lay1;
      // struttura->vals = realloc(struttura->vals, (lay1+1) *
      // sizeof(tipoNeurone* ));
      lay = 0;
    }
    ++ptr;
  }
  if (lay != 0) {
    struttura->pdim = realloc(struttura->pdim, (lay1 + 1) * sizeof(int));
    struttura->pdim[lay1] = lay;
    ++lay1;
  }
  struttura->npatt = lay1;
  /*cerco la dimensione corretta per l' output,scelgo quella che nel ts mi
   * permette di non avere buchi*/
  lay = 0;
  for (tot = 0; tot < struttura->npatt; tot++) {
    if ((struttura->pdim[tot] - struttura->dim[0]) != 0) lay = 1;
  }
  if (!lay) {
    struttura->dim_out = 0;
    printf("\n**file ts caricato correttamente**\n");
    printf("\n**modalita' funzionamneto(output da calcolare)**\n");
    return struttura;
  }
  lay = (struttura->pdim[0] - struttura->dim[0]);
  for (tot = 0; tot < struttura->npatt; tot++) {
    if (lay > (struttura->pdim[tot] - struttura->dim[0]))
      lay = (struttura->pdim[tot] - struttura->dim[0]);
    if (lay == 0) {
      printf("\n**trovato un pattern senza output(correggi!)**\n");
      system("pause");
      exit(1);
    }
    if (lay < 0) {
      printf("\n**trovato un pattern errato(valori < rispetto dimensione)**\n");
      system("pause");
      exit(1);
    }
  }
  /*mi salvo la dim dell' output ottimale*/
  struttura->dim_out = lay;
  printf("\n**file ts caricato correttamente**\n");
  free(struttura->pdim);
  return struttura;
}

char *Net_save(Rete R, inFiles *INNFts, inFiles *INNFnet, int sel, int tipoF,
               tipoNeurone **out, Retetrg struttura2) {
  int i, j, len;
  char prova[1000], *prova2, *ptr, *filescelto;
  time_t ora_t;
  struct tm ora;
  FILE *ptrfile;
  SinapsiLink ptr1;
  /*preparo nome con data*/
  time(&ora_t);
  localtime_s(&ora, &ora_t);
  strftime(prova, 1000, "%d_%b_%Y_%H_%M_%S&", &ora);
  if (tipoF == LEARN_MODE) {
    filescelto = getSelezione(INNFts, sel);
    strcat_s(prova, 1000, filescelto);
    prova[(len = strlen(prova)) - 3] = '.';
    prova[len - 2] = 'd';
    prova[len - 1] = 'a';
    prova[len] = 't';
    prova[len + 1] = '\0';
    prova2 = _strdup(prova);
    prova[len - 2] = 'n';
    prova[len - 1] = 'e';
    prova[len] = 't';
    prova[len + 1] = '\0';
  } else {
    if (tipoF == FUNC_MODE) {
      if (out == NULL) {
        printf("\n**struttura valori out assente**\n");
        system("pause");
        exit(1);
      }
      len = 0;
      filescelto = getSelezione(INNFnet, sel);
      while (filescelto[len++] != '&')
        ;
      ptr = filescelto;
      strcat_s(prova, 1000, ptr + len);
      while (filescelto[len++] != '.')
        ;
      prova[len] = 'o';
      prova[len + 1] = 'u';
      prova[len + 2] = 't';
      prova[len + 3] = '_';
      prova[len + 4] = 't';
      prova[len + 5] = 's';
      prova[len + 6] = '\0';
      if ((fopen_s(&ptrfile, prova, "w")) != 0) {
        printf("errore file");
        system("pause");
        exit(1);
      }
      fprintf(ptrfile,
              "<valori ottenuti con la computazione del file di input>\n<\n ");
      for (len = 0; len < struttura2->npatt; len++) {
        for (i = 0; i < struttura2->dim_out; i++) {
          fprintf(ptrfile, " %.10f --> %.10f", out[len][i],
                  struttura2->vals[len][struttura2->dim[0] + i]);
          printf(" %.10f --> %.10f", out[len][i],
                 struttura2->vals[len][struttura2->dim[0] + i]);
        }
        fprintf(ptrfile, "\n");
        printf("\n");
      }
      fprintf(ptrfile, " >");
      fclose(ptrfile);
      printf("mse --> %.10f\n", struttura2->mse / struttura2->npatt);
      if (get_continue()) return NULL;
      return "";
    }
  }
  /**/
  if ((fopen_s(&ptrfile, prova, "w")) != 0) {
    printf("errore file");
    system("pause");
    exit(1);
  }
  /* caso salvataggio dati dell' apprendimento*/
  fprintf(ptrfile, "<dim> ");
  for (i = 0; i < R->nstrati; i++) {
    fprintf(ptrfile, "%d ", R->ptr_str[i]->num_neuro_el);
  }
  fprintf(ptrfile, " </dim>\n");
  fprintf(ptrfile, "<net> ");
  fprintf(ptrfile, "1");
  fprintf(ptrfile, " </net>\n");
  fprintf(ptrfile, "<pesi>\n");
  for (i = 0; i < R->nstrati - 1; i++) {
    fprintf(ptrfile, "< ");
    for (j = 0; j < R->ptr_str[i]->num_neuro_el; j++) {
      ptr1 = R->ptr_str[i]->neuro_el[j].next;
      fprintf(ptrfile, "( ");
      while (ptr1 != NULL) {
        fprintf(ptrfile, "%g ", ptr1->W);
        ptr1 = ptr1->next;
      }
      fprintf(ptrfile, ") ");
    }
    fprintf(ptrfile, "bias( ");
    for (j = 0; j < R->ptr_str[i + 1]->num_neuro_el; j++) {
      fprintf(ptrfile, "%g ", R->ptr_str[i + 1]->neuro_el[j].bias);
    }
    fprintf(ptrfile, ") ");
    fprintf(ptrfile, ">\n");
  }
  fprintf(ptrfile, "</pesi>");
  fclose(ptrfile);
  return prova2;
}

char *getSelezione(inFiles *INNF, int sel) {
  Listanomi *ptrFT;
  int i;
  for (i = 0, ptrFT = INNF->HEAD; i != sel; i++, ptrFT = ptrFT->next)
    ;
  return ptrFT->nome;
}

FILE *crea_temp_file(char **prova) {
  FILE *ptrfile;
  time_t ora_t;
  struct tm ora;
  /*preparo nome con data*/
  *prova = malloc(1000 * sizeof(char));
  time(&ora_t);
  localtime_s(&ora, &ora_t);
  strftime(*prova, 1000, "%d_%b_%Y_%H_%M_%S_", &ora);
  strcat_s(*prova, 1000, "TEMP-FILE.dat");
  if ((ptrfile = _fsopen(*prova, "w+", _SH_DENYNO)) == NULL) {
    printf("errore file temp");
    system("pause");
    exit(1);
  }
  return ptrfile;
}

int crea_grafico(char *nameF, char *nameF_old, FILE *gnuplot) {
  // char Buffer[1000],Buffer2[1000];
  char nameF1[1000], *nameF2;
  int len;
  // FILE * ptrfile;
  // time_t ora_t;
  // struct tm ora;
  /*preparo nome con data*/
  rename(nameF_old, nameF);
  free(nameF_old);
  fprintf_s(gnuplot, "plot \"%s\" with lines lw 2 lc 3\n", nameF);
  fprintf_s(gnuplot, "pause mouse any\n");
  fprintf_s(gnuplot, "set term \"gif\"\n");
  nameF2 = _strdup(nameF);
  len = strlen(nameF2);
  nameF2[len] = '\000';
  nameF2[len - 1] = 'f';
  nameF2[len - 2] = 'i';
  nameF2[len - 3] = 'g';
  fprintf_s(gnuplot, "set output\"%s\"\n", nameF2);
  fprintf_s(gnuplot, "plot \"%s\" with lines lw 2 lc 3\n", nameF);
  remove(nameF1);
  return 0;
}

FILE *crea_grafico_pipe() {
  char Buffer[1000];
  FILE *ptrfile;
  /*procedura per la cattura della directory per il grafico*/
  _getcwd(Buffer, 1000 * sizeof(char));
  strcat_s(Buffer, 1000, "\\binary\\");
  _chdir(Buffer);  // cambia la posizione della cartella di lavoro
  _getcwd(Buffer, 1000 * sizeof(char));
  memset(Buffer, 0, 1000);
  strcpy_s(Buffer, 1000, "gnuplot");
  ptrfile = _popen(Buffer, "w");
  // creo i comandi
  fprintf_s(ptrfile, "set xlabel \"EPOCHE\" font \"arial,15\"\n");
  fprintf_s(ptrfile, "set ylabel \"MSE\" font \"arial,15\"\n");
  fprintf_s(ptrfile, "set autoscale\n");
  fprintf_s(ptrfile, "set terminal windows\n");
  fprintf_s(ptrfile, "cd \"%s\" \n", "..");
  fprintf_s(ptrfile, "cd \"%s\" \n", "input_files");
  fflush(ptrfile);
  _getcwd(Buffer, 1000 * sizeof(char));
  strcpy_s(Buffer, 1000, "..");
  _chdir(Buffer);  // cambia la posizione della cartella di lavoro
  _getcwd(Buffer, 1000 * sizeof(char));
  strcat_s(Buffer, 1000, "\\input_files\\");
  _chdir(Buffer);  // cambia la posizione della cartella di lavoro
  return ptrfile;
}

char *clean_str(char *link) {
  int j = 0, dim, dim2 = 0;
  char *prv, *prv2;
  dim = strlen(link) + 1;
  prv = link;
  while ((prv = strstr(prv, "\\")) != NULL) {
    j = prv - link;
    prv2 = malloc(strlen(link + j) + 1 * sizeof(char *));
    strcpy_s(prv2, strlen(link + j) + 1, link + j);
    dim2 += 2;
    link[j] = '\\';
    link[j + 1] = '\\';
    if ((strlen(prv2) + 1) > 1) memset(link + j + 2, 0, strlen(link + j + 2));
    if ((strlen(prv2) + 1) > 1) strcat_s(link + j + 2, dim + dim2, prv2 + 1);
    prv += 2;
    free(prv2);
  }
  return link;
}

Retetrg splitta_test_set(Retetrg orig) {
  int perc, i;
  Retetrg struttura = malloc(sizeof(retetrg));  // preparo la struttura
  struttura->npatt = perc = (int)(orig->npatt * 0.2);
  struttura->dim = malloc(sizeof(int));
  struttura->dim[0] = orig->dim[0];
  struttura->dim_out = orig->dim_out;
  struttura->vals = malloc(perc * sizeof(double *));  // preparo la struttura
  for (i = 0; i < perc; i++) {
    struttura->vals[i] = malloc((orig->dim[0] + orig->dim_out) *
                                sizeof(double));  // preparo la struttura
    memcpy(struttura->vals[i], orig->vals[i + (orig->npatt - perc)],
           (orig->dim[0] + orig->dim_out) * sizeof(double));
  }
  for (i = 0; i < 3; i++) free(orig->vals[(orig->npatt - perc) + i]);
  orig->npatt -= perc;
  return struttura;
}
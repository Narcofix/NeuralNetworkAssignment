#include <math.h>
#include <share.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "Neuroprototipi.h"
#include "nedmalloc.h"
/*******************/ /***************************************************************/
/*homework parte 1 */ /**/
/*Manuel Apilongo  */ /**/
/*mtr:175880       */ /**/
/*******************/ /***************************************************************/
int stop;

void esci(int x) {
  stop = 1;
  return;
}

int main() {
  int dim_out = 0, dim_out1 = 0, old_dim_out = 0, k, epoca = 0, sel_ts, sel_net,
      *occ, modo, modo1;
  char *nameF, *nameF1;
  tipoNeurone mse = 0.0, **out;
  Rete R;
  inFiles* INNF;
  Retestr struttura;
  Retetrg struttura2, struttura3;
  FILE* ptrfile;
  FILE* gnuplot;
  /*inizializzo il trapper*/
  signal(SIGINT, esci);
  /*cerco i file da aprire*/
  gnuplot = crea_grafico_pipe();
  INNF = apri_dir();
  sel_net = apri_files_net(INNF); /*lista file .net disponibili*/
  if (sel_net != -1 && sel_net != -2)
    struttura =
        analizza_net(apri_il_net(INNF, sel_net)); /*caso architettura da file*/
  sel_ts = apri_files_ts(INNF);                   /*lista file .ts disponibili*/
  struttura2 =
      analizza_ts(apri_il_ts(INNF, sel_ts)); /*bufferizzazione file ts*/
  if (struttura2->dim_out != 0 &&
      (sel_net != -1)) {                 /*Scelta tipo di esecuzione: */
    modo = get_modo_func();              /*   viene richiesto se      */
    if (modo == 1) {                     /*        eseguire           */
      old_dim_out = struttura2->dim_out; /*    un' apprendimento      */
      struttura2->dim_out = 0;           /*  oppure un funzionamento  */
    }
  }
  /**************************************************/
  /*modalit� funzionamento*/
  if (struttura2->dim_out == 0 && (sel_net != -1)) {
    if (struttura->pesi == NULL) {
      printf("\n**pesi nel File .net necessari per computare il file .ts**\n");
      system("pause");
      exit(0);
    }
    R = CreaArchitettura_file(struttura, 0);
    out = funzionamento(R, struttura, struttura2);
    if ((Net_save(R, INNF, sel_net, FUNC_MODE, out, struttura2)) == NULL) {
      struttura2->dim_out = old_dim_out;
      goto APP;
    }
    system("pause");
    return 0;
  } else if (struttura2->dim_out == 0 && (sel_net == -1)) {
    printf("\n**File .net necessario per computare il file .ts**\n");
    system("PAUSE");
    return 0;
  } /*FINE modalit� funzionamento*/
  /**************************************************/
  /*Configurazione architettura per l' apprendimento*/
  if (sel_net == -1)
    R = CreaArchitettura_default(struttura2);
  else if (sel_net == -2)
    R = Crea_Architettura_CERCHI();
  /*capisco se usare i pesi di default o letti da file*/
  else if (struttura->pesi == NULL)
    R = CreaArchitettura_file(struttura, 1);
  else
    R = CreaArchitettura_file(struttura, 0);
/**************************************************/
APP:
  /*apprendimento*/  // a questo punto l' architettura � pronta per essere
                     // addestrata
  ptrfile = crea_temp_file(&nameF1);
  modo = get_modo_app();  // scelta dell' algoritmo di apprendimento
  if (modo == BACKPROP) get_momento_fapp(R);
  get_mse(R);
  /*inizializzo valori default irprop*/
  if (modo == IRPROP) {
    R->delta_Fapp_max = (tipoFattori)50.0;
    R->delta_Fapp_min = (tipoFattori)0.0;
    R->Fapp_neg = (tipoFattori)0.5;
    R->Fapp_pos = (tipoFattori)1.2;
    R->epoche = 5000;
  }
  modo1 = NO_SPLIT;
  if (modo1 != NO_SPLIT)
    struttura3 =
        splitta_test_set(struttura2);  // split del training set (optionale)
  for (epoca = 0; epoca < R->epoche && !stop; epoca++) { /*per ogni epoca*/
    occ = estrai_patt_rand(struttura2->npatt);
    if (modo1 != NO_SPLIT) struttura3->mse = 0.0;
    mse = 0.0;
    for (k = 0; k < struttura2->npatt; k++) { /*per ogni pattern nel ts*/
      /*aggiorno valori input*/
      agg_input(R->ptr_str[0], struttura2->vals[occ[k]]);
      /*feed forward*/
      propagavals(R);
      /*calcolo delta out*/
      delta_outs(R, struttura2, occ[k]);
      /*calcolo delta intermedi*/
      propagaerrors(R);
      if (modo == BACKPROP) {
        /*aggiorno pesi tecnica Fapp fissa*/
        aggiorna_pesi_fapp_fissa(R);
      }
      /*aggiorno i gradienti per la rProp-*/
      if (modo == IRPROP) updategradients(R);
      /*sommatoria mse ottenuti su ogni patterns*/
      mse += err_quad(R->ptr_str[R->nstrati - 1], struttura2->vals[occ[k]],
                      struttura2->dim[0]);
    }
    /*aggiorno pesi tecnica iRprop-*/
    if (modo == IRPROP) aggiorna_pesi_rprop(R);
    if (modo1 == NO_SPLIT) {
      mse /= struttura2->npatt;
    } else { /*eseguo funzionamento sulla parte splittata per verificare errore
                su pattern sconosciuti*/
      struttura = get_info_rete(R);
      out = funzionamento(R, struttura, struttura3);
      nedfree(out);
      mse = (struttura3->mse / struttura3->npatt);
    }
    fprintf(ptrfile, "%g\n", mse);
    fflush(ptrfile);
    printf("\n mse %.10g epoca %d", mse, epoca);
    fflush(stdout);

    if (epoca == 2) {
      fprintf_s(gnuplot, "plot \"%s\" with lines  lw 2 lc 3 \n", nameF1);
      fflush(gnuplot);
    } else if (epoca > 2 && (epoca % 40 == 0)) {
      fprintf_s(gnuplot, "replot\n");
      fflush(gnuplot);
    }
    if (mse < R->precisione) break;
  }
  fclose(ptrfile);
  Sleep(1);
  nameF = Net_save(R, INNF, sel_ts, LEARN_MODE, NULL, struttura2);
  if (epoca > 0) crea_grafico(nameF, nameF1, gnuplot);
  _fcloseall();
  system("PAUSE");
  return 0;
}

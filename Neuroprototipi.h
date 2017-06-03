#include "Neurotipi.h"
#include "stdio.h"

/*******************Prototipi per manipolare le strutture dati********************/
Rete								InitReteN(int Num_Strati,int epoche,tipoFattori Fapp,tipoFattori momentum);
Strato						InitStrato(int Num_neuroni,char tipo,int posizione,tipoNeurone (*funz_att)(tipoNeurone val_x));
Rete								CreaArchitettura_file(Retestr struttura,int no_pesi);
Rete        CreaArchitettura_default(Retetrg struttura2);
Rete        Crea_Architettura_CERCHI();
//Strato	     freeStrato(Strato neuroStr);
int									get_continue();
int									get_modo_func();
int         get_modo_app();
void        get_momento_fapp(Rete R);
void								get_mse(Rete R);
void        agg_input(Strato ptr_str,tipoNeurone * vals);
/*int			      EsisteNeuroneOutput(Strato neuroStr, int Nj);
int			      EsisteSinapsi(SinapsiLink head, int Nn , int Ns);*/
void								Insertsinapsi(Strato neuroStr1,Strato neuroStr2, int Ni, int Nj, tipopesoSinapsi x);
void								CreaSinapsi(Strato neuroStr1,Strato neuroStr2);
void        CreaSinapsi_file(Strato neuroStr1,Strato neuroStr2,tipopesoSinapsi *pesi);
void        propagavals(Rete R);
tipoNeurone potential(Neuroni ptr1);
unsigned __stdcall potential_T ( void *pmydata);
tipoNeurone delta_out(tipoNeurone rete_out,tipoNeurone out_ts);
void        delta_outs(Rete R,Retetrg struttura2,int occ);
void        propagaerrors(Rete R);
void        app_momento(Rete R);
void        aggiorna_pesi_fapp_fissa(Rete R);
void        updategradients(Rete R);
void	    			aggiorna_pesi_rprop(Rete R);
tipoNeurone err_quad(Strato ptr_str,tipoNeurone * vals,int dim);
tipoNeurone **   funzionamento(Rete R,Retestr struttura,Retetrg struttura2);
Retestr     get_info_rete(Rete R);
/*******************Prototipi per manipolare i files********************/
inFiles*    apri_dir();
int									apri_files_net(inFiles * INNF);
int									apri_files_ts(inFiles * INNF);
char*							apri_il_net(inFiles * INNF,int sel);
char*							apri_il_ts(inFiles * INNF,int sel);
Retestr					analizza_net(char *buff);
Retetrg			 	analizza_ts(char *buff);
char*	      Net_save(Rete R,inFiles *INNF,int sel,int tipoF,tipoNeurone ** out,Retetrg struttura2);
FILE*       crea_temp_file(char** prova);
int         crea_grafico(char * nameF,char * nameF_old);
char*       clean_str(char * link);
Retetrg     splitta_test_set(Retetrg orig);
/*******************Prototipi altre funzioni********************/
char*       narcostrstr (buf, sub, limit);
tipopesoSinapsi		    casual();
int			      quadrato(int x);
tipoNeurone	sigm_uni(tipoNeurone x);
tipoNeurone derivata_sigm(tipoNeurone rete_out);
#define		   sign(x)( ((x) < 0) ?  -1 : ( ((x) == 0 ) ? 0 : 1) )
#define     taglio(x, basso, alto) ( ((x) < (basso)) ? (basso) : (((x) > (alto)) ? (alto) : (x)))
#define     trasforma(val,min,max)((((float)(val - min)) / ((float)(max - min))))

int *		     estrai_patt_rand(int k);
/*******************Costanti dichiarate visibili a tutto il progetto********************/
extern char WORDKEY[],WORDKEY1[],WORDKEY2[],WORDKEY3[],WORDKEY4[],WORDKEY5[],WORDKEY6[],WORDKEY7[];
extern int  FUNC_MODE,LEARN_MODE;
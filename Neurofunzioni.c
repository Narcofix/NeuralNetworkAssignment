#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nedmalloc.h"
#include "Neuroprototipi.h"
#include <process.h>
#include <string.h>
#include <windows.h> 
//#include <windows.h>
/*funzione per inizializzare la rete*/

Rete InitReteN(int Num_Strati,int epoche,tipoFattori Fapp,tipoFattori momentum){
 Rete R=nedcalloc(1,sizeof(*R));
 R->ptr_str=nedcalloc(Num_Strati,sizeof(Strato));
 R->nstrati=Num_Strati;
 R->epoche=epoche;          // numero di epoche
 R->Fapp=Fapp;              // fattore di apprendimento
 R->momentum=momentum;      // fattore di inerzia
 return R;
}
/*inizializzo il percettrone con la struttura letta da file*/
Rete CreaArchitettura_file(Retestr struttura,int no_pesi){
 Rete R;
 tipopesoSinapsi acc=0.0;
 int i;//j;
 char c;
// SinapsiLink ptr;
 R=InitReteN(struttura->ndim,400000,(tipoFattori)0.1,(tipoFattori)0.1);
 for(i=0;i<struttura->ndim;i++){
  if(i==0)c='i';
  if(i>0 && i<struttura->ndim-2)c='h';
  if(i==(struttura->ndim-1))c='o';
  R->ptr_str[i]=InitStrato(struttura->dim[i],c,i,sigm_uni);
 }
 if(no_pesi){
/*creo le sinapsi con i valori casuali poiché assenti da file*/
  for(i=0;i<R->nstrati-1;i++){
   CreaSinapsi(R->ptr_str[i],R->ptr_str[i+1]);
  }
 /* do{
   acc=0.0;
   for(i=0;i<R->nstrati-1;i++){
    for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
     ptr=R->ptr_str[i]->neuro_el[j].next;
     while(ptr!=NULL){
      acc+=(ptr->W=casual());
      ptr=ptr->next;
     }
     acc+=R->ptr_str[i]->neuro_el[j].bias;
    }
   }
   printf("bilanciamento pesi nell'intervallo 0<x<1.0, sommatoria =%g \n",acc);
  }while(acc<0.1 || acc>0.5);*/
  //system("pause");
 }else{
/*creo le sinapsi con i valori letti da file se ci sono*/
  for(i=0;i<R->nstrati-1;i++){
   CreaSinapsi_file(R->ptr_str[i],R->ptr_str[i+1],struttura->pesi[i]);
  }
 }
 return R;
}

Rete CreaArchitettura_default(Retetrg struttura2){
/*inizializzo il percettrone con la struttura a fisarmonica*/
 int dim_out,dim_out1,i;//,j;
 Rete R;
// SinapsiLink ptr;
 tipopesoSinapsi acc=0.0;
 R=InitReteN(5,400000,(tipoFattori)0.5,(tipoFattori)0.1);
 if(struttura2->dim_out==1)dim_out=2;
 else dim_out=struttura2->dim_out;
 if(struttura2->dim[0]==1)dim_out1=2;
 else dim_out1=struttura2->dim[0];
 R->ptr_str[0]=InitStrato(struttura2->dim[0],'i',0,sigm_uni);
 R->ptr_str[1]=InitStrato(quadrato(struttura2->dim[0] ),'h',1,sigm_uni);
 R->ptr_str[2]=InitStrato(dim_out1 * dim_out ,'h',2,sigm_uni);
 R->ptr_str[3]=InitStrato(quadrato(struttura2->dim_out) ,'h',3,sigm_uni);
 R->ptr_str[4]=InitStrato(struttura2->dim_out,'o',4,sigm_uni);
/*creo le sinapsi con i valori casuali */
 for(i=0;i<R->nstrati-1;i++){
  CreaSinapsi(R->ptr_str[i],R->ptr_str[i+1]);
 }
 /*do{
  acc=0.0;
  for(i=0;i<R->nstrati-1;i++){
   for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
	   ptr=R->ptr_str[i]->neuro_el[j].next;
	   while(ptr!=NULL){
	    acc+=(ptr->W=casual());
	    ptr=ptr->next;
	   }
  if(i!=0)	acc+=R->ptr_str[i]->neuro_el[j].bias;
   }
  }
  printf("bilanciamento pesi nell'intervallo 0<x<1.0, sommatoria =%g \n",acc);
 }while(acc<0.05 || acc>0.1);*/
 return R;
}

/*funzione per inizializzare uno strato*/
Strato InitStrato(int Num_neuroni,char tipo,int posizione,tipoNeurone (*funz_att)(tipoNeurone val_x)){
 int i;
 Strato neuroStr=(Strato)nedmalloc(sizeof(*neuroStr));
 neuroStr->num_neuro_el =Num_neuroni;
 neuroStr->tipo_strato=tipo;
 neuroStr->num_strato=posizione;
 neuroStr->neuro_el=nedcalloc( Num_neuroni, sizeof(Neurone));
 for(i=0; i<Num_neuroni; i++) {
  neuroStr->neuro_el[i].funz_attivazione=nedmalloc(sizeof(funz_att));
  neuroStr->neuro_el[i].funz_attivazione=funz_att;
  neuroStr->neuro_el[i].next	     =NULL;
  neuroStr->neuro_el[i].inn       =NULL;
  neuroStr->neuro_el[i].coda      =NULL;
  neuroStr->neuro_el[i].coda2     =NULL;
  neuroStr->neuro_el[i].E         =(tipoDeltaVal  )0.0;
  neuroStr->neuro_el[i].soglia    =(tipoNeurone)0.0;
  neuroStr->neuro_el[i].val_x     =(tipoNeurone)0.0;
  neuroStr->neuro_el[i].bias      =(tipopesoSinapsi)0.0;
  neuroStr->neuro_el[i].precWbias =(tipopesoSinapsi)0.0;
  neuroStr->neuro_el[i].bias_D    =(tipoDeltaVal)0.0;
  neuroStr->neuro_el[i].bias_precD=(tipoDeltaVal)0.0;
  neuroStr->neuro_el[i].bias_delta=(tipoDeltaVal)0.1;
 }
 return neuroStr;
}
/*funzione per liberare memoria occupata dallo strato di neuroni*/
/*
Strato freeStrato(Strato neuroStr){ 
  int i;
  SinapsiLink ptr;
  
  for (i = 0; i < neuroStr->num_neuro_el; i++) {  //elimina sinapsi input
    while ((neuroStr->neuro_el)[i].next!= NULL) {
      ptr=neuroStr->neuro_el[i].next;
      neuroStr->neuro_el[i].next=(neuroStr->neuro_el)[i].next->next;
      free(ptr);
	}}
  free(neuroStr-> neuro_el);  
  free(neuroStr);
  return NULL;
}*/

/*funzione ricorsiva che verifica se esiste una sinapsi tra 2 neuroni*/
/*int EsisteSinapsi(SinapsiLink head, int Nn , int Ns){
  if (head == NULL) return 0; 
   else 
	if (head->N == Nn && head->S == Ns ) return 1; 
	 else
      return EsisteSinapsi(head->next, Nn, Ns);
}*/

/*funzione che verifica se esiste un determinato neurone nell' insieme di output*/
/*int EsisteNeuroneOutput(Strato neuroStr, int Nj){
	if ( Nj >= neuroStr->num_neuro_el) return 0;
	 else 
	  return 1;
}*/

/*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
void Insertsinapsi(Strato neuroStr1,Strato neuroStr2, int Ni, int Nj, tipopesoSinapsi x){
 SinapsiLink     head;
 BackLinks   backhead;
 //inserisco la sinapsi
 head			    = nedmalloc(sizeof(*head));
 head->N		  = neuroStr2->neuro_el+Nj; //neurone in cui finisco
 head->S		  = neuroStr2->num_strato;
 head->W		  = x;
 head->Ni		 = Nj;
 head->precW= (tipopesoSinapsi)0.0;
 head->D		  = (tipoDeltaVal)0.0;
 head->precD= (tipoDeltaVal)0.0;
 head->delta= (tipoDeltaVal)0.1;
 head->next	= NULL;
 if(neuroStr1->neuro_el[Ni].next==NULL){
  neuroStr1->neuro_el[Ni].next=neuroStr1->neuro_el[Ni].coda=head;
 }else{
  neuroStr1->neuro_el[Ni].coda->next=head;
  neuroStr1->neuro_el[Ni].coda=neuroStr1->neuro_el[Ni].coda->next;
 }
 //inserisco il backlink
 if(neuroStr1->tipo_strato!='o'){
  backhead       = nedmalloc(sizeof(*backhead));
  backhead->N    = neuroStr1->neuro_el+Ni; //neurone da cui provengo
  backhead->Ni   = Ni;
  backhead->S    = neuroStr1->num_strato;
  backhead->back = head;
  backhead->next = NULL;
  if(neuroStr2->neuro_el[Nj].inn==NULL){
   neuroStr2->neuro_el[Nj].inn=neuroStr2->neuro_el[Nj].coda2=backhead;
  }else{
   neuroStr2->neuro_el[Nj].coda2->next=backhead;
   neuroStr2->neuro_el[Nj].coda2=neuroStr2->neuro_el[Nj].coda2->next;
  }
 }
}

/*funzione che crea la maglia di sinapsi tra i neuroni di input e quelli di output*/
/*(per il momento tutti i neuroni di input vengono collegati con quelli di output*/
/* ed ogni peso viene inizializzato con il valore casuale)*/
void CreaSinapsi(Strato neuroStr1,Strato neuroStr2){
 int i,j;
 for(i=0;i<neuroStr1->num_neuro_el;i++){
  for(j=0;j<neuroStr2->num_neuro_el;j++){
  	Insertsinapsi(neuroStr1,neuroStr2,i,j,casual());
	  neuroStr2->neuro_el[j].bias=casual();
  }
  //neuroStr1->neuro_el[i].bias=casual();
 }
}

/*funzione che crea la maglia di sinapsi tra i neuroni di input e quelli di output*/
/*(per il momento tutti i neuroni di input vengono collegati con quelli di output*/
/* ed ogni peso viene inizializzato con il valore letto da file)*/
void CreaSinapsi_file(Strato neuroStr1,Strato neuroStr2,tipopesoSinapsi *pesi){
 int i,j;
/*assegno i pesi dallo strato i allo strato i+1*/
 for(i=0;i<neuroStr1->num_neuro_el;i++){
  for(j=0;j<neuroStr2->num_neuro_el;j++){
   Insertsinapsi(neuroStr1,neuroStr2,i,j,pesi[(i*neuroStr2->num_neuro_el)+j]);
  }
 }
/*assegno i bias dello strato i+1*/
 for(j=0;j<neuroStr2->num_neuro_el;j++){
  neuroStr2->neuro_el[j].bias=pesi[(i*(neuroStr2->num_neuro_el))+(j)];
 }
}

Rete Crea_Architettura_CERCHI(){
 int i,j,k,h/*,dim_out,dim_out1*/;
/*inizializzo il percettrone con la struttura a fisarmonica*/
 Rete R;
// SinapsiLink ptr;
 tipopesoSinapsi acc=0.0;
 R=InitReteN(3,400000,(tipoFattori)0.5,(tipoFattori)0.1);
 R->ptr_str[0]=InitStrato(256,'i',0,sigm_uni);
 //R->ptr_str[1]=InitStrato(384,'h',1,sigm_uni);
 R->ptr_str[1]=InitStrato(146,'h',1,sigm_uni);
	//R->ptr_str[2]=InitStrato(128,'h',2,sigm_uni);
	R->ptr_str[2]=InitStrato(1  ,'o',2,sigm_uni);
	//R->ptr_str[2]=InitStrato(144 ,'h',2,sigm_uni);
 //R->ptr_str[3]=InitStrato(7  ,'o',3,sigm_uni);
/*creo le sinapsi con i valori casuali */

	for(k=0;k<2;k++){
	 for(i=0;i<2;i++){
		 for(j=0;j<8;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + (j * 16) + (i * 8) + (k * 128) ,i + 2 * k  , casual()); 
	 	}
  }
	}
	
	for(k=0;k<4;k++){
	 for(i=0;i<4;i++){
		 for(j=0;j<4;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (j * 16) + (i * 4) + (k * 64) ,i + 4 * k +4 , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (j * 16) + (i * 4) + (k * 64) ,i + 4 * k +4 , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (j * 16) + (i * 4) + (k * 64) ,i + 4 * k +4 , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (j * 16) + (i * 4) + (k * 64) ,i + 4 * k +4 , casual()); 
			}
  }
	}

	 for(i=0;i<4;i++){
		 for(j=0;j<4;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    ((j + (i*4)) * 16),i +20  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + ((j + (i*4)) * 16),i +20  , casual());
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],8 + ((j + (i*4)) * 16),i +20  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],9 + ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],10+ ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],11+ ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],12+ ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],13+ ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],14+ ((j + (i*4)) * 16),i +20  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],15+ ((j + (i*4)) * 16),i +20  , casual());
			}
  }
	
		for(i=0;i<4;i++){
		 for(j=0;j<16;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (i*4) + (j*16),i +24  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (i*4) + (j*16),i +24  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (i*4) + (j*16),i +24  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (i*4) + (j*16),i +24  , casual());
			}
  }

	for(k=0;k<8;k++){
	 for(i=0;i<8;i++){
		 for(j=0;j<2;j++){
		 	Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (j * 16) + (i * 2) + (k * 32) ,i + 8 * k +28  , casual());
		 	Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (j * 16) + (i * 2) + (k * 32) ,i + 8 * k +28  , casual());
			}
		}
	}

	/*
	for(k=0;k<256;k++){
		Insertsinapsi(R->ptr_str[0],R->ptr_str[1], k ,k+92 , casual());
	}
	*/

	for(i=0;i<8;i++){
	 for(j=0;j<16;j++){
 /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (i*2) + (j*16),i +92  , casual());
	  Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (i*2) + (j*16),i +92  , casual());
		}
 }

	 for(i=0;i<8;i++){
		 for(j=0;j<2;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    ((j + (i*2)) * 16),i +100  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + ((j + (i*2)) * 16),i +100  , casual());
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],8 + ((j + (i*2)) * 16),i +100  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],9 + ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],10+ ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],11+ ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],12+ ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],13+ ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],14+ ((j + (i*2)) * 16),i +100  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],15+ ((j + (i*2)) * 16),i +100  , casual());
			}
  }
	
		//+2
	for(k=0;k<2;k++){
	 for(i=0;i<1;i++){
		 for(j=0;j<8;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + (j * 16) + (i * 8) + (k * 128) + 4 ,i + 2 * k + 108  , casual()); 
	 	}
  }
	}
//+12
 for(k=0;k<4;k++){
	 for(i=0;i<3;i++){
		 for(j=0;j<4;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (j * 16) + (i * 4) + (k * 64) + 2 ,i + 4 * k +110  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (j * 16) + (i * 4) + (k * 64) + 2 ,i + 4 * k +110  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (j * 16) + (i * 4) + (k * 64) + 2 ,i + 4 * k +110  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (j * 16) + (i * 4) + (k * 64) + 2 ,i + 4 * k +110  , casual()); 
			}
  }
	}

  for(i=0;i<3;i++){
		 for(j=0;j<4;j++){
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    ((j + 2 + (i*4)) * 16),i +122  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + ((j + 2 + (i*4)) * 16),i +122  , casual());
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],8 + ((j + 2 + (i*4)) * 16),i +122  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],9 + ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],10+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],11+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],12+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],13+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],14+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			 Insertsinapsi(R->ptr_str[0],R->ptr_str[1],15+ ((j + 2 + (i*4)) * 16),i +122  , casual());
			}
  }

		for(i=0;i<3;i++){
		 for(j=0;j<16;j++){
    /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    (i*4) + (j*16) + 2,i +125  , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + (i*4) + (j*16) + 2,i +125  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + (i*4) + (j*16) + 2,i +125  , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + (i*4) + (j*16) + 2,i +125  , casual());
			}
  }

		for(h=0;h<16;h++){
		      /*funzione che crea una sinapsi sul neurone Ni che va nel neurone Nj*/
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],    h * 16 , 128 + h , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],1 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],2 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],3 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],4 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],5 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],6 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],7 + h * 16 , 128 + h , casual());
    Insertsinapsi(R->ptr_str[0],R->ptr_str[1],8 + h * 16 , 128 + h , casual());
	   Insertsinapsi(R->ptr_str[0],R->ptr_str[1],9 + h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],10+ h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],11+ h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],12+ h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],13+ h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],14+ h * 16 , 128 + h , casual());
				Insertsinapsi(R->ptr_str[0],R->ptr_str[1],15+ h * 16 , 128 + h , casual());
	  }
		

		for(h=0;h<2;h++){
	  for(k=0;k<256;k++){
		  Insertsinapsi(R->ptr_str[0],R->ptr_str[1], k ,144 + h, casual());
	  }
		}



  //inzializzo bias primo strato
  //for(i=0;i<384;i++){
	for(i=0;i<146;i++){
			R->ptr_str[1]->neuro_el[i].bias=casual();
		}
/*
  for(i=0;i<4;i++){
			for(j=0;j<12;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=4;i<20;i++){
			for(j=12;j<24;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=20;i<24;i++){
			for(j=24;j<36;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

  for(i=24;i<28;i++){
			for(j=36;j<48;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=28;i<92;i++){
			for(j=48;j<60;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=92;i<348;i++){
			for(j=60;j<72;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

  for(i=348;i<356;i++){
			for(j=72;j<84;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

  for(i=356;i<364;i++){
			for(j=84;j<96;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=364;i<366;i++){
			for(j=96;j<108;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

  for(i=366;i<378;i++){
			for(j=108;j<120;j++){
			Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=378;i<381;i++){
			for(j=120;j<132;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		for(i=381;i<384;i++){
			for(j=132;j<144;j++){
			 Insertsinapsi(R->ptr_str[1],R->ptr_str[2],i,j,casual());
			}
		}

		//inzializzo bias secondo strato
  for(i=0;i<144;i++){
			R->ptr_str[2]->neuro_el[i].bias=casual();
		}

		CreaSinapsi(R->ptr_str[2],R->ptr_str[3]);
*/
  CreaSinapsi(R->ptr_str[1],R->ptr_str[2]);
		//CreaSinapsi(R->ptr_str[2],R->ptr_str[3]);
 /*do{
  acc=0.0;
  for(i=0;i<R->nstrati-1;i++){
   for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
	   ptr=R->ptr_str[i]->neuro_el[j].next;
	   while(ptr!=NULL){
	    acc+=(ptr->W=casual());
	    ptr=ptr->next;
	   }
  if(i!=0)	acc+=R->ptr_str[i]->neuro_el[j].bias;
   }
   //if(acc<0.0 || acc>0.3){break;acc=2.0;}
  }
  printf("bilanciamento pesi nell'intervallo 0<x<1.0, sommatoria =%g \n",acc);
 }while(acc<0.0 || acc>1.0);*/
 //system("pause");
 return R;

}

Retestr get_info_rete(Rete R){
 int i;
 Retestr struttura=nedcalloc(1,sizeof(retestr));//preparo la struttura
 struttura->dim=nedcalloc(R->nstrati,sizeof(int));
 struttura->ndim=R->nstrati;
 for(i=0;i<R->nstrati;i++)struttura->dim[i]=R->ptr_str[i]->num_neuro_el;
 struttura->pesi=NULL;
 return struttura;
}

int get_continue(){
 char c[100];
	int d=1;
 while(1){
		printf("\n\n**Vuoi apprendere il TS?\n[0]no\n[1]si\n");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  d=atoi(c);
  fflush(stdin);
  if(d==1||d==2)break;
  printf("\n**scegli un valore tra 1 e 2**");
 }
	printf("\n");
	return d;
}

int get_modo_func(){
 char c[100];
	int d=1;
 while(1){
		printf("\n**scegli:**\n[1]TESTING\n[2]APPRENDIMENTO\n");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  d=atoi(c);
  fflush(stdin);
  if(d==1||d==2)break;
  printf("\n**scegli un valore tra 1 e 2**");
 }
	printf("\n");
	return d;
}

int get_modo_app(){
 char c[100];
	int d=1;
 while(1){
		printf("\n**inserisci modo apprendimento**\n[1]IRPROP-\n[2]BACKPROP\n");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  d=atoi(c);
  fflush(stdin);
  if(d==1||d==2)break;
  printf("\n**scegli un valore tra 1 e 2**");
 }
	return d;
}

void get_momento_fapp(Rete R){
 char c[100],*b;
 while(1){
  printf("\n**inserisci valore per il fattore di apprendimento:");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  R->Fapp=(tipoFattori)strtod(c,&b);
  fflush(stdin);
  if(R->Fapp==0.0)continue;
  if(R->Fapp<0.0){
   printf("\n**fattore di apprendimento negativo non accettato**");
   continue;
  }
  if(R->Fapp>2.0){
   printf("\n**fattore di apprendimento consigliato <2**");
   continue;
  }
  break;
 }
 while(1){
  printf("\n**inserisci valore per il fattore momento:");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  R->momentum=(tipoFattori)strtod(c,&b);
  fflush(stdin);
  if(R->momentum==0.0)continue;
  if(R->momentum<0.0){
   printf("\n**fattore momento negativo non accettato**");
   continue;
  }
  if(R->momentum>2.0){
   printf("\n**fattore momento consigliato <2**");
   continue;
  }
  break;
 }
}

void get_mse(Rete R){
 char c[100],*b;
 while(1){
  printf("\n**inserisci mse per minimo per fermare l' apprendimento**");
  memset(c,0,100);
  scanf_s("%99s",c,100);
  R->precisione=(tipoFattori)strtod(c,&b);
  fflush(stdin);
  if(R->precisione==0.0)continue;
  if(R->precisione<0.0){
   printf("\n**consigliato un mse positivo**");
   continue;
  }
  break;
 }
}

void agg_input(Strato ptr_str,tipoNeurone * vals){
 int i;
 for(i=0;i<ptr_str->num_neuro_el;i++){
  ptr_str->neuro_el[i].val_x=vals[i];
  ptr_str->neuro_el[i].soglia= vals[i];
 }
}

void propagavals(Rete R){
 int i,j,k=0;/*,mod,mod1,mod2=2*/
	tipoNeurone max_val_x;

 for(j=1;j<R->nstrati;j++){
  for(i=0;i<R->ptr_str[j]->num_neuro_el;i++){
   R->ptr_str[j]->neuro_el[i].val_x  = potential(R->ptr_str[j]->neuro_el+i);
			R->ptr_str[j]->neuro_el[i].val_x+=R->ptr_str[j]->neuro_el[i].bias;
			R->ptr_str[j]->neuro_el[i].val_x*=0.5;
				max_val_x = 150/0.5;
			if(R->ptr_str[j]->neuro_el[i].val_x > max_val_x)R->ptr_str[j]->neuro_el[i].val_x = max_val_x;
			else if(R->ptr_str[j]->neuro_el[i].val_x < -max_val_x)R->ptr_str[j]->neuro_el[i].val_x = -max_val_x;
   R->ptr_str[j]->neuro_el[i].soglia = sigm_uni(R->ptr_str[j]->neuro_el[i].val_x);
  }  
		//for(j=1;j<R->nstrati;j++){
  // k=0;
		//	mod2=5;
		//	mod =R->ptr_str[j]->num_neuro_el % 5;
  // mod1=R->ptr_str[j]->num_neuro_el - mod;
		//	while(k<=mod1){
		//		if(k==mod1){
  //   mod2=mod;
		//			if(!mod2)break;
		//		}
		//	 //for(i=0;i<mod2;i++){
		//		if(mod2>0){ 
		//		 //local[0]->i=0+k;
		//	  //local[0]->j=j;
		//	  //local[0]->R=R;
		//	  //pot[0]=(HANDLE)	_beginthreadex ( NULL,0,&potential_T,local[0],0,NULL);
		//	  R->ptr_str[j]->neuro_el[0+k].val_x  = potential(R->ptr_str[j]->neuro_el+0+k);
  //   R->ptr_str[j]->neuro_el[0+k].soglia = sigm_uni(R->ptr_str[j]->neuro_el[0+k].val_x+R->ptr_str[j]->neuro_el[0+k].bias);
		//		}
		//		if(mod2>1){ 
		//		 //local[1]->i=1+k;
		//	  //local[1]->j=j;
		//	  //local[1]->R=R;
		//	  //pot[1]=(HANDLE)	_beginthreadex ( NULL,0,&potential_T,local[1],0,NULL);
		//			R->ptr_str[j]->neuro_el[1+k].val_x  = potential(R->ptr_str[j]->neuro_el+1+k);
  //   R->ptr_str[j]->neuro_el[1+k].soglia = sigm_uni(R->ptr_str[j]->neuro_el[1+k].val_x+R->ptr_str[j]->neuro_el[1+k].bias);
		//		}
		//		if(mod2>2){ 
		//		 //local[1]->i=1+k;
		//	  //local[1]->j=j;
		//	  //local[1]->R=R;
		//	  //pot[1]=(HANDLE)	_beginthreadex ( NULL,0,&potential_T,local[1],0,NULL);
		//			R->ptr_str[j]->neuro_el[2+k].val_x  = potential(R->ptr_str[j]->neuro_el+2+k);
  //   R->ptr_str[j]->neuro_el[2+k].soglia = sigm_uni(R->ptr_str[j]->neuro_el[2+k].val_x+R->ptr_str[j]->neuro_el[2+k].bias);
		//		}
		//		if(mod2>3){ 
		//		 //local[1]->i=1+k;
		//	  //local[1]->j=j;
		//	  //local[1]->R=R;
		//	  //pot[1]=(HANDLE)	_beginthreadex ( NULL,0,&potential_T,local[1],0,NULL);
		//			R->ptr_str[j]->neuro_el[3+k].val_x  = potential(R->ptr_str[j]->neuro_el+3+k);
  //   R->ptr_str[j]->neuro_el[3+k].soglia = sigm_uni(R->ptr_str[j]->neuro_el[3+k].val_x+R->ptr_str[j]->neuro_el[3+k].bias);
		//		}
		//		if(mod2>4){ 
		//		 //local[1]->i=1+k;
		//	  //local[1]->j=j;
		//	  //local[1]->R=R;
		//	  //pot[1]=(HANDLE)	_beginthreadex ( NULL,0,&potential_T,local[1],0,NULL);
		//			R->ptr_str[j]->neuro_el[4+k].val_x  = potential(R->ptr_str[j]->neuro_el+4+k);
  //   R->ptr_str[j]->neuro_el[4+k].soglia = sigm_uni(R->ptr_str[j]->neuro_el[4+k].val_x+R->ptr_str[j]->neuro_el[4+k].bias);
		//		}
		//		//}
  // //WaitForMultipleObjects(mod2,pot,TRUE,INFINITE);
		//	//system("pause");
		//	k+=5;
		//}
	}
}

tipoNeurone potential(Neuroni ptr1){
 tipoNeurone out_val=(tipoNeurone)0.0;
 BackLinks ptr2;
 //int i=0;
 /* Per ogni sinapsi in ingresso al neurone...*/
 ptr2=ptr1->inn;
 while(ptr2!=NULL){
  out_val+= ptr2->N->soglia * ptr2->back->W;
  ptr2=ptr2->next;
 }
 return out_val;
}

unsigned __stdcall potential_T ( void *pmydata){
	MYDATA local = (MYDATA)pmydata;
	int i=local->i,j=local->j;
	tipoNeurone out_val=(tipoNeurone)0.0;
	BackLinks ptr2;
 /* Per ogni sinapsi in ingresso al neurone...*/
	ptr2=local->R->ptr_str[j]->neuro_el[i].inn;
 while(ptr2!=NULL){
  out_val+= ptr2->N->soglia * ptr2->back->W;
  ptr2=ptr2->next;
 }
 local->R->ptr_str[j]->neuro_el[i].soglia = sigm_uni(out_val + local->R->ptr_str[j]->neuro_el[i].bias);
	return 0;
}

tipoNeurone derivata_sigm(tipoNeurone rete_out){
 tipoNeurone approx;//=rete_out;
 approx=taglio(rete_out,(tipoNeurone)0.01,(tipoNeurone)0.99);
 return ( (tipoNeurone)2.0 *approx * (tipoNeurone)0.5 * (1 - approx) );
}

tipoNeurone delta_out(tipoNeurone rete_out,tipoNeurone out_ts){
 return  (out_ts - rete_out) * derivata_sigm( rete_out);
}

void delta_outs(Rete R,Retetrg struttura2,int occ){
 int i;
 for(i=0;i<R->ptr_str[R->nstrati-1]->num_neuro_el;i++){
  R->ptr_str[R->nstrati-1]->neuro_el[i].E =delta_out(R->ptr_str[R->nstrati-1]->neuro_el[i].soglia,struttura2->vals[occ][(struttura2->dim[0])+i]);
 }
}

void propagaerrors(Rete R){
 int i,j;//k;
 tipoDeltaVal sum;
 SinapsiLink ptr1;
 for(i=(R->nstrati-2);i>0;i--){
		for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
   ptr1=R->ptr_str[i]->neuro_el[j].next;
   sum=0.0;
   while(ptr1!=NULL){	    //sommatoria produttorie
    sum+= ptr1->N->E * ptr1->W;
    ptr1=ptr1->next;
   }
   R->ptr_str[i]->neuro_el[j].E= derivata_sigm(R->ptr_str[i]->neuro_el[j].soglia) * sum;
  }
 }
}

void aggiorna_pesi_fapp_fissa(Rete R){
 int i,j;
 BackLinks ptr2;
 tipopesoSinapsi delta,delta_w;
 for(i=1;i<R->nstrati;i++){
  for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
   delta=R->ptr_str[i]->neuro_el[j].E * R->Fapp;  
			ptr2=R->ptr_str[i]->neuro_el[j].inn;
			while(ptr2!=NULL){  //sommatoria produttorie
				delta_w= (delta * ptr2->N->soglia) + (R->momentum * ptr2->back->precW);
				ptr2->back->W+=delta_w;
    ptr2->back->precW=delta_w;
				ptr2=ptr2->next; 
   }
			delta_w= delta + (R->momentum * R->ptr_str[i]->neuro_el[j].precWbias);
   R->ptr_str[i]->neuro_el[j].bias+= delta_w;
			R->ptr_str[i]->neuro_el[j].precWbias= delta_w;
  }
 }
}

void updategradients(Rete R){
 int i,j;
 BackLinks ptr2;
 for(i=1;i<R->nstrati;i++){
  for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
   ptr2= R->ptr_str[i]->neuro_el[j].inn;
   while(ptr2!=NULL){  //sommatoria produttorie
    ptr2->back->D+= R->ptr_str[i]->neuro_el[j].E * ptr2->N->soglia;
    ptr2= ptr2->next; 
   }
   R->ptr_str[i]->neuro_el[j].bias_D+=R->ptr_str[i]->neuro_el[j].E;//si presuppone che il bias ha soglia uno
  }
 }
}

void aggiorna_pesi_rprop(Rete R){
 int i,j;
 tipoDeltaVal grad;
 BackLinks ptr2;
 for(i=1;i<R->nstrati;i++){
  for(j=0;j<R->ptr_str[i]->num_neuro_el;j++){
   ptr2= R->ptr_str[i]->neuro_el[j].inn;
   while(ptr2!=NULL){  //sommatoria produttorie
    ptr2->back->delta = max( ptr2->back->delta,(tipoNeurone)0.001); 
	   grad= ptr2->back->precD * ptr2->back->D;
    if(grad>0.0){ 
     ptr2->back->delta= min( ptr2->back->delta * R->Fapp_pos , R->delta_Fapp_max);
    }
				else if(grad<0.0){
	    ptr2->back->delta= max( ptr2->back->delta * R->Fapp_neg , R->delta_Fapp_min);
	    ptr2->back->D=0.0;
	   }
	   else{
	    ptr2->back->delta=0.0;
	   }
	   if(ptr2->back->D <0.0)ptr2->back->W -=ptr2->back->delta;
	   else ptr2->back->W +=ptr2->back->delta;
    ptr2->back->precD= ptr2->back->D;
	   ptr2->back->D=0.0;
    ptr2= ptr2->next; 
   }
	//aggiorno i bias tecnica irProp-
   R->ptr_str[i]->neuro_el[j].bias_delta = max( R->ptr_str[i]->neuro_el[j].bias_delta,(tipoNeurone)0.001); 
   grad= R->ptr_str[i]->neuro_el[j].bias_D * R->ptr_str[i]->neuro_el[j].bias_precD;
   if(grad>0.0){ 
    R->ptr_str[i]->neuro_el[j].bias_delta= min( R->ptr_str[i]->neuro_el[j].bias_delta * R->Fapp_pos , R->delta_Fapp_max);
   }
   else if(grad<0.0){
    R->ptr_str[i]->neuro_el[j].bias_delta= max( R->ptr_str[i]->neuro_el[j].bias_delta * R->Fapp_neg , R->delta_Fapp_min);
    R->ptr_str[i]->neuro_el[j].bias_D=0.0;
   }
   else{
    R->ptr_str[i]->neuro_el[j].bias_delta=0.0;
   }
   if(R->ptr_str[i]->neuro_el[j].bias_D <0)R->ptr_str[i]->neuro_el[j].bias -=R->ptr_str[i]->neuro_el[j].bias_delta;
   else R->ptr_str[i]->neuro_el[j].bias +=R->ptr_str[i]->neuro_el[j].bias_delta;
   R->ptr_str[i]->neuro_el[j].bias_precD= R->ptr_str[i]->neuro_el[j].bias_D;
   R->ptr_str[i]->neuro_el[j].bias_D=0.0;
  }
 }
}

 tipoNeurone err_quad(Strato ptr_str,tipoNeurone * vals,int dim){
 int i;
 tipoNeurone acc=(tipoNeurone)0.0;
 tipoNeurone app=(tipoNeurone)0.0;
 for(i=0;i<ptr_str->num_neuro_el;i++){
  app=(vals[dim+i]-ptr_str->neuro_el[i].soglia);
  acc+=(tipoNeurone)pow(app,2);
 }
 return acc;
}

tipoNeurone ** funzionamento(Rete R,Retestr struttura,Retetrg struttura2){
 int i,k,dim=1;
 tipoNeurone ** out;
 out=nedmalloc(struttura2->npatt*sizeof(tipoNeurone*));
 for(i=0;i<struttura2->npatt;i++){ 
  out[i] = nedmalloc(struttura->dim[struttura->ndim-1] * sizeof(tipoNeurone));
 }
 /*eseguo funzionamento*/
 for(k=0;k<struttura2->npatt;k++){
  agg_input(R->ptr_str[0],struttura2->vals[k]);
  propagavals(R);
  /*salvo valore ottenuto dalla computazione*/
  for(i=0;i<R->ptr_str[R->nstrati-1]->num_neuro_el;i++){
   out[k][i]=R->ptr_str[R->nstrati-1]->neuro_el[i].soglia;
  }
		struttura2->mse+=err_quad(R->ptr_str[R->nstrati-1],struttura2->vals[k],struttura2->dim[0]);
 }
 struttura2->dim_out=struttura->dim[struttura->ndim-1];
 /*fine funzionamento*/
 return out;
}

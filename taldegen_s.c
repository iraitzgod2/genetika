/* 
    KA - Lan praktikoa  -- OpenMP
    taldegen_s.c     SERIEKO BERTSIOA

*************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "definetg.h"
#include "funtg.h"


float   elem[EMAX][ALDAKOP];	// prozesatu behar diren elementuak
int     nor[TALDEKOP][EMAX];    // talde bakoitzeko kideen zerrenda



// programa nagusia
// ================================

void main (int argc, char *argv[])
{
  float   zent[TALDEKOP][ALDAKOP], zentberri[TALDEKOP][ALDAKOP];   // kalkulatzen diren zentroideak (taldeak)
  double  baturak[TALDEKOP][ALDAKOP+1];
  float   trinko[TALDEKOP];					   // talde bakoitzeko trinkotasuna

  int     i, j, elekop, taldea;
  int     popul[EMAX], tkop[TALDEKOP];	// popul: elementu bakoitzeko taldea;  tkop: taldeen tamaina
  int     bukatu = 0, iterkop = 0; 
  float   diszent;

  FILE    *f1, *f2;
  struct timespec  t1, t2;
  double  texe;


  if ((argc < 2)  || (argc > 3)) {
    printf ("ADI:  progr datu-fitx [elem kop])\n");
    exit (-1);
  }


  printf ("\n >> Exekuzioa seriean\n");
  clock_gettime (CLOCK_REALTIME, &t1);

  
  // irakurri datuak fitxategitik: elem[i][j]
  // ========================================
  
  f1 = fopen (argv[1], "r");
  if (f1 == NULL) {
    printf ("Errorea %s fitxategia irekitzean\n", argv[1]);
    fclose (f1);
    exit (-1);
  }

  fscanf (f1, "%d", &elekop);
  if (argc == 3) elekop = atoi(argv[2]);

  for (i=0; i<elekop; i++) 
  for (j=0; j<ALDAKOP; j++) 
    fscanf (f1, "%f", &(elem[i][j]));
  
  fclose (f1);


  // aukeratu lehen zentroideak, ausaz
  // ===================================

  srand (147);
  for (i=0; i<TALDEKOP; i++) 
  for (j=0; j<ALDAKOP; j++) 
    zent[i][j] = (rand() % 10000) / 100.0; 



  // 1. fasea: kalkulatu zentroideak eta sailkatu elementuak
  // =========================================================

  iterkop = 0; bukatu = 0;
  while ((bukatu == 0) && (iterkop < ITMAX))
  {
    // kalkulatu talde gertuena (OSATZEKO) 
    talde_gertuena (elekop, elem, zent, popul);


    // kalkulatu talde bakoitzeko zentroide berriak
    // dimentsio bakoitzaren batazbestea
    // baturak: 100 aldagaien balioak akumulatzeko; azkena kopurua da
    for (i=0; i<TALDEKOP; i++)
    for (j=0; j<ALDAKOP+1; j++) 
      baturak[i][j] = 0.0;

    for (i=0; i<elekop; i++)
    {
      for (j=0; j<ALDAKOP; j++) 
        baturak[popul[i]][j] += elem[i][j];
      baturak[popul[i]][ALDAKOP] ++;
    }


    // kalkulatu zentroide berriak eta erabaki bukatu den edo jarraitu behar den, DELTAren arabera
    bukatu = 1;
    for (i=0; i<TALDEKOP; i++) 
    {
      if (baturak[i][ALDAKOP] > 0) // taldea ez dago hutsik
      {
        for (j=0; j<ALDAKOP; j++) zentberri[i][j] = baturak[i][j] / baturak[i][ALDAKOP];    
      
        // erabaki bukatu behar den
        diszent = dis_gen (&zentberri[i][0], &zent[i][0]);
        if (diszent > DELTA) bukatu = 0;	// dimentsio batean aldaketa dago; segi smulazioarekin

        // kopiatu zentroide berriak
        for (j=0; j<ALDAKOP; j++) zent[i][j] = zentberri[i][j];    
      }
    }

    iterkop ++;
  } // while


  
  // 2. fasea: kontatu populazio bakoitzaren elementuen kopurua eta kalkulatu talden "trinkotasuna"
  // ===============================================================================================

  for (i=0; i<TALDEKOP; i++) tkop[i] = 0;

  // elementuen kopurua eta sailkapena
  for (i=0; i<elekop; i++) 
  {
    taldea = popul[i];
    nor[taldea][tkop[taldea]] = i;
    tkop[taldea] ++; 
  }

  // trinkotasuna talde bakoitzean: elementuen arteko distantzien batezbestekoa (OSATZEKO)
  trinkotasuna (tkop, elem, nor, trinko);


  // idatzi emaitza batzuk fitxategi batean
  // ========================================
  
  f2 = fopen ("dbh_irt", "w");
  if (f2 == NULL) {
    printf ("Errorea %s fitxategia irekitzean\n", argv[1]);
    fclose (f2);
    exit (-1);
  }
  
  // zentroideak, kopurua eta trinkotasuna
  for (i=0; i<TALDEKOP; i++) {
    for (j=0; j<ALDAKOP; j++) fprintf (f2, "%7.3f", zentberri[i][j]);
    fprintf (f2, "\n %d  %f \n", tkop[i], trinko[i]);
  }
  
  fclose (f2);


  clock_gettime (CLOCK_REALTIME, &t2);
  texe = (t2.tv_sec-t1.tv_sec) + (t2.tv_nsec-t1.tv_nsec)/(double)1e9;


  // inprimatu emaitza batzuk
  // ==============================

  printf ("\n  0, 40 eta 80 zentroideak eta haien trinkotasuna\n ");
  for (i=0; i<TALDEKOP; i+=40) {
    printf ("\n  z%2d -- ", i);
    for (j=0; j<ALDAKOP; j++) printf ("%5.1f", zent[i][j]);
    printf ("\n          %5.6f\n", trinko[i]);
  }

  printf ("\n >> Taldeen tamaina \n");
  for (i=0; i<10; i++) {
    for (j=0; j<10; j++) printf ("%7d", tkop[10*i+j]);
    printf("\n");
  }

  printf ("\n >> Iterazio kopurua: %d", iterkop);
  printf ("\n >> Tex (serie): %1.3f s\n\n", texe);
}


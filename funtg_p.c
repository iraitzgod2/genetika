/*
    KA - OpenmP
    funtg_p.c
    taldegen_p.c programan erabiltzen diren errutinak

    OSATZEKO
**************************************************************************************************/

#include <stdio.h>
#include <float.h>     
#include <limits.h>

#include <math.h>
#include "definetg.h"		// konstanteen definizioak

#include <omp.h>



/* 1 - Bi elementuen arteko distantzia genetikoa kalkulatzeko funtzioa (distantzia euklidearra)
       Sarrera:  20 aldagaiko bi elementu (erreferentziaz)
       Irteera:  distantzia (float)
***************************************************************************************************/

// EGITEKO
// kalkulatu bi elementuen arteko distantzia (euklidearra)
float dis_gen (float *zent, float *elem)
{
   float dis = 0.0; 
   int i;
   //#pragma omp parallel for
   for (i = 0; i < ALDAKOP; ++i)
      dis +=(float) pow((double)zent[i]-elem[i], 2.0);
   return sqrt(dis);
}





/* 2 - Talde gertuena kalkulatzeko funtzioa (zentroide gertuena)
   Sarrera:  elekop elementu kopurua, int
             elem   EMAX x ALDAKOP tamainako matrizea, erreferentziaz
             zent   TALDEKOP x ALDAKOP tamainako matrizea, erreferentziaz
   Irteera:  popul  EMAX tamainako bektorea, erreferentziaz, osagai bat elementuko, dagokion taldea
***************************************************************************************************/

// EGITEKO
// popul: elementu bakoitzaren zentroide hurbilena, haren "taldea"
void talde_gertuena (int elekop, float elem[][ALDAKOP], float zent[][ALDAKOP], int *popul)
{
   int ele,zen;
   float dis, dismin[elekop];
   int ktald, kelem, kalda, nth = omp_get_num_threads();
   ktald = TALDEKOP/nth;
  kalda = ALDAKOP/nth;
  kelem = elekop/nth;
   #pragma omp parallel for private(ele) schedule(static)
   for (ele = 0; ele < elekop; ele++)
      dismin[ele]=FLT_MAX;
   for (zen = 0; zen < TALDEKOP; zen++)
   {
      #pragma omp parallel for private(ele,dis) schedule(static) 
      for (ele = 0; ele < elekop; ele++)
      {
         dis = dis_gen(elem[ele], zent[zen]);
         if (dis < dismin[ele])
         {
            dismin[ele] = dis;
            popul[ele] = zen;
         }
      }
   }
}





/* 3 - Taldeen trinkotasuna kalkulatzeko funtzioa (kideen arteko distantzia, b.b.) 
   Sarrera:  tkop   talde bakoitzeko kide kopurua (TALDEKOP tamainako bektorea, erreferentziaz) 
             elem   fitxategiko elementuak (EMAX x ALDAKOP tamainako matrizea, erreferentziaz)
             nor    taldekideen zerrenda (TALDEKOP x EMAX tamainako matrizea, erreferentziaz)
   Irteera:  trinko trinkotasuna (TALDEKOP tamainako bektorea, erreferentziaz) 
***************************************************************************************************/

// EGITEKO
// Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batazbestekoa
void trinkotasuna (int *tkop, float elem[][ALDAKOP], int nor[][EMAX], float *trinko)
{
   int kont,i,j,k;
   double batura_dis;
   for (i = 0; i < TALDEKOP; i++)
   {
      kont = 0;
      batura_dis = 0.0;
      #pragma omp parallel for private(j,k) reduction(+:batura_dis,kont) schedule(dynamic)
      for (j = 0; j < tkop[i]; j++)
         for (k = j+1; k < tkop[i]; k++)
         {
            batura_dis += (double) dis_gen(elem[nor[i][j]], elem[nor[i][k]]);
	    kont++;
         }      
      trinko[i] = tkop[i] < 2 ? 0.0 : (float) (batura_dis / kont); 
   }
}





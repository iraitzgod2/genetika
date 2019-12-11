/*
    KA - OpenmP
    funtg_s.c
    talde_gen.c programan erabiltzen diren errutinak

    OSATZEKO
**************************************************************************************************/

#include <stdio.h>
#include <float.h>     

#include <math.h>
#include "definetg.h"		// konstanteen definizioak





/* 1 - Bi elementuen arteko distantzia genetikoa kalkulatzeko funtzioa (distantzia euklidearra)
       Sarrera:  20 aldagaiko bi elementu (erreferentziaz)
       Irteera:  distantzia (float)
***************************************************************************************************/

// EGITEKO
// kalkulatu bi elementuen arteko distantzia (euklidearra)
float dis_gen (float *zent, float *elem)
{
   float dis = 0.0; 
   for (int i = 0; i < 20; ++i)
      dis +=(float)pow((double)(zent[i]-elem[i]), 2.0);
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
   int zentmin;
   float dis, dismin;
   for (int ele = 0; ele < elekop; ele++)
   {
      dismin = FLT_MAX;
      for (int zen = 0; zen < TALDEKOP; zen++)
      {
         dis = dis_gen(elem[ele], zent[zen]);
         if (dis < dismin)
         {
            dismin = dis;
            zentmin = zen;
         }
      }
      popul[ele] = zentmin;
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
   // EGITEKO
   // Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batazbestekoa
   int kont;
   double batura_dis;
   for (int i = 0; i < TALDEKOP; i++)
   {
      kont = 0;
      batura_dis = 0.0;
      for (int j = 0; j < tkop[i]; j++)
         for (int k = j+1; k < tkop[i]; k++)
         {
            batura_dis += (double) dis_gen(elem[nor[i][j]], elem[nor[i][k]]);
            kont++;
         }
      trinko[i] = tkop[i] < 2 ? 0.0 : (float) (batura_dis / kont); 
   }
}





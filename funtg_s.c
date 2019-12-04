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

float dis_gen (float *zent, float *elem)
{
   float dis = 0.0; 
   for (int i = 0; i < 20; ++i)
      dis += pow(zent[i]-elem[i], 2.0);
   return sqrt(dis);
   // EGITEKO
   // kalkulatu bi elementuen arteko distantzia (euklidearra)

}





/* 2 - Talde gertuena kalkulatzeko funtzioa (zentroide gertuena)
   Sarrera:  elekop elementu kopurua, int
             elem   EMAX x ALDAKOP tamainako matrizea, erreferentziaz
             zent   TALDEKOP x ALDAKOP tamainako matrizea, erreferentziaz
   Irteera:  popul  EMAX tamainako bektorea, erreferentziaz, osagai bat elementuko, dagokion taldea
***************************************************************************************************/

void talde_gertuena (int elekop, float elem[][ALDAKOP], float zent[][ALDAKOP], int *popul)
{
   int* poplag = popul;
   int zentmin;
   float dis, dismin = FLT_MAX;
   for (int ele = 0; ele < elekop; ele++) {
      for (int zen = 0; zen < TALDEKOP; zen++) {
         dis = dis_gen(elem[ele], zent[zen]);
         if (dis < dismin) {
            dismin = dis;
            zentmin = zen;
         }
         //printf("%d\t", zen);
      }
      *poplag = zentmin;
      ++poplag;
      //printf("%f\t%d\t %d ", dismin, zentmin, ele);
   }
   //printf("\t-- Bukaera --\n");
   // EGITEKO
   // popul: elementu bakoitzaren zentroide hurbilena, haren "taldea"

}





/* 3 - Taldeen trinkotasuna kalkulatzeko funtzioa (kideen arteko distantzia, b.b.) 
   Sarrera:  tkop   talde bakoitzeko kide kopurua (TALDEKOP tamainako bektorea, erreferentziaz) 
             elem   fitxategiko elementuak (EMAX x ALDAKOP tamainako matrizea, erreferentziaz)
             nor    taldekideen zerrenda (TALDEKOP x EMAX tamainako matrizea, erreferentziaz)
   Irteera:  trinko trinkotasuna (TALDEKOP tamainako bektorea, erreferentziaz) 
***************************************************************************************************/

void trinkotasuna (int *tkop, float elem[][ALDAKOP], int nor[][EMAX], float *trinko)
{
   // EGITEKO
   // Kalkulatu taldeen trinkotasuna: kideen arteko distantzien batazbestekoa
   int* tkoplag = tkop;
   for (int i = 0; i < TALDEKOP; i++) {
      for (int j = i+1; j < *tkoplag; j++) {
         
      }
      tkoplag++;
   }
}


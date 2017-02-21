#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include <math.h>
#include "Reseau.h"


/* Table de hachage */
typedef struct {
	int nE;
	int m;
	CellNoeud** tab;
} tableHachage;
	
int fonctionClef(int x , int y);

int fonctionHachage(int clef,int M);

Noeud* rechercheCreeNoeudHachage(Reseau *R, tableHachage* H, double x, double y);

#endif

#ifndef __TAS2CLEFS_H__
#define __TAS2CLEFS_H__

typedef struct{
	double c; /* distance */
	int i; /* numero */
} Element;

typedef struct{
	int n;
	Element **H;
	int *A;
} Tas2Clefs;

void init(Tas2Clefs *t, int n);

int racine();

int filsGauche(int i);

int filsDroit(int i);

int pere(int i);

int hasPere(int i);

int taille(Tas2Clefs *t);

int isNoeud(Tas2Clefs *t, int i);

int hasFilsGauche(Tas2Clefs *t, int i);

int hasFilsDroit(Tas2Clefs *t, int i);

int isFeuille(Tas2Clefs *t, int i);

void swap(Tas2Clefs *t, int i, int j);

void monter(Tas2Clefs *t, int i);

int plusPetitFils(Tas2Clefs *t, int i);

void descendre(Tas2Clefs *t, int i);

Element *min(Tas2Clefs *t);

void insert(Tas2Clefs *t, int i, double c);

int supprMin(Tas2Clefs *t);

int estDansTas(Tas2Clefs *t, int i);

void supprElem(Tas2Clefs *t, int i);


#endif

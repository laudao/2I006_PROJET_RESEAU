#include <stdlib.h>
#include "Tas2Clefs.h"
#include <stdio.h>

void init(Tas2Clefs *t, int max){
	t->n = 0;
	t->max = max;
	t->H = (Element**)malloc(sizeof(Element*)*(max+1));
	t->A = (int*)malloc(sizeof(int)*(max+1));
	int i;
	for (i=1; i<=max; i++){
		t->H[i] = NULL;
		t->A[i] = -1;
	}
}

int racine(){
	return 1;
}

int filsGauche(int i){
	return 2*i;
}

int filsDroit(int i){
	return 2*i+1;
}

int pere(int i){
	if (i == racine()) return 0;
	else return i/2;
}

int hasPere(int i){
	return i != racine();
}

int taille(Tas2Clefs *t){
	return t->n;
}

int isNoeud(Tas2Clefs *t, int i){
	return i <= taille(t);
}

int hasFilsGauche(Tas2Clefs *t, int i){
	return isNoeud(t, filsGauche(i));
}

int hasFilsDroit(Tas2Clefs *t, int i){
	return isNoeud(t, filsDroit(i));
}

int isFeuille(Tas2Clefs *t, int i){
	return !hasFilsGauche(t, i);
}

void swap(Tas2Clefs *t, int i, int j){
	Element *tmp = t->H[i];
	t->H[i] = t->H[j];
	t->H[j] = tmp;
	t->A[t->H[i]->i] = i;
	t->A[t->H[j]->i] = j;
}

void monter(Tas2Clefs *t, int i){
	if (!hasPere(i)) return ;
	int papa = pere(i);
	if (t->H[papa]->c > t->H[i]->c){
		swap(t, i, papa);
		monter(t, papa);
	}
}

int plusPetitFils(Tas2Clefs *t, int i){
	if (!hasFilsDroit(t, i)){
		return filsGauche(i);
	}
	else{
		int cg = filsGauche(i);
		int cd = filsDroit(i);
		return (t->H[cg]->c < t->H[cd]->c)?cg:cd;
	}
}

void descendre(Tas2Clefs *t, int i){
	if (isFeuille(t, i)) return;
	int fiston = plusPetitFils(t, i);
	if(t->H[i]->c > t->H[fiston]->c){
		swap(t, fiston, i);
		descendre(t, fiston);
	}
}

Element *min(Tas2Clefs *t){
	if (t->n != 0) return t->H[racine()];
	else return NULL;
}

void insert(Tas2Clefs *t, int i, double c){
	if (!(estDansTas(t, i))){
		Element *nouveau = (Element*)malloc(sizeof(Element));
		nouveau->c = c;
		nouveau->i = i;
	
		t->n++;
		t->H[t->n] = nouveau;
		t->A[i] = t->n;
		monter(t, t->n);
	}
}

int supprMin(Tas2Clefs *t){
	if (t->n == 0){
		printf("Le tas est vide\n");
		return -1;
	}
	int min = t->H[racine()]->i;
	swap(t, racine(), t->n);
	t->A[min] = -1;
	t->n--;
	descendre(t, racine());
	return min;
}

int estDansTas(Tas2Clefs *t, int i){
	return t->A[i] != -1;
}

void supprElem(Tas2Clefs *t, int i){
	if (!(estDansTas(t, i))){
		printf("L'element cherche n'est pas dans le tas\n");
		return ;
	}
	else{
		int position = t->A[i];
		swap(t, position, t->n);
		t->A[i] = -1;
		t->n--;
		t->A[t->H[position]->i] = position;
		descendre(t, position);
		monter(t, position);
	}
}

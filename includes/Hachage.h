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

/* fonction clef pour tout point (x,y): f(x, y) = y + (x + y)(x + y + 1)/2 */
int fonctionClef(int x , int y);

/* fonction de hachage h(k) = floor(M(kA − floor(kA)) pour toute clef k */
int fonctionHachage(int clef,int M);

/* retourne un Noeud du Reseau R correspondant au point (x,y) dans la table de hachage H de R 
	 si ce point existe dans H, retourne un point existant dans H
	 dans le cas contraire, crée un Noeud et l'ajoute dans H ainsi que dans la liste des noeuds de 
	 R */
Noeud *rechercheCreeNoeudHachage(Reseau *R, tableHachage* H, double x, double y);

/* reconstruit le reseau R a partir de la liste des chaines C en utilisant la table de hachage H */
Reseau *reconstitueReseauHachage(Chaines *C);

#endif

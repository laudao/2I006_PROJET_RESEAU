#include "Hachage.h"
#define A ((sqrt(5) - 1) / 2)

int fonctionClef(int x , int y)
{
	return y+(x+y)*(x+y+1)/2;
}

int fonctionHachage(int clef,int M)
{
	int h = floor(M* (clef * A - floor(clef * A)));
	return h;
}

Noeud* rechercheCreeNoeudHachage(Reseau *R, tableHachage* H, double x, double y)
{
	int indice=fonctionHachage(fonctionClef(x,y),H->m);
	Noeud* noeud;
	CellNoeud* curr=H -> tab[indice];
	
	while((curr)&&((curr->nd->x != x )|| (curr->nd->y != y))){
		curr=curr->suiv;
	}
	if(curr){
		printf("Noeud (%f,%f) trouve\n", x,y);
		return curr->nd;
	}
	noeud=creerNoeud(R,x,y);
	ajoutNoeudReseau(R,noeud);
	
	H->tab[indice]=creerCellNoeud(H->tab[indice],noeud);
	return noeud;
}

Reseau *reconstitueReseauHachage(Chaines *C)
{
	CellChaine *chaine; /* chaine courante */
	CellPoint *point; /* point courant dans la chaine courante */
	Noeud *noeudCurr; /* noeudCourant correspondant au point courant */
	Noeud *extrA; /* extremite A d'une chaine */
	Noeud *extrB; /* extremite B d'une chaine */
	Noeud *prec; /* element precedent d'un noeud dans une chaine */

	/* initialisation du reseau R */
	Reseau *R = (Reseau *)malloc(sizeof(Reseau));
	R -> nbNoeuds = 0;
	R -> gamma = C -> gamma;
	R -> noeuds = NULL;
	R -> commodites = NULL;

	chaine = C -> chaines;
	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
	while (chaine){
		point = chaine -> points;
		prec = NULL;

		/* extremite de la chaine */
		if (point){
			extrA = rechercheCreeNoeudListe(R, point -> x, point -> y);
		}

		while (point){
			
			/* on ajoute le noeud si on ne l'a pas deja rencontre */
			/* noeudCurr = le Noeud correspondant a point */
			noeudCurr  = rechercheCreeNoeudListe(R, point -> x, point -> y);

			/* on ajoute ses voisins */
			if (prec){
				ajoutCellNoeudVoisin(noeudCurr, prec); /* son voisin precedent */
			}
			/* son voisin suivant */
			if (point -> suiv){
				ajoutCellNoeudVoisin(noeudCurr, rechercheCreeNoeudListe(R, point -> suiv -> x, point -> suiv -> y));
			}

			prec = noeudCurr;
			point = point -> suiv;
		}

		/* la chaine compte plus d'un point */
		if (comptePoints(chaine) > 1){
			extrB = prec;
			ajoutCellCommodite(R, creerCellCommodite(extrA, extrB));			
		}

		chaine = chaine -> suiv;
	}

	return R;
}


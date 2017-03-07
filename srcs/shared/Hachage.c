#include "Hachage.h"
#include "Reseau.h"
#define A ((sqrt(5) - 1) / 2)

tableHachage *initTableHachage(int m)
{
	int i;
	tableHachage *tableH = (tableHachage *)malloc(sizeof(tableHachage));

	if (tableH == NULL){
		fprintf(stderr, "Erreur lors de l'allocation de la table de hachage\n");
		exit(1);
	}

	tableH -> nE = 0;
	tableH -> m = m;
	/* allocation d'un tableau de m pointeurs sur CellNoeud */
	tableH -> tab = (CellNoeud **)malloc(sizeof(CellNoeud *) * m);

	for (i = 0; i < m; i++){
		tableH -> tab[i] = NULL;
	}

	return tableH;
}
int fonctionClef(int x , int y)
{
	return y + (x + y) * (x + y + 1) / 2;
}

int fonctionHachage(int clef,int M)
{
	int h = floor(M * (clef * A - floor(clef * A)));
	return h;
}

Noeud* rechercheCreeNoeudHachage(Reseau *R, tableHachage* H, double x, double y)
{
	int indice; /* valeur de hachage, indice de la case ou aller chercher le Noeud */
	Noeud* noeud; /* Noeud eventuellement cree */
	CellNoeud* curr; /* CellNoeud courant dans la liste */

	indice = fonctionHachage(fonctionClef(x,y), H -> m);
	curr = H -> tab[indice];

	/* parcours de la liste contenue dans la case d'indice indice du tableau de hachage */
	while((curr) && ((curr -> nd -> x != x ) || (curr->nd->y != y))){
		curr = curr->suiv;
	} 
	
	/* le Noeud se trouve dans la liste */
	if (curr){
		noeud = curr -> nd;
	}
	/* pas dans la liste */
	else{
		noeud = creerNoeud(R, x, y); /* creation du Noeud */

		/* creation du CellNoeud correspondant au noeud et ajout en tete des noeuds du Reseau */
		ajoutNoeudReseau(R, noeud);
	
		/* creation du CellNoeud correspondant au noeud et ajout en tete de liste de H -> tab[indice] */
		H -> tab[indice] = creerCellNoeud(H -> tab[indice], noeud);
	}
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
	tableHachage *H; /* table de hachage */
	Reseau *R;
	
	/* initialisation du reseau R */
	R = initialiseReseau(C);

	H = initTableHachage(100); /* initialisation d'une table de hachage de taille 100 */

	chaine = C -> chaines;

	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
	while (chaine){
		point = chaine -> points;
		prec = NULL;

		/* extremite de la chaine */
		if (point){
			extrA = rechercheCreeNoeudHachage(R, H, point -> x, point -> y);
		}

		while (point){
			
			/* on ajoute le noeud si on ne l'a pas deja rencontre */
			/* noeudCurr = le Noeud correspondant a point */
			noeudCurr  = rechercheCreeNoeudHachage(R, H, point -> x, point -> y);

			/* on ajoute ses voisins */
			if (prec){
				ajoutCellNoeudVoisin(noeudCurr, prec); /* son voisin precedent */
			}
			/* son voisin suivant */
			if (point -> suiv){
				ajoutCellNoeudVoisin(noeudCurr, rechercheCreeNoeudHachage(R, H, point -> suiv -> x, point -> suiv -> y));
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


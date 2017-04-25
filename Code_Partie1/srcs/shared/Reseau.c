#include "Reseau.h"
#include <float.h>

Noeud *creerNoeud(Reseau *R, double x, double y)
{
	Noeud *nouveau = (Noeud *)malloc(sizeof(Noeud));
	
	if (nouveau == NULL){
		printf("Erreur : probleme d'allocation d'un Noeud\n");
		exit(1);
	}

	nouveau -> num = R -> nbNoeuds;
	nouveau -> x = x;
	nouveau -> y = y;
	nouveau -> voisins = NULL;

	return nouveau;
}

CellNoeud *creerCellNoeud(CellNoeud *liste, Noeud *noeud)
{
	CellNoeud *nouveau = (CellNoeud *)malloc(sizeof(Noeud));
	nouveau -> nd = noeud;
	nouveau -> suiv = liste;
	return nouveau;
}

void ajoutNoeudReseau(Reseau *R, Noeud *noeud)
{
	(R -> nbNoeuds)++; /* incrementation du nombre de noeuds de R */
	/* creation du CellNoeud correspondant au noeud */
	CellNoeud *nouveauCellNoeud = creerCellNoeud(R -> noeuds, noeud);

	/* ajout du CellNoeud en tete de liste */
	nouveauCellNoeud -> suiv = R -> noeuds;
	R -> noeuds = nouveauCellNoeud;
}

Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y)
{
	CellNoeud *cellNoeudCourant;
	Noeud *nouveauNoeud;

	cellNoeudCourant = R -> noeuds;

	/* parcours de la liste chainee de noeuds jusqu'a trouver le noeud correspondant
	   ou a tomber sur NULL */
	while ((cellNoeudCourant) && ((cellNoeudCourant -> nd -> x != x) || (cellNoeudCourant -> nd -> y != y))){
		cellNoeudCourant = cellNoeudCourant -> suiv;
	}

	/* le noeud a ete trouve */
	if (cellNoeudCourant){
		return cellNoeudCourant -> nd;	
	}
	/* le noeud n'a pas ete trouve */
	else{
		nouveauNoeud = creerNoeud(R, x, y); /* creation du noeud */
		ajoutNoeudReseau(R, nouveauNoeud); /* ajout du noeud dans la liste des noeuds de R */
		return nouveauNoeud;
	}
}

CellCommodite *creerCellCommodite(Noeud *extrA, Noeud *extrB)
{
	CellCommodite *nouveau = (CellCommodite *)malloc(sizeof(CellCommodite));
	nouveau -> extrA = extrA;
	nouveau -> extrB = extrB;
	nouveau -> suiv = NULL;

	return nouveau;
}

void ajoutCellCommodite(Reseau *R, CellCommodite *cellComm)
{
	/* cellComm est ajoute en tete de liste de commodites de R */
	cellComm -> suiv = R -> commodites; 
	R -> commodites = cellComm;
}

void ajoutCellNoeudVoisin(Noeud *noeud, Noeud *voisin)
{
	CellNoeud *cellNoeudVoisin;
	CellNoeud *tmp;
	
	tmp = noeud -> voisins;
	
	/* voisin est peut etre deja dans la liste des voisins de noeuds
		 parcours de la liste des voisins pour verifier */
	while ((tmp) && (tmp -> nd -> num != voisin -> num)){	
		tmp = tmp -> suiv;
	}
	
	/* voisin n'est pas dans la liste */
	if (!tmp){
		cellNoeudVoisin = creerCellNoeud(noeud -> voisins, voisin); /* CellNoeud pointant sur voisin */
		noeud -> voisins = cellNoeudVoisin; /* ajout de ce CellNoeud en tete de liste */
	}
}

Reseau* initialiseReseau(Chaines *C)
{
	Reseau *nouv = (Reseau *)malloc(sizeof(Reseau));

	if (!nouv){
		fprintf(stderr, "Erreur lors de l'allocation du Reseau\n");
		exit(1);
	}

	nouv -> nbNoeuds = 0;
	nouv -> gamma = C -> gamma;
	nouv -> noeuds = NULL;
	nouv -> commodites = NULL;

	return nouv;
}

void libere_noeud(Noeud *n)
{
	free(n);
}

void libere_liste_cell_noeud(CellNoeud *liste)
{
	CellNoeud *tmp;
	
	while (liste){
		tmp = liste;
		liste = liste->suiv;
		libere_noeud(tmp->nd);
		free(tmp);
	}
}

void libere_liste_cell_commodite(CellCommodite *liste){

	CellCommodite *tmp;
	
	while (liste){
		tmp = liste;
		liste = liste->suiv;
		free(tmp);
	}
}

void libere_reseau(Reseau *r)
{
	libere_liste_cell_noeud(r->noeuds);
	libere_liste_cell_commodite(r->commodites);
	free(r);
}

Reseau *reconstitueReseauListe(Chaines *C)
{
	CellChaine *chaine; /* chaine courante */
	CellPoint *point; /* point courant dans la chaine courante */
	Noeud *noeudCurr; /* noeudCourant correspondant au point courant */
	Noeud *extrA; /* extremite A d'une chaine */
	Noeud *extrB; /* extremite B d'une chaine */
	Noeud *prec; /* element precedent d'un noeud dans une chaine */
	Reseau *R; /* le Reseau */

	/* initialisation du reseau R */
	R = initialiseReseau(C);

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


int nbLiaison(Reseau *R)
{
	int nbTotal; /* nombre total de liaisons */
	CellNoeud *cellNoeudCurr; /* cellNoeud courant */
	Noeud *noeudCurr;  /* noeud courant */
	CellNoeud *voisin; /* voisin courant du noeud courant */

	cellNoeudCurr = R -> noeuds;
	nbTotal = 0;
	
	/* parcours des CellNoeud */
	while (cellNoeudCurr){
		noeudCurr = cellNoeudCurr -> nd; 
		voisin = noeudCurr -> voisins;
		/* on parcourt les voisins de noeudCurr */
		while (voisin){
			/* si on est pas encore passe sur voisin, on compte une liaison en plus 
			   sinon, cela signifie qu'on a deja compte la liaison entre voisin et noeudCurr 
			   on sait qu'on est deja passe sur voisin si le numero de son noeud est >= a celui de noeudCurr 
			   les noeuds sont ajoutes en tete de liste et R -> nbNoeuds est incremente a chaque ajout
			   on parcourt donc la liste de CellNoeud dans l'ordre decroissant
			   */
			if (voisin -> nd -> num < noeudCurr -> num){
				nbTotal++;
			}
			voisin = voisin -> suiv;
		}
	
		cellNoeudCurr = cellNoeudCurr -> suiv;
	}

	return nbTotal;
}

int nbCommodites(Reseau *R)
{
	CellCommodite *commodite;
	int nbComm;

	commodite = R -> commodites;
	nbComm = 0;

	while (commodite){
		nbComm++;
		commodite = commodite -> suiv;
	}

	return nbComm;
}

void ecrireReseauTxt(Reseau *R, FILE *f)
{
	CellNoeud *cellNoeudCurr;
	Noeud *noeudCurr;
	CellNoeud *voisin;
	CellCommodite *comm;

	fprintf(f, "NbNoeuds: %d\n", R -> nbNoeuds);
	fprintf(f, "NbLiaison: %d\n", nbLiaison(R));
	fprintf(f, "NbCommodite: %d\n", nbCommodites(R));
	fprintf(f, "Gamma: %d\n", R -> gamma);
	fprintf(f, "\n");

	cellNoeudCurr = R -> noeuds;

	/* affichage des noeuds */
	while (cellNoeudCurr){
		noeudCurr = cellNoeudCurr -> nd;
		fprintf(f, "v %d %f %f\n", noeudCurr -> num, noeudCurr -> x, noeudCurr -> y);
		cellNoeudCurr = cellNoeudCurr -> suiv;
	}
	
	fprintf(f, "\n");

	cellNoeudCurr = R -> noeuds;

	/* affichage des liaisons */
	while (cellNoeudCurr){
		noeudCurr = cellNoeudCurr -> nd;
		voisin = noeudCurr -> voisins;

		/* parcours des voisins */
		while (voisin){
			/* si son numero est < a celui de noeudCurr, c'est qu'on est pas encore passe dessus */
			if (voisin -> nd -> num < noeudCurr -> num){
				fprintf(f, "l %d %d\n", voisin -> nd -> num, noeudCurr -> num);
			}
			voisin = voisin -> suiv;
		}
		cellNoeudCurr = cellNoeudCurr -> suiv;
	}

	fprintf(f, "\n");
	comm = R -> commodites;

	while (comm){
		fprintf(f, "k %d %d\n", comm -> extrA -> num, comm -> extrB -> num);
		comm = comm -> suiv;
	}
}

void afficheReseauSVG(Reseau *R, char *nomInstance)
{
	SVGwriter *svg;
	CellNoeud *cellNoeudCurr;
	Noeud *noeudCurr;
	CellNoeud *voisin;
	double xmin, ymin, xmax, ymax;
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
	double xnormalise;
	double ynormalise;
	double xnormalisesuiv;
	double ynormalisesuiv;

	svg = (SVGwriter *)malloc(sizeof(SVGwriter));

	/* initialise svg avec nomInstance pour nom du fichier html a creer 
		 et avec une taille de 500x500 pixels */
	SVGinit(svg, nomInstance, 500, 500);
	//SVGlineRandColor(svg); /* couleur aleatoire pour les lignes */
	SVGlineColor(svg, Black);
	SVGpointColor(svg, Red);  /* fixe la couleur des points a rouge */

	cellNoeudCurr = R -> noeuds;

	while (cellNoeudCurr){
		noeudCurr = cellNoeudCurr -> nd;
		
		if (noeudCurr -> x < xmin){
			xmin = noeudCurr -> x;
		}
			
		if (noeudCurr -> y < ymin){
			ymin = noeudCurr -> y;
		}
		
		if (noeudCurr -> x > xmax){
			xmax = noeudCurr -> x;
		}
			
		if (noeudCurr -> y > ymax){
			ymax = noeudCurr -> y;
		}		
		
		cellNoeudCurr = cellNoeudCurr -> suiv;
	}
	
	cellNoeudCurr = R -> noeuds;
	
	while (cellNoeudCurr){
		noeudCurr = cellNoeudCurr -> nd;
		xnormalise = (noeudCurr -> x - xmin)*(500)/(xmax-xmin);
		ynormalise = (noeudCurr -> y - ymin)*(500)/(ymax-ymin);
		SVGpoint(svg,xnormalise,ynormalise);

		voisin = noeudCurr -> voisins;

		while (voisin){
			if (voisin -> nd -> num >= noeudCurr -> num){
				xnormalisesuiv = (voisin -> nd -> x - xmin)*(500)/(xmax-xmin);
				ynormalisesuiv = (voisin -> nd -> y - ymin)*(500)/(ymax-ymin); 
				SVGline(svg, xnormalise, ynormalise, xnormalisesuiv, ynormalisesuiv);
			}
			voisin = voisin -> suiv;
		}
		
		cellNoeudCurr = cellNoeudCurr -> suiv;
	}

	SVGfinalize(svg);
}

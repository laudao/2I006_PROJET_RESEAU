#include "arbreQuat.h"
#include <float.h>

void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;
	
	/* premiere chaine de la liste chainee de chaines de instance */
	chaine_curr = C -> chaines;

	while (chaine_curr){
		/* premier point de la liste chainee de points de chaine_curr */
		point_curr = chaine_curr -> points;
		while (point_curr){
			if(point_curr -> x > *xmax){
				*xmax = point_curr -> x; 
			}
			 
			if(point_curr -> y > *ymax){
				*ymax = point_curr -> y; 
			}
			 
			if(point_curr -> x < *xmin){
				*xmin = point_curr -> x; 
			}
			if(point_curr -> y < *ymin){
				*ymin = point_curr -> y; 
			}
			 
			point_curr = point_curr -> suiv;
		}

		chaine_curr = chaine_curr -> suiv;
	}
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY)
{
	ArbreQuat *nouveau = (ArbreQuat *)malloc(sizeof(ArbreQuat));
	
	if (nouveau == NULL){
		fprintf(stderr, "Erreur lors de l'allocation de l'arbre\n");
		exit(1);
	}
	
	nouveau -> xc = xc;
	nouveau -> yc = yc;
	nouveau -> coteX = coteX;
	nouveau -> coteY = coteY;
	nouveau -> noeud = NULL;
	nouveau -> so = NULL;
	nouveau -> se = NULL;
	nouveau -> no = NULL;
	nouveau -> ne = NULL;
	
	return nouveau;
}

ArbreQuat* initialise_parent(Chaines *C)
{
	double xmin, ymin, xmax, ymax;
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
	chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
	return creerArbreQuat(xmin + ((xmax-xmin)/2), ymin + ((ymax-ymin)/2) ,xmax-xmin,ymax-ymin);
}

ArbreQuat* insererNoeudArbre(Noeud* n, ArbreQuat* a, ArbreQuat* parent)
{
	ArbreQuat *nouveau;
	Noeud *noeudStocke;
	double xc;
	double yc;

	/* insertion dans un arbre vide */ 
	if (a == NULL){
		/* les dimensions de la nouvelle cellule correspondent a la moitie de la cellule parent
			les deux premiers parametres correspondent aux coordonnees du centre de la cellule
		*/

		if (n -> x < parent -> xc){
			xc = (parent -> xc)	- ((parent -> coteX)/4);
		}
		else{
			xc = (parent -> xc)	+ ((parent -> coteX)/4);
		}

		if (n -> y > parent -> yc){
			yc = (parent -> yc) + ((parent -> coteY)/4);
		}else{
			yc = (parent -> yc) - ((parent -> coteY)/4);
		}

		nouveau = creerArbreQuat(xc, yc, (parent -> coteX) / 2, (parent -> coteY) / 2);
		nouveau -> noeud = n; 
		return nouveau;
	}
	else{
		/* insertion au niveau d'une feuille de l'arbre */
		if (a -> noeud != NULL){
			noeudStocke = a -> noeud; /* on recupere le noeud deja stocke */
			a -> noeud = NULL; 
			a = insererNoeudArbre(n, a, parent);
			a = insererNoeudArbre(noeudStocke, a, parent);

			return a;
		} 
		/* A != NULL et A -> noeud == NULL
			 On est sur une cellule interne de l'arbre */
		else{	
			/* ouest */
			if (n -> x < a -> xc){
				if (n -> y > a -> yc){
					/* nord ouest */
					a -> no = insererNoeudArbre(n, a -> no, a);
				}else{
					/* sud ouest */
					a-> so = insererNoeudArbre(n, a -> so, a);
				}
			}
			/* est */
			else{
				if (n -> y > a -> yc){
					/* nord est */
					a -> ne = insererNoeudArbre(n, a -> ne, a);
				}else{
					/* sud est */
					a-> se = insererNoeudArbre(n, a -> se, a);
				}
			}
			return a;
		}
	}
}

Noeud* chercherNoeudArbre(CellPoint* pt, Reseau* R, ArbreQuat** aptr, ArbreQuat* parent)
{
	Noeud *n = NULL;

	/* arbre vide*/
	if (*aptr == NULL){
		n = creerNoeud(R, pt -> x, pt -> y); /* creation du noeud */
		ajoutNoeudReseau(R, n); /* ajout du noeud au reseau */
		*aptr = insererNoeudArbre(n, *aptr, parent); /* insertion du noeud dans l'arbre*/
		return n;
	}
	else{
		/* feuille */
		if ((*aptr) -> noeud != NULL){
			/* le noeud que l'on cherche correspond a celui de la feuille */
			if ((*aptr) -> noeud -> x == pt -> x && (*aptr) -> noeud -> y == pt -> y){
				return (*aptr) -> noeud;
			}
			/* le noeud de la feuille ne correspond pas */
			else{
				n = creerNoeud(R, pt ->x, pt -> y); /* on cree le noeud*/
				ajoutNoeudReseau(R, n); /* on ajoute au reseau R */
				*aptr = insererNoeudArbre(n, *aptr, parent); /* insertion dans l'arbre*/
				return n;
			}
		}
		/* cellule interne */
		else{
			/* ouest */
			if (pt -> x < (*aptr) -> xc){
				if (pt -> y > (*aptr) -> yc){
					/* nord-ouest */
					n = chercherNoeudArbre(pt,R,&((*aptr)->no), *aptr);
				}
				else{
					/* sud-ouest */
					n = chercherNoeudArbre(pt,R,&((*aptr)->so), *aptr);
				}
			}
			/* est */
			else{
				if (pt -> y > (*aptr) -> yc){
					/* nord-est */
					n = chercherNoeudArbre(pt,R,&((*aptr)->ne), *aptr);
				}
				else{
					/* sud-est */
					n = chercherNoeudArbre(pt,R,&((*aptr)->se), *aptr);
				}
			}
			return n;
		}
	}
}

Reseau* recreeReseauArbre(Chaines* C)
{
	CellChaine *chaine; /* chaine courante */
	CellPoint *point; /* point courant dans la chaine courante */
	Noeud *noeudCurr; /* noeudCourant correspondant au point courant */
	Noeud *extrA; /* extremite A d'une chaine */
	Noeud *extrB; /* extremite B d'une chaine */
	Noeud *prec; /* element precedent d'un noeud dans une chaine */
	Reseau *R;
	ArbreQuat* aptr; /* arbre courant dans la recherche ou l'insertion */
	ArbreQuat* parent; /* arbre parent */

	/* initialisation du reseau R */
	R = initialiseReseau(C);
	/* initialisation de l'arbre quaternaire parent */
	parent = initialise_parent(C); 
	aptr = parent;
	
	chaine = C -> chaines;

	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
	while (chaine){
		point = chaine -> points;
		prec = NULL;

		/* extremite de la chaine */
		if (point){
			extrA = chercherNoeudArbre(point,R,&aptr,parent);
		}

		while (point){
			
			/* on ajoute le noeud si on ne l'a pas deja rencontre */
			/* noeudCurr = le Noeud correspondant a point */
			noeudCurr  = chercherNoeudArbre(point,R,&aptr,parent);
			
			/* on ajoute ses voisins */
			if (prec){
				ajoutCellNoeudVoisin(noeudCurr, prec); /* son voisin precedent */
			}
		
				
			/* son voisin suivant */
			if (point -> suiv){
				ajoutCellNoeudVoisin(noeudCurr, chercherNoeudArbre(point->suiv,R,&aptr,parent));
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

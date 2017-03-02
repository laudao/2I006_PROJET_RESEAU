#include "Reseau.h"
#include "arbreQuat.h"

void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;
	
	/* premiere chaine de la liste chainee de chaines de instance */
	chaine_curr = instance -> chaines;

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
			 
			if(point_curr -> x < xmin){
				*xmin = point_curr -> x; 
			}
			if(point_curr -> y < xmin){
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
ArbreQuat* insererNoeudArbre(Noeud* n, ArbreQuat* a, ArbreQuat* parent);
{
	ArbreQuat *nouveau;
	Noeud *noeudStocke;

	if (a == NULL){
		/* les dimensions de la nouvelle cellule correspondent a la moitie de la cellule parent
			les deux premiers parametres correspondent aux coordonnees du centre de la cellule
		*/
		nouveau = creerArbreQuat((parent -> coteX)/4, (parent -> coteY)/4, (parent -> coteX) / 2, (parent -> coteY) / 2);
		nouveau -> noeud = n;
		return nouveau;
	}
	else{
		if (a -> noeud != NULL){
			noeudStocke = a -> noeud; /* on recupere le noeud deja stocke */
			a -> noeud = NULL; 
				
			/* n doit etre place au nord-ouest de la cellule */
			if ((n -> x < a -> xc) && (n -> y > a -> yc)){
				a -> no = insererNoeudArbre(n, a -> no, a);
			} 
			
			if ((noeudStocke -> x < a -> xc) && (noeudStocke -> y > a -> yc)){
				a -> no = insererNoeudArbre(noeudStocke, a -> no, a);
			}
			/* au nord-est */
			if ((n -> x > a -> xc) && (n -> y > a -> yc)){
				a -> ne = insererNoeudArbre(n, a -> ne, a);
			} 
			
			if ((noeudStocke -> x > a -> xc) && (noeudStocke -> y > a -> yc)){
				a -> ne = insererNoeudArbre(noeudStocke, a -> ne, a);
			} 

			/* au sud-ouest */
			if ((n -> x < a -> xc) && (n -> y < a -> yc)){
				a-> so = insererNoeudArbre(n, a -> so, a);
			} 
			
			if ((noeudStocke -> x > a -> xc) && (noeudStocke -> y > a -> yc)){
				a -> so = insererNoeudArbre(noeudStocke, a -> so, a);
			} 

			/* au sud-est */
			if ((n -> x > a -> xc) && (n -> y < a -> yc)){
				a-> se = insererNoeudArbre(n, a -> se, a);
			}
			
			if ((noeudStocke -> x > a -> xc) && (noeudStocke -> y < a -> yc)){
				a-> se = insererNoeudArbre(noeudStocke, a -> se, a);
			}

			return a;
		} 
		/* A != NULL et A -> noeud == NULL */
		else{
			if ((n -> x < a -> xc) && (n -> y > a -> yc)){
				a -> no = insererNoeudArbre(n, a -> no, a);
			}

			if ((n -> x > a -> xc) && (n -> y > a -> yc)){
				a -> ne = insererNoeudArbre(n, a -> ne, a);
			}

			if ((n -> x < a -> xc) && (n -> y < a -> yc)){
				a-> so = insererNoeudArbre(n, a -> so, a);
			}

			if ((n -> x > a -> xc) && (n -> y < a -> yc)){
				a-> se = insererNoeudArbre(n, a -> se, a);
			}
			return a;
		}
}

Noeud* chercherNoeudArbre(CellPoint* pt, Reseau* R, ArbreQuat** aptr, ArbreQuat* parent);
{

	if(*aptr==NULL){
	
	Noeud *n=creerNoeud(R, pt->x, pt->y);
	ajoutNoeudReseau(R,n);
	insererNoeudArbre(n,aptr,parent);
	printf("noeud (%f,%f) ajouté\n",noud->x,noud->y)
	
	return aptr->noud;
	}
	
	else{
		if(*aptr->noud!=NULL){	
			if(aptr->noud->x==pt->x && aptr->noud->y==pt->y){
				printf("noeud (%f,%f) trouvé\n",noud->x,noud->y);
				return aptr->noud;
			}
			else{
				Noeud *n=creerNoeud(R, pt->x, pt->y);
				joutNoeudReseau(R,n);
				insererNoeudArbre(n,aptr,parent);
				printf("noeud (%f,%f) ajouté\n",noud->x,noud->y)
				return aptr->noud;
			}
		
		}
	
		if ((pt -> x < aptr -> xc) && (pt -> y > aptr -> yc)){
				chercherNoeudArbre(pt,R,aptr->no,aptr);
			}
		if ((pt -> x > aptr -> xc) && (pt -> y > aptr -> yc)){
				chercherNoeudArbre(pt,R,aptr->ne,aptr);
			}

		if ((pt -> x < aptr -> xc) && (pt -> y < aptr -> yc)){
				chercherNoeudArbre(pt,R,aptr->so,aptr);
			}
		if ((pt -> x > aptr -> xc) && (pt -> y < aptr -> yc)){
				chercherNoeudArbre(pt,R,aptr_>se,aptr);
			}
	
	}
	return NULL;
}

Reseau* recreeReseauArbre(Chaines* C)
{
CellChaine *chaine; /* chaine courante */
	CellPoint *point; /* point courant dans la chaine courante */
	Noeud *noeudCurr; /* noeudCourant correspondant au point courant */
	Noeud *extrA; /* extremite A d'une chaine */
	Noeud *extrB; /* extremite B d'une chaine */
	Noeud *prec; /* element precedent d'un noeud dans une chaine */

	ArbreQuat** aptr;
	ArbreQuat* parent=NULL;

	/* initialisation du reseau R */
	Reseau *R = (Reseau *)malloc(sizeof(Reseau));
	R -> nbNoeuds = 0;
	R -> gamma = C -> gamma;
	R -> noeuds = NULL;
	R -> commodites = NULL;
	
	/* initialisation de l'arbre quaternaire parent */
	parent = initialise_parent(C); 
	*aptr = parent;

	chaine = C -> chaines;

	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
	while (chaine){
		point = chaine -> points;
		prec = NULL;

		/* extremite de la chaine */
		if (point){
			extrA = chercherNoeudArbre(point,R,*aptr,parent);
		}

		while (point){
			
			/* on ajoute le noeud si on ne l'a pas deja rencontre */
			/* noeudCurr = le Noeud correspondant a point */
			noeudCurr  = chercherNoeudArbre(point,R,*aptr,parent);

			/* on ajoute ses voisins */
			if (prec){
				ajoutCellNoeudVoisin(noeudCurr, prec); /* son voisin precedent */
			}
			/* son voisin suivant */
			if (point -> suiv){
				ajoutCellNoeudVoisin(noeudCurr,chercherNoeudArbre(point->suiv,R,p_Arbre,parent));
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

}

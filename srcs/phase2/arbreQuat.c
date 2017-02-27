#include "Reseau.h"

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
	
	//if (*parent=NULL){
	// coordonnees: centre
 	//return creerArbreQuat(n -> x, n -> y,coteX, coteY);
	//}
	
	if (*a == NULL){
		nouveau = creerArbreQuat(n -> x, n -> y, (parent -> coteX) / 2, (parent -> coteY) / 2);
		
		return nouveau;
	}
	else{
		if (a -> noeud != NULL){
			a -> noeud = NULL;
			if ((n -> x < a -> xc) && (n -> y > a -> yc)){
				a -> no = insererNoeudArbre(n, a, parent);
			} 
		
		if ((nouveau -> x > parent -> x) && (nouveau -> y > parent -> y)){
			parent -> ne = nouveau;
		} 
		
		if ((nouveau -> x < parent -> x) && (nouveau -> y < parent -> y)){
			parent -> so = nouveau;
		} 
		
		if ((nouveau -> x > parent -> x) && (nouveau -> y < parent -> y)){
			parent -> se = nouveau;
		} 
	
}

Noeud* chercherNoeudArbre(CellPoint* pt, Reseau* R, ArbreQuat** aptr, ArbreQuat* parent);
{
	return NULL;
}

Reseau* recreeReseauArbre(Chaines* C)
{
	return NULL;
}

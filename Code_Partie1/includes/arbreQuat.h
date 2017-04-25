#ifndef __ARBRE_QUAT_H
#define __ARBRE_QUAT_H
#include "Reseau.h"
#include <float.h>

/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat{
	double xc, yc; /* Coordonnees du centre de la cellule */
	double coteX; /* Longueur de la cellule */
	double coteY; /* Hauteur de la cellule */
	Noeud *noeud; /* Pointeur vers le noeud du reseau */
	struct arbreQuat *so; /* Sous-arbre sud-ouest, pour x < xc et y < yc */
	struct arbreQuat *se; /* Sous-arbre sud-est, pour x >= xc et y < yc */
	struct arbreQuat *no; /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
	struct arbreQuat *ne;  /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

/* determine les coordonnees minimales et maximales des points constituant les differentes chaines du reseau */
void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax);

/* cree une cellule de l'arbre quaternaire, de centre (xc, yc), 
	 de longueur coteX et de hauteur coteY
	 initialise le noeud du reseau, les arbres nord-ouest, nordest, sud-ouest et sud-est a NULL
*/
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY);

/* initialise un arbre parent a partir de chaines */
ArbreQuat* initialise_parent(Chaines *C);

/* insere un Noeud du reseau dans l'arbre quaternaire */
ArbreQuat* insererNoeudArbre(Noeud* n, ArbreQuat* a, ArbreQuat* parent);

/* retourne un Noeud du reseau R correspondant au point (pt -> nd -> x, pt -> nd -> y) dans l'arbre quaternaire
	Si il existe dans l'arbre, la fonction retourne un noeud existant dans l'arbre
	Sinon, elle cree un noeud et l'ajoute dans l'arbre ainsi que dans la liste des noeuds de R 
*/
Noeud* chercherNoeudArbre(CellPoint* pt, Reseau* R, ArbreQuat** aptr, ArbreQuat* parent);

/* reconstruit le reseau R a partir de la liste des chaines C et en utilisant l'arbre quaternaire*/
Reseau* recreeReseauArbre(Chaines* C);

void libere_arbre(ArbreQuat *a);

#endif

#ifndef __RESEAU_H__
#define __RESEAU_H__

#include "Chaine.h"
#include "SVGwriter.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;

/* cree un Noeud a partir d'un numero et de coordonnees x, y */
Noeud *creerNoeud(Reseau *R, double x, double y);

/* cree un CellNoeud a partir d'un Noeud et l'ajoute dans la liste de noeuds liste */
CellNoeud *creerCellNoeud(CellNoeud *liste, Noeud *noeud);

/* ajoute un Noeud dans la liste des noeuds du Reseau R */
void ajoutNoeudReseau(Reseau *R, Noeud *noeud);

/* retourne un Noeud du reseau R correspondant au point (x, y) 
	 dans la liste chainee noeuds de R 
	 si ce point n'existe pas dans noeud, cree un noeud et l'ajoute dans noeud ainsi
	 que dans la liste des noeuds du reseau de R */
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);

/* cree un CellCommodite ayant pour extremite A extrA, pour extremite B extrB */
CellCommodite *creerCellCommodite(Noeud *extrA, Noeud *extrB);

/* ajoute cellComm en tete de liste de commodites de R */
void ajoutCellCommodite(Reseau *R, CellCommodite *cellComm);

/* cree un CellNoeud qui pointe sur voisin et l'ajoute a la liste de voisins de noeud */
void ajoutCellNoeudVoisin(Noeud *noeud, Noeud *voisin);

/* reconstruit le reseau R a partir de la liste des chaines C
	 et en utilisant pour structure directement la liste chainee noeud */
Reseau* reconstitueReseauListe(Chaines *C);

/* ecrit dans un fichier le contenu d'un Reseau */
void ecrireReseauTxt(Reseau *R, FILE *f);

/* compte le nombre de liaisons d'un reseau */
int nbLiaison(Reseau *R);

/* compte le nombre de commodites d'un reseau */
int nbCommodites(Reseau *R);

/* permet de creer un fichier SVG en html pour visualiser un reseau */
void afficheReseauSVG(Reseau *R, char* nomInstance);


#endif


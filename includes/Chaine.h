#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "entree_sortie.h"

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

/* permet d'allouer, de remplir et de retourner une instance de Chaines a partir
d'un fichier d'entree f*/
Chaines* lectureChaine(FILE *f);

Chaines *initialiseChaines();

/* cree un CellPoint ayant pour coordonnees x et y */
CellPoint *creerCellPoint(double x, double y);

/* cree un CellChaine ayant pour numero num, pour points les CellPoint de liste 
et pour suivant suiv */
CellChaine *creerCellChaine(int num);

/* ajoute un CellPoint en fin d'une liste de points contenue dans chaine */
void ajout_en_fin_point(CellChaine *chaine, double x, double y);

/* ajoute un CellPoint en tete d'une liste de points contenue dans chaine */
void ajout_en_tete_point(CellChaine *chaine, double x, double y);

/* ajoute un CellChaine en fin d'une liste de chaines contenue dans un Chaines*/
void ajout_en_fin_chaine(CellChaine *ch, Chaines *instance);

/* ajoute un CellChaine en tete d'une liste de chaines contenue dans instance */
void ajout_en_tete_chaine(CellChaine *ch, Chaines *instance);

/* libere la memoire occupee par un CellChaine */ 
void detruireLE(CellChaine *L);

/* affiche l'ensemble des chaines de instance*/
void afficheChaines(Chaines *instance);

/* compte le nombre de points dans une liste de points */
int comptePoints(CellChaine *ch);

/* ecrit dans un fichier le contenu d'une Chaine */
void ecrireChaineTxt(Chaines *C, FILE *f);

/* cree le fichier SVG en html a partir d'un struct Chaines */
void afficheChaineSVG(Chaines *C, char* nomInstance);

/* calcule la longueur physique de la chaine C*/
double longueurChaine(CellChaine *C);

/* calcule la longueur physique totale des chaines de l'instance C */
double longueurTotale(Chaines *C);

/* donne le nombre total d'occurences de points dans l'instance C */
int comptePointsTotal(Chaines *C);

#endif	

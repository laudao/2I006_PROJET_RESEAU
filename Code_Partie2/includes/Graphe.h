#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include<stdlib.h>
#include<stdio.h>
#include "Struct_Liste.h"
#include "Struct_File.h"
#include "Tas2Clefs.h"

typedef struct{
  int u,v;        /* Numeros des sommets extremité */
  double longueur;    /* longueur de l arete */

 /**************************************************/
 /* Emplacement des donnees utiles aux algorithmes */

  int calc_gamma; 

 /**************************************************/

} Arete;


typedef struct cellule_arete{
  Arete *a;     /* pointeur sur l'arete */
  struct cellule_arete *suiv;
} Cellule_arete;


typedef struct {
  int num;                 /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete* L_voisin;  /* Liste chainee des voisins */

 /**************************************************/
 /* Emplacement des donnees utiles aux algorithmes */

 

 /**************************************************/

} Sommet;


typedef struct{
  int e1,e2;    /* Les deux extremites de la commodite */
} Commod;


typedef struct{
  int nbsom;        /* Nombre de sommets */
  Sommet* *T_som;    /* Tableau de pointeurs sur sommets */
  int gamma;     

  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;

/* ajout de l'arete (u,v) dans le graphe */
void ajout_voisin(Graphe* G, int u, int v);

/* retourne un pointeur sur l'arete (u,v) si elle existe
   et NULL sinon */
Arete* acces_arete(Graphe* G, int u, int v);

/* retourne le plus petit nombre d'aretes d'un chemin entre deux sommets u et v */
int nbAretesMin_depuis_u(Graphe *G, int u, int v);

/* retourne le plus court chemin reliant u a v */
int* chemin_u_v(Graphe *G, int u, int v);

/* renvoie une liste d'entiers correspondant au plus courant chemin de u a v a partir d'un tableau de predecesseurs */
ListeEntier liste_chemin_u_v(int u, int v, int *pere);

/* retourne les chaines reliant les extremites de chaque commodite du graphe */
void chaines_commodites(Graphe *G, ListeEntier *L);

void chaines_commodites_Dijkstra(Graphe *G, ListeEntier *L);

/* ecrit les chaines reliant les extremites de chaque commodite dans un fichier .ncha */
void ecrit_chaines_commodites(Graphe *G, char *filename);

/* calcule le nombre maximal de chaines qui passe par la meme arete */
int evaluation_gamma(Graphe *G);

/* calcule la longueur totale des chemins obtenus en distance euclidienne des aretes */
double evaluation_longueur(Graphe *G);

/* met a jour la bordure */
void maj_bordure(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s);

/* retourne le plus court chemin en terme de distance euclidienne de r a u */
int* plus_court_chemins(Graphe *G, int r, int u);

void lecture_graphe(Graphe *G, FILE * f);

void afficheGrapheSVG(Graphe *G, char* nomInstance);





#endif

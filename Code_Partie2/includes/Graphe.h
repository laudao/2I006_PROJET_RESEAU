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
  
  /* pour plus_court_chemin_longueur_gamma */
	int dans_chemin; /* 1 si l'arete fait partie du chemin, 0 sinon */
	int dans_parcours; /* 1 si le parcours a considere l'arete, 0 sinon */
	int nb_util; /* pour un parcours, le nombre de fois ou on passe par l'arete */

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

/* retourne le plus court chemin, en terme d'aretes, reliant u a v */
int* plus_court_chemin_aretes(Graphe *G, int u, int v);

/* renvoie une liste d'entiers correspondant au plus courant chemin de u a v a partir d'un tableau de predecesseurs pere 
	met aussi le champ dans_chemin des aretes utilisees a 1 */
ListeEntier liste_chemin_u_v(Graphe *G, int u, int v, int *pere);

/* retourne les chaines reliant les extremites de chaque commodite du graphe 
	le parcours se fait en fonction du choix chmethod
*/
void chaines_commodites(Graphe *G, ListeEntier *L, int chmethod);

/* ecrit les chaines reliant les extremites de chaque commodite dans un fichier .ncha
	le parcours se fait en fonction du choix chmethod
*/
void ecrit_chaines_commodites(Graphe *G, char *filename, int chmethod);

/* met a 0 le champ calc_gamma de chaque arete */
void initialise_gamma(Graphe *G);

/* pour chaque arete utilisee dans le chemin precedent, remet a 0 le champ dans_chemin */
void remise_a_zero_dans_chemin(Graphe *G, int *pere, int u, int v);

/* pour chaque arete qui n'est pas dans le chemin mais qui a ete consideree dans le parcours, reinitialise son gamma a sa valeur pre-parcours 
 pour toutes les aretes, reinitialise dans_parcours et nb_util a 0 */
void remise_a_zero_post_parcours(Graphe *G);

/* calcule le nombre maximal de chaines qui passe par la meme arete */
int evaluation_gamma(Graphe *G, int chmethod);

/* calcule la longueur totale des chemins obtenus en distance euclidienne des aretes */
double evaluation_longueur(Graphe *G, int chmethod);

/* met a jour la bordure */
void maj_bordure(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s);

/* retourne le plus court chemin en terme de distance euclidienne de r a u */
int* plus_court_chemin_distance(Graphe *G, int r, int u);

/* met a jour la bordure en prenant en compte calc_gamma */
void maj_bordure_gamma(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s);

/* retourne le plus court chemin en terme de distance en prenant en compte le nombre de chemins passant par les aretes */
int *plus_court_chemin_longueur_gamma(Graphe *G,int r, int u);

void lecture_graphe(Graphe *G, FILE * f);

void afficheGrapheSVG(Graphe *G, char* nomInstance);

#endif

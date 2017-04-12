#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Graphe.h"
#include"SVGwriter.h"
#include"entree_sortie.h"
#include"Struct_Liste.h"
#include <limits.h>

void ajout_voisin(Graphe* G, int u, int v){
  Cellule_arete *pca;
  Arete *pa;

	/* creation de l'arete reliant u a v */
  pa=(Arete *) malloc(sizeof(Arete));
  pa->u=u;
  pa->v=v;
  pa->longueur=sqrt( (G->T_som[u]->x - G->T_som[v]->x)*(G->T_som[u]->x - G->T_som[v]->x) + (G->T_som[u]->y - G->T_som[v]->y)*(G->T_som[u]->y - G->T_som[v]->y));
	pa -> calc_gamma = 0;
	pa -> dans_chemin = 0;
	pa -> dans_parcours = 0;
	pa -> nb_util = 0;
	/* creation d'une cellule pointant sur cette arete */
  
  pca=(Cellule_arete*) malloc(sizeof(Cellule_arete));
  pca->a=pa;
  /* ajout de cette cellule dans la liste des voisins de u*/
  pca->suiv=G->T_som[u]->L_voisin;
  G->T_som[u]->L_voisin=pca;

	/* idem pour v */
  pca=(Cellule_arete*) malloc(sizeof(Cellule_arete));
  pca->a=pa;
  pca->suiv=G->T_som[v]->L_voisin;
  G->T_som[v]->L_voisin=pca;

}

Arete* acces_arete(Graphe* G, int u, int v){
  Cellule_arete *coura;

  coura=G->T_som[u]->L_voisin;
  while ( (coura!=NULL)&&(coura->a->u!=v)&&(coura->a->v!=v))
    coura=coura->suiv;

  if (coura==NULL) 
  	return NULL;
  else  
  	return coura->a;
}

int nbAretesMin_depuis_u(Graphe *G, int u, int v)
{
	Cellule_arete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visit = (int*)malloc(((G -> nbsom) + 1) * sizeof(int)); /* tableau de visites */
	int i;
	int nbAretesMin;
	S_file F; /* la file pour stocker les noeuds a visiter */

	nbAretesMin = -1;
	Init_file(&F);
	coura = NULL;
	e1 = 0;
	e2 = 0;

	/* initialisation des visites a -1 */
	for (i = 1; i <= G->nbsom; i++){
		visit[i] = -1;
	}	

	visit[u] = 0;
	enfile(&F, u);
	
	/* tant que la file n'est pas vide et qu'on a pas trouve le sommet v*/
	while (!(estFileVide(&F)) && (e2 != v)){ 
		e1 = defile(&F); /* e1 est le sommet a visiter qu'on vient de defiler */
		coura = G -> T_som[e1] -> L_voisin; /* arete incidente a e1 */
		
		/* parcours des aretes incidentes a e1 */
		while ((coura != NULL) && (e2 != v)){ 
			e2 = coura -> a -> v; /* extremite de l'arete */
			
			/* si l'extremite de l'arete est la meme */
			if (e2 == e1){
				e2 = coura -> a -> u; /* on prend l'autre extremite */
			}

			/* e2 n'a pas encore ete visite */
			if (visit[e2] == -1){
				visit[e2] = visit[e1] + 1; /* on incremente de 1 la longueur du chemin pour e2 */
				
				if (e2 != v){ /* e2 n'est pas le sommet recherche */
					enfile(&F, e2); /* on enfile e2 */
				}
			}
			coura = coura -> suiv;
		}
	}
	
	if (e2 == v){
		nbAretesMin = visit[v];
		free(visit);
		return nbAretesMin;
	}
	else{
		free(visit);
		return -1;
	}
}

int* plus_court_chemin_aretes(Graphe *G, int u, int v)
{
	Cellule_arete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visit = (int*)malloc(((G -> nbsom) + 1) * sizeof(int)); /* tableau de visites */
	int i;
	int *pere = (int*)malloc(((G -> nbsom) + 1) * sizeof(int)); /* tableau qui indique qui est le pere de qui dans arbo */
	S_file F;
	
	Init_file(&F);
	coura = NULL;
	e1 = 0;
	e2 = 0;
	
	/* initialisation des visites a -1 */
	for (i = 1; i <= G->nbsom; i++){
		visit[i] = -1;
		pere[i] = 0;
	}	
	
	visit[u] = 0;
	enfile(&F, u);
	
	/* tant que la file n'est pas vide et que v n'a pas ete trouve */
	while (!(estFileVide(&F)) && (e2 != v)){ 
		e1 = defile(&F); /* le sommet a visiter */
		coura = G -> T_som[e1] -> L_voisin; /* arete incidente a e1 */
			
		/* parcours des aretes */
		while ((coura != NULL) && (e2 != v)){ 
			e2 = coura -> a -> v; /* extremite de l'arete */
			
			/* si l'extremite de l'arete est la meme */
			if (e2 == e1){
				e2 = coura -> a -> u; /* on prend l'autre extremite */
			}
			
			if (visit[e2] == -1){ /* sommet non visite */
				visit[e2] = visit[e1] + 1; /* mise a jour de la longueur du chemin liant u a e2 */
				pere[e2] = e1; /* le pere de e2 dans l'arborescence des chemins de  u est e1*/
				enfile(&F, e2); /* on doit visiter e2*/
			}
			coura = coura -> suiv;
		}
	}
	
	free(visit);
	return pere;
}

ListeEntier liste_chemin_u_v(Graphe *G, int u, int v, int *pere)
{
	Arete *a;
	ListeEntier L; /* la liste des sommets liant u a v */
	Init_Liste(&L);
	
	ajoute_en_tete(&L, v);
	while (v != u){ /* on remonte l'arborescence jusqu'a u */
		ajoute_en_tete(&L, pere[v]); /* on ajoute le pere de chaque sommet dans la liste */
		a = acces_arete(G, pere[v], v);
		//printf("(%d, %d) est parcourue\n", a->u, a->v);
		a->dans_chemin = 1; /* l'arete est marquee comme etant utilisee dans ce chemin */
		v = pere[v];
	}

	return L;
}

void chaines_commodites(Graphe *G, ListeEntier* L, int chmeth){
	int i;
	int *pred; /* le tableau des predecesseurs */

	switch(chmeth){
		case 1:
			/* pour chaque commodite du graphe */
			for(i=0; i < G->nbcommod; i++){			
				/* on stocke dans L[i] le chemin reliant une extremite a l'autre */
				pred = plus_court_chemin_aretes(G, G->T_commod[i].e1, G->T_commod[i].e2);
				L[i] = liste_chemin_u_v(G, G->T_commod[i].e1, G->T_commod[i].e2, pred);
			}
			break;
		case 2:
			for(i=0; i < G->nbcommod; i++){
				pred = plus_court_chemin_distance(G, G->T_commod[i].e1, G->T_commod[i].e2);
				L[i] = liste_chemin_u_v(G, G->T_commod[i].e1, G->T_commod[i].e2, pred);
			}	
			break;
		case 3:
			initialise_gamma(G);
			for (i=0; i < G->nbcommod; i++){
				pred = plus_court_chemin_longueur_gamma(G, G->T_commod[i].e1, G->T_commod[i].e2);
				L[i] = liste_chemin_u_v(G, G->T_commod[i].e1, G->T_commod[i].e2, pred);
				remise_a_zero_post_parcours(G);
				remise_a_zero_dans_chemin(G, pred, G->T_commod[i].e1, G->T_commod[i].e2);
			}
			break;
		default:;
	}
}

void initialise_gamma(Graphe *G)
{
  int i;
  Cellule_arete *curr;

	/* pour chaque sommet */
  for (i=1; i<=G->nbsom; i++){
  	/* pour chaque arete incidente a ce sommet */
  	curr = G->T_som[i]->L_voisin;
  	
  	while (curr){
  	  curr->a->calc_gamma=0; /* on met calc_gamma a 0 */
  	  curr=curr->suiv;
  	}
  }
}

void remise_a_zero_dans_chemin(Graphe *G, int *pere, int u, int v)
{
	Arete *a;
	
	while (v != u){ /* on remonte l'arborescence jusqu'a u */
		a = acces_arete(G, pere[v], v);
		a->dans_chemin = 0; /* reinitialisation */
		v = pere[v];
	}
}

void remise_a_zero_post_parcours(Graphe *G)
{
	int i;
	Cellule_arete *curr;

	for(i=1; i<=G->nbsom; i++){
		curr = G->T_som[i]->L_voisin;

		while (curr){
			/* si l'arete n'est pas dans le chemin mais a ete consideree durant le parcours */
			if ((curr->a->dans_chemin == 0) && (curr->a->dans_parcours == 1)){
				/* on reinitialise calc_gamma a sa valeur pre-parcours*/
				curr->a->calc_gamma = curr->a->calc_gamma - curr->a->nb_util;
		//		printf("(%d, %d) parcourue mais pas utilisee : %d\n", curr->a->u, curr->a->v, curr->a->calc_gamma);
			}
			/* pour toutes les aretes (donc aussi celles utilisees dans le chemin) */
			curr->a->dans_parcours=0;
			curr->a->nb_util=0;
			curr = curr->suiv;
		}
	}
}

void ecrit_chaines_commodites(Graphe *G, char* filename, int chmeth){
	FILE *f = fopen(filename, "w");
	/* tableau de liste d'entiers pour stocker les chemins */
	ListeEntier* tabchaines = (ListeEntier*)malloc((G->nbcommod)*sizeof(ListeEntier));
	int i;
	ListeEntier cour;
	
	if (f == NULL){
		fprintf(stderr,"Probleme lors de l'ouverture du fichier %s\n", filename);
	}
	else{
		if ((chmeth > 0) && (chmeth <= 3)){
			chaines_commodites(G, tabchaines, chmeth); /* tabchaines va contenir le chemin pour chaque commodite*/
			
			/* on parcourt chaque chemin pour chaque commodite */
			for(i=0; i < G->nbcommod; i++){
				cour = tabchaines[i];
				while(cour != NULL){
					fprintf(f, "%d ", cour->u);
					cour = cour->suiv;
				}
				fprintf(f,"-1\n");
			}
		}
		fclose(f);
	}
}



int evaluation_gamma(Graphe *G, int chmeth)
{
	/* tableau de listes d'entiers*/
	ListeEntier *tabchaines = (ListeEntier*)malloc((G->nbcommod)*sizeof(ListeEntier));
	int i;
	int u, v;
	int gamma;
	ListeEntier cour;
	Arete* arete_cour;

	gamma = 0;

/* tabchaines contient le chemin pour chaque commodite */
	chaines_commodites(G, tabchaines, chmeth);
	initialise_gamma(G);

	/* on parcourt chaque chemin pour chaque commodite */
	for (i=0; i < G->nbcommod; i++){
		cour = tabchaines[i];
		
		/* parcours du chemin */
		while (cour->suiv != NULL){
			/* les extremites de l'arete */
			u = cour->u;
			v = cour->suiv->u;
			
			/* on recupere l'arete correspondante */
			arete_cour = acces_arete(G, u, v);
			(arete_cour->calc_gamma)++;
				
			if (arete_cour->calc_gamma > gamma){
					gamma = arete_cour->calc_gamma;
			}

			cour = cour->suiv;
		}
	}

	G->gamma = gamma;
	return gamma;
}

double evaluation_longueur(Graphe *G, int chmeth)
{
	ListeEntier *tabchaines = (ListeEntier*)malloc((G->nbcommod)*sizeof(ListeEntier));
	int i;
	int u, v;
	double longueur_totale;
	ListeEntier cour;
	Arete *arete_cour;

	longueur_totale = 0;

/* tabchaines contient le chemin pour chaque commodite */
	chaines_commodites(G, tabchaines, chmeth);

	/* on parcourt chaque chemin pour chaque commodite */
	for (i=0; i < G->nbcommod; i++){
		cour = tabchaines[i];

		/* parcours du chemin */
		while (cour->suiv != NULL){
			/* les extremites de l'arete pour chaque arete du chemin */
			u = cour->u;
			v = cour->suiv->u;
			/* on recupere l'arete correspondante */
			arete_cour = acces_arete(G, u, v);

			longueur_totale += arete_cour->longueur;
			
			cour = cour->suiv;
		}
	}

	return longueur_totale;
}


void maj_bordure(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s){
	int i;
	Cellule_arete *coura = G->T_som[s]->L_voisin;
	
	while (coura != NULL){ /* parcours des voisins */
		i = coura->a->v;
		
		if (i == s) i = coura->a->u;
		
		/* si le sommet n'a pas ete visite et qu'on a trouve un chemin plus interessant pour y acceder */
		if ((marque[i] == 0) && (lambda[i] > lambda[s] + coura->a->longueur)){
			lambda[i] = lambda[s] + coura->a->longueur;
			pred[i] = s;
			insert(bordure, i, lambda[i]); /* insertion dans la bordure */
		}
		
		coura = coura->suiv;
	}
}

int *plus_court_chemin_distance(Graphe *G, int r, int u)
{
	Tas2Clefs bordure;
	/* tableau de liste d'entiers pour stocker le plus court chemin de r aux autres sommets*/
	ListeEntier *L = (ListeEntier*)malloc((G->nbsom+1)*sizeof(ListeEntier));
	/* tableau des valeurs des plus courts chemins de racine r*/
	int *lambda = (int*)malloc(sizeof(int)*(G->nbsom+1)); 
	/* tableau des predecesseurs dans l'arborescence des plus courts chemins de racine r*/
	int *pred = (int*)malloc(sizeof(int)*(G->nbsom+1));
	int *marque = (int*)malloc(sizeof(int)*(G->nbsom+1)); /* indique si le sommet a ete visite ou non*/
	int i;
	int s;
	
	init(&bordure, G->nbsom); 
	
	for (i=1; i<=G->nbsom; i++){
		pred[i] = -1;
		marque[i] = 0;
		lambda[i] = INT_MAX;
		Init_Liste(&(L[i]));
	}

	lambda[r] = 0;
	insert(&bordure, r, 0);
	
	while ((bordure.n != 0) && (s != u)){ /* tant que la bordure n'est pas vide */
		s = supprMin(&bordure); /* on enleve l'element de plus courte distance de r */
		marque[s] = 1;
		if (s != u){
			maj_bordure(G, pred, marque, lambda, &bordure, s);
		}
	}
	
	free(lambda);
	free(marque);
	return pred;
}

void maj_bordure_gamma(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s){
	int i;
	float val_compare;
	Cellule_arete *coura = G->T_som[s]->L_voisin;
	
	
	while (coura != NULL){ /* parcours des voisins */
		
		i = coura->a->v;
		if (i == s) i = coura->a->u;

		/* si la longueur > gamma, privilegier gamma, et inversement */
		if (coura->a->longueur > coura->a->calc_gamma){
			val_compare = 0.01*coura->a->longueur + coura->a->calc_gamma;
		}

		if (coura->a->longueur < coura->a->calc_gamma){
			val_compare = coura->a->longueur + 0.01*coura->a->calc_gamma;
		}


		/* si le sommet n'a pas ete visite et qu'on a trouve un chemin plus interessant pour y acceder */

		if ((marque[i] == 0) && (lambda[i] > lambda[s] + val_compare)){
			lambda[i] = lambda[s] + val_compare;
			pred[i] = s;
			insert(bordure, i, lambda[i]); /* insertion dans la bordure */
		}

		coura = coura->suiv;
	}
}



int *plus_court_chemin_longueur_gamma(Graphe *G,int r, int u){

	Tas2Clefs bordure;
	/* tableau de liste d'entiers pour stocker le plus court chemin de r aux autres sommets*/
	ListeEntier *L = (ListeEntier*)malloc((G->nbsom+1)*sizeof(ListeEntier));
	/* tableau des valeurs des plus courts chemins de racine r*/
	int *lambda = (int*)malloc(sizeof(int)*(G->nbsom+1)); 
	/* tableau des predecesseurs dans l'arborescence des plus courts chemins de racine r*/
	int *pred = (int*)malloc(sizeof(int)*(G->nbsom+1));
	int *marque = (int*)malloc(sizeof(int)*(G->nbsom+1)); /* indique si le sommet a ete visite ou non*/
	int i;
	int s;
	Arete *a;
	
	init(&bordure, G->nbsom); 
	
	for (i=1; i<=G->nbsom; i++){
		pred[i] = -1;
		marque[i] = 0;
		lambda[i] = INT_MAX;
		Init_Liste(&(L[i]));
	}
	
	lambda[r] = 0;
	insert(&bordure, r, 0);
	while ((bordure.n != 0) && (s != u)){ /* tant que la bordure n'est pas vide */
		s = supprMin(&bordure); /* on enleve l'element de plus courte distance de r */
	
		/* si le sommet s n'est pas la racine, on incremente le champ calc_gamma de l'arete qui le lie a son predecesseur */
		if (s != r){
			a = acces_arete(G, s, pred[s]);
			(a->calc_gamma)++;
			(a->nb_util)++;
			a->dans_parcours=1;
			printf("(%d, %d) calc_gamma: %d\n", a->u, a->v,a->calc_gamma);
		}
		
		marque[s] = 1;
		if (s != u){
			maj_bordure_gamma(G, pred, marque, lambda, &bordure, s);
		}
	}
	free(lambda);
	free(marque);
	return pred;
}

void lecture_graphe(Graphe *G, FILE * f){

  char temp[101];
  int nbarete;   /* stockage temporaire du nombre d aretes */
  int i;
  int num;       /* lecture dans le fichier d un numero */

 
  GetChaine(f,100,temp);
  G->nbsom=GetEntier(f);
  GetChaine(f,100,temp);
  nbarete=GetEntier(f);
  GetChaine(f,100,temp);
  G->nbcommod=GetEntier(f);
  GetChaine(f,100,temp);
  G->gamma=GetEntier(f);

  G->T_som=(Sommet**) malloc((G->nbsom+1)*sizeof(Sommet*));
  for (i=1;i<=G->nbsom;i++){
    G->T_som[i]=(Sommet*) malloc(sizeof(Sommet));
    G->T_som[i]->L_voisin=NULL;
  }

  G->T_commod=(Commod*)malloc(G->nbcommod*sizeof(Commod));

  for (i=0;i<G->nbsom;i++){
    GetChaine(f,100,temp);
    num=GetEntier(f);
    G->T_som[num]->num=num;
    G->T_som[num]->x=GetReel(f);
    G->T_som[num]->y=GetReel(f);
  }

  for (i=0;i<nbarete;i++){
    GetChaine(f,100,temp);
    ajout_voisin(G,GetEntier(f),GetEntier(f));
  }

  for (i=0;i<G->nbcommod;i++){
    GetChaine(f,100,temp);
    G->T_commod[i].e1=GetEntier(f);
    G->T_commod[i].e2=GetEntier(f);
  }

}


void afficheGrapheSVG(Graphe *G, char* nomInstance){

  SVGwriter svg;
  double maxx=0,maxy=0,minx=1e6,miny=1e6;
  int i;
  double xu,yu,xv,yv;
  Cellule_arete *coura;

  for(i=1;i<=G->nbsom;i++){
    if (maxx<G->T_som[i]->x) maxx=G->T_som[i]->x;
    if (maxy<G->T_som[i]->y) maxy=G->T_som[i]->y;
    if (minx>G->T_som[i]->x) minx=G->T_som[i]->x;
    if (miny>G->T_som[i]->y) miny=G->T_som[i]->y;
  }

  SVGinit(&svg,nomInstance,500,500);

  SVGpointColor(&svg, Red);
  SVGlineColor(&svg, Black);

  for(i=1;i<=G->nbsom;i++){

    xu=G->T_som[i]->x;
    yu=G->T_som[i]->y;
    
//    printf("%d %d %f %f\n",i,G->T_som[i]->num,xu,yu);
    SVGpoint(&svg,500*(xu-minx)/(maxx-minx),500*(yu-miny)/(maxy-miny));

    coura=G->T_som[i]->L_voisin;
    while (coura!=NULL){

      if (coura->a->u==i){
	xv=G->T_som[coura->a->v]->x;
	yv=G->T_som[coura->a->v]->y;
      }
      else{
	xv=G->T_som[coura->a->u]->x;
	yv=G->T_som[coura->a->u]->y;
      }

      SVGline(&svg,500*(xu-minx)/(maxx-minx),500*(yu-miny)/(maxy-miny),500*(xv-minx)/(maxx-minx),500*(yv-miny)/(maxy-miny));
      
      coura=coura->suiv;
    }

  }

  SVGfinalize(&svg);

}





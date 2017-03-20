#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Graphe.h"
#include"SVGwriter.h"
#include"entree_sortie.h"
#include"Struct_Liste.h"


void ajout_voisin(Graphe* G, int u, int v){
  Cellule_arete *pca;
  Arete *pa;

  pa=(Arete *) malloc(sizeof(Arete));
  pa->u=u;
  pa->v=v;
  pa->longueur=sqrt( (G->T_som[u]->x - G->T_som[v]->x)*(G->T_som[u]->x - G->T_som[v]->x) + (G->T_som[u]->y - G->T_som[v]->y)*(G->T_som[u]->y - G->T_som[v]->y));

  pca=(Cellule_arete*) malloc(sizeof(Cellule_arete));
  pca->a=pa;
  pca->suiv=G->T_som[u]->L_voisin;
  G->T_som[u]->L_voisin=pca;

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
	Cellule_arrete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visite = (int*)malloc(G -> nbsom + 1* sizeof(int)); /* tableau de visites */
	int i;
	int nbAretesMin;
	int verif;
	S_File F;
	Init_file(F);
	nbAretesMin = 0;
	coura = NULL;
	e1 = 0;
	e2 = 0;
	verif = 0;
	/* initialisation des visites a -1 */
	for (i = 1; i <= G->nbsom; i++){
		visit[i] = -1;
	}	
	
	visit[u] = 0;
	enfile(&F, u);
	
	/* tant que la file n'est pas vide */
	while (!(estFileVide(&F) && (!verif)){ 
		e1 = defile(&F);
		coura = G -> t_som[e1] -> L_voisin; /* arete incidente a e1 */
		
		/* parcours des aretes */
		while ((coura != NULL) && (!verif)){ 
			e2 = coura -> a -> v; /* extremite de l'arete */
			
			/* si l'extremite de l'arete est la meme */
			if (e2 == e1){
				e2 = coura -> a -> u; /* on prend l'autre extremite */
			}
			if (visit[e2] == -1){ /* sommet non visite */
				visit[e2] = visit[e1] + 1;
				
				if (e2 == v){ /* c'est le sommet qu'on cherche*/
					verif = 1;
				}
				else{
					enfile(&F, e2);
				}
			}
			coura = coura -> suiv;
		}
	}
	
	return nbAretesMin;
}

ListeEntier chemin_u_v(Graphe *G, int u, int v)
{
	Cellule_arrete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visite = (int*)malloc(G -> nbsom + 1 * sizeof(int)); /* tableau de visites */
	int i;
	int nbAretesMin;
	int verif;
	int *pere = (int*)malloc(G -> nbsom + 1 * sizeof(int)); /* pour indiquer qui est le pere de qui dans arbo */
	S_File F;
	ListeEntier L;
	Init_Liste(&L);
	
	Init_file(F);
	nbAretesMin = 0;
	coura = NULL;
	e1 = 0;
	e2 = 0;
	verif = 0;
	
	/* initialisation des visites a -1 */
	for (i = 1; i <= G->nbsom; i++){
		visit[i] = -1;
		pere[i] = 0;
	}	
	
	visit[u] = 0;
	enfile(&F, u);
	
	/* tant que la file n'est pas vide */
	while (!(estFileVide(&F) && (!verif)){ 
		e1 = defile(&F);
		coura = G -> t_som[e1] -> L_voisin; /* arete incidente a e1 */
		
		/* parcours des aretes */
		while ((coura != NULL) && (!verif)){ 
			e2 = coura -> a -> v; /* extremite de l'arete */
			
			/* si l'extremite de l'arete est la meme */
			if (e2 == e1){
				e2 = coura -> a -> u; /* on prend l'autre extremite */
			}
			
			if (e2 == v){ /* c'est le sommet qu'on cherche*/
					verif = 1;
			}
				
			if (visit[e2] == -1){ /* sommet non visite */
				visit[e2] = visit[e1] + 1;
				pere[e2] = e1; /* le pere de e2 dans l'arborescence de u est e1*/
				enfile(&F, e2); /* on doit visiter e2*/
			}
			coura = coura -> suiv;
		}
	}
	
	if (!verif){ /* v n'est pas dans le graphe */
		return NULL;
	}
	else{
		while (v != u){ /* on remonte l'arborescence jusqu'a u */
			ajoute_en_tete(&L, pere[v]); /* on ajoute le pere de chaque sommet dans la liste */
			v = pere[v];
		}
		ajoute_en_tete_(&L, u); /* ajout de u a la fin*/
	}
	
	return L;
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
    
    printf("%d %d %f %f\n",i,G->T_som[i]->num,xu,yu);
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



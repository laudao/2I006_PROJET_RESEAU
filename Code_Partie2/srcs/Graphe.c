#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Graphe.h"
#include"SVGwriter.h"
#include"entree_sortie.h"
#include"Struct_Liste.h"

/* ajout de l'arete (u,v) dans le graphe */
void ajout_voisin(Graphe* G, int u, int v){
  Cellule_arete *pca;
  Arete *pa;

	/* creation de l'arete reliant u a v */
  pa=(Arete *) malloc(sizeof(Arete));
  pa->u=u;
  pa->v=v;
  pa->longueur=sqrt( (G->T_som[u]->x - G->T_som[v]->x)*(G->T_som[u]->x - G->T_som[v]->x) + (G->T_som[u]->y - G->T_som[v]->y)*(G->T_som[u]->y - G->T_som[v]->y));

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

/* determine si u et v sont voisins ou non */
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

/* retourne le plus petit nombre d'aretes d'un chemin entre deux sommets u et v */
int nbAretesMin_depuis_u(Graphe *G, int u, int v)
{
	Cellule_arete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visit = (int*)malloc((G -> nbsom) + 1 * sizeof(int)); /* tableau de visites */
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

/* retourne une liste d'entiers correspondant au plus court chemin reliant u a v */
ListeEntier chemin_u_v(Graphe *G, int u, int v)
{
	Cellule_arete *coura; /* arete courante */
	int e1, e2; /* extremite de l'arete */
	int *visit = (int*)malloc(G -> nbsom + 1 * sizeof(int)); /* tableau de visites */
	int i;
	int *pere = (int*)malloc(G -> nbsom + 1 * sizeof(int)); /* tableau qui indique qui est le pere de qui dans arbo */
	S_file F;
	ListeEntier L; /* la liste des sommets liant u a v */
	Init_Liste(&L);
	
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
			
			if (e2 != v){
				if (visit[e2] == -1){ /* sommet non visite */
					visit[e2] = visit[e1] + 1; /* mise a jour de la longueur du chemin liant u a e2 */
					pere[e2] = e1; /* le pere de e2 dans l'arborescence des chemins de  u est e1*/
					enfile(&F, e2); /* on doit visiter e2*/
				}
			}
			coura = coura -> suiv;
		}
	}
	
	if (e2 != v){ /* v n'est pas dans le graphe */
		return NULL;
	}
	else{
		ajoute_en_tete(&L, v);
		while (v != u){ /* on remonte l'arborescence jusqu'a u */
			ajoute_en_tete(&L, pere[v]); /* on ajoute le pere de chaque sommet dans la liste */
			v = pere[v];
		}
		ajoute_en_tete(&L, u); /* ajout de u a la fin*/
	}
	
	return L;
}

void chaines_commodites(Graphe *G,ListeEntier* L ){

	int i;
	
	for(i=0;i<nbcommod;i++){
		L[i]=chemin_u_v(G,G->T_commod[i].e1, G->T_commod[i].e1);
	}
}
		 
void ecrit_chaines_commodites(Graphe *G,char* filename){
	FILE *f=fopen(filename);
	ListeEntier* tabchaines=(ListeEntier*)malloc(G->nbcommod*sizeof(ListeEntier));
	int i;
	ListeEntier cour;
	
	if(f==NULL){
		fprintf(stderr,"Probleme lors de l'ouverture du fichier %s\n",filename);
	}
	else{
		chaines_commodites(G,tabchaines);
		
		for(i=0;i<G->nbcommod;i++){
			cour=tabchaines[i];
			while(cour!=NULL){
				fprintf(f,"%d ",cour->u);
				cour=cour->suiv;
			}
			fprintf(f,"-1\n");
		}
	}
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



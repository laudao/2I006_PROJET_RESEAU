#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Graphe.h"
#include "Struct_Liste.h"
#include "evaluation_NChaines.h"

int main(int argc,char**argv){

  char filename[104],filenameres[104],filenamencha[104];;
  int chmeth;
	Graphe G;
	FILE *f;
	int nbAr;
	double longueur;
//	double eval;

  if(argc!=3){
    printf("usage: %s <file> <numeromethod>\n",argv[0]);
    printf("where numeromethode = 1 if using Width firs search\n");
    return 1;
  }

  chmeth=atoi(argv[2]);
  strcpy(filename,argv[1]);
  strcpy(filenameres,argv[1]);
  strcpy(filenamencha,argv[1]);
  strcat(filenameres,".res");
  strcat(filenamencha,".ncha");

  f = fopen(filenameres,"r");

  if (f==NULL){
    printf("Probleme lecture du fichier %s\n",filenameres);
    exit(1);
  }

  lecture_graphe(&G,f);
	printf("Nombre de sommet: %d\n", G.nbsom);
	nbAr = nbAretesMin_depuis_u(&G, 5, 11);
	printf("Nombre d'aretes reliant 5 a 11: %d\n", nbAr);
	ecrit_chaines_commodites(&G, filenamencha);
	
	longueur = evaluation_longueur(&G, 5);
	printf("longueur totale: %f\n", longueur);

	evaluation_gamma(&G);
	printf("Gamma: %d\n", G.gamma);

//	eval = evaluation_NChaines(evaluation_gamma(&G), longueur_totale_chemins(&G, 1), filename);
//	printf("%f\n", eval);
	fclose(f);

	

  afficheGrapheSVG(&G,filename);


  return 0;
}

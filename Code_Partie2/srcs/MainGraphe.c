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
	//int nbAr;
	double longueur;
	double eval;
	int gamma;

  if(argc!=3){
    printf("usage: %s <file> <numeromethod>\n",argv[0]);
    printf("where numeromethode = 1 if using Width firs search\n");
    return 1;
  }

  chmeth=atoi(argv[2]);

	if ((chmeth < 0) || (chmeth > 3)){
		fprintf(stderr, "usage: 0 < chmeth <= 3\n");
		return 1;
	}

  if (chmeth == 1)
  	printf("1 : Parcours en largeur\n");

	if (chmeth == 2)
		printf("2 : Plus courts chemins (Algorithme de Dijkstra)\n");

	if (chmeth == 3)
		printf("3 : Plus courts chemins minimisant la valeur maximale d'utilisation des cables\n");

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
//	printf("Nombre de sommet: %d\n", G.nbsom);
//	nbAr = nbAretesMin_depuis_u(&G, 5, 11);
//	printf("Nombre d'aretes reliant 5 a 11: %d\n", nbAr);
	
	ecrit_chaines_commodites(&G, filenamencha, chmeth);
	
	longueur = evaluation_longueur(&G, chmeth);

	gamma = evaluation_gamma(&G, chmeth);


	eval = evaluation_NChaines(gamma, longueur, filename);
	printf("longueur totale: %f\n", longueur);
	printf("Gamma: %d\n", gamma);
	printf("Evaluation: %f\n", eval);
	fclose(f);

	

  afficheGrapheSVG(&G,filename);


  return 0;
}

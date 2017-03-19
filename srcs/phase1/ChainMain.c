#include "Chaine.h"

void menu()
{
	printf("--------------------------------\n");
	printf("Que voulez-vous faire?\n");
	printf(" 0 : sortie\n");
	printf(" 1 : Reconstituer les chaines\n");
	printf(" 2 : Ecrire le contenu des chaines dans un fichier\n");
	printf(" 3 : Creer un fichier SVG en html pour visualiser les chaines\n");
	printf(" 4 : Calculer la longueur physique totale des chaines de l'instance\n");
	printf(" 5 : Calculer le nombre total d'occurences de points dans l'instance\n"); 
	printf("> ");
}

int main(int argc, char **argv){
	char *nameFile;
	FILE *f1; /* le fichier contenant les chaines a reconstituer */
	FILE *f2; /* le fichier ou l'on ecrit les chaines */
	double total;
	int nbPoints;
	Chaines *instance;
	int choice;

	if (argc != 2){
		printf("Erreur format: %s <Instances_cha/nomFichier.cha>\n", argv[0]);
		return 1;
	}
	
	instance = NULL;
	nameFile = strdup(argv[1]);
	f1 = fopen(nameFile, "r");

	if (f1 == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
		return 1;
	}

	do{
		menu();
		scanf(" %d", &choice);

		switch(choice){
			case 0:
				printf("Au revoir et revenez vite o/\n");
				break;
			case 1:
				printf("**********************************\n");
				printf("Reconstitution des chaines de %s\n", nameFile);
				instance = lectureChaine(f1);
				break;
			case 2:
				if (instance){
					printf("**********************************\n");
					printf("Ecrire du contenu de %s dans Chaines.cha\n", nameFile);
					f2 = fopen("Chaines.cha", "w");
					ecrireChaineTxt(instance, f2);
				}
				else{
					printf("Vous n'avez pas encore reconstitue les chaines...\n");
				}
				break;
			case 3:
				if (instance){
					printf("**********************************\n");
					printf("Creation du fichier SVGChaines.html pour visualiser les chaines\n");
					afficheChaineSVG(instance, "SVGChaines");
				}
				else{
					printf("Vous n'avez pas encore reconstitue les chaines...\n");
				}
				break;
			case 4:
				if (instance){
					printf("**********************************\n");
					printf("Calcul de la longueur physique totale des chaines de l'instance\n");
					total = longueurTotale(instance);
					printf("Longueur totale: %f\n", total);
				}
				else{
					printf("Vous n'avez pas encore reconstitue les chaines...\n");
				}
				break;
			case 5:
				if (instance){
					printf("**********************************\n");
					printf("Calcul du nombre total d'occurences de points dans l'instance\n");
					nbPoints = comptePointsTotal(instance);
					printf("Nombre total de points: %d\n", nbPoints);
				}
				else{
					printf("Vous n'avez pas encore reconstitue les chaines...\n");
				}
				break;
			default:
				printf("Nous n'avons pas bien compris votre requete...\n");
			break;
		}
	}while (choice != 0);
	
	if (f1)
		fclose(f1);
	if (f2)
		fclose(f2);
	return 0;
}

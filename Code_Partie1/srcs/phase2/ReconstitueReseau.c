#include "Reseau.h"
#include "Hachage.h"
#include "arbreQuat.h"
#include <time.h>

void menu()
{
	printf("-----------------------------------\n");
	printf("Que voulez-vous faire?\n");
	printf(" 0 : sortie\n");
	printf(" 1 : Reconstituer le reseau\n");
	printf(" 2 : Ecrire le contenu du reseau dans un fichier\n");
	printf(" 3 : Creer un fichier SVG en html pour visualiser le reseau\n");
	printf("> ");
}

int main(int argc, char **argv)
{
	char *nameFile;
	FILE *fread;
	FILE *fwrite;
	int n;
	int choice;
	Chaines *instance;
	Reseau *R;

	if (argc != 3){
		printf("Erreur format: %s <Instances_cha/nomFichier.cha> <1 (liste chainee) /2 (table de hachage) /3 (arbre)>\n", argv[0]);
		return 1;
	}

	nameFile = strdup(argv[1]);
	n = atoi(argv[2]);
	fread = fopen(nameFile, "r");
	
	if (fread == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
		return 1;
	}

	instance = lectureChaine(fread);
	R = NULL;
	fread = NULL;
	fwrite = NULL;

	do{
		menu();
		scanf(" %d", &choice);
		
		switch(choice){
			case 0:
				printf("Au revoir et revenez vite o/\n");
				break;
			case 1:
				printf("**********************************\n");
				switch (n){
					case 1:
						printf("Reconstitution avec liste chainee\n");
			
						R = reconstitueReseauListe(instance);
			
						printf("Reseau de l'instance %s reconstitue avec succes\n", nameFile);
						break;
					case 2:
						printf("Reconstitution avec table de hachage:\n");
			
						R = reconstitueReseauHachage(instance);
						
						printf("Reseau de l'instance %s reconstitue avec succes\n", nameFile);
						break;
					case 3:
						printf("Reconstitution avec arbre quaternaire\n");
						
						R = recreeReseauArbre(instance);
						
						printf("Reseau de l'instance %s reconstitue avec succes\n", nameFile);
						break;
			}
			break;
			case 2:
				printf("**********************************\n");
				if (R){
					fwrite = fopen("reconstitutionReseau.res", "w");
					ecrireReseauTxt(R, fwrite);
					printf("Ecriture du reseau\n");
				}
				else{
					printf("Vous n'avez pas encore cree votre reseau...\n");
				}
				break;
			case 3:
				printf("**********************************\n");
				if (R){
					afficheReseauSVG(R, "SVGReseau");
					printf("Creation du fichier html correspondant\n");
				}
				else{
					printf("Vous n'avez pas encore cree votre reseau...\n");
				}
				break;
			default:
				printf("Nous n'avons pas bien compris votre requete...\n");
			break;
		}
	}while (choice != 0);

	if (fwrite)
		fclose(fwrite);
	if (fread)
		fclose(fread);
	return 0;
}

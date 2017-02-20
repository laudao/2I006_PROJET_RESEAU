#include "Reseau.h"

int main(int argc, char **argv)
{
	char *nameFile;
	FILE *fread;
	FILE *fwrite;
	Chaines *instance;
	Reseau *R;

	if (argc != 3){
		printf("Erreur format: %s <nomFichier.cha> <1 (liste chainee) /2 (table de hachage) /3 (arbre)>\n", argv[0]);
		return 1;
	}

	nameFile = strdup(argv[1]);
	fread = fopen(nameFile, "r");

	if (fread == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
		return 1;
	}

	instance = lectureChaine(fread);

	if (strcmp(argv[2], "1") == 0){
		R = reconstitueReseauListe(instance);	
		fwrite = fopen("reconstitution.res", "w");
		ecrireReseauTxt(R, fwrite);
		afficheReseauSVG(R, "TestSVGReseau");
	}
	return 0;
}

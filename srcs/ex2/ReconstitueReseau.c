#include "Reseau.h"
#include "Hachage.h"

int main(int argc, char **argv)
{
	char *nameFile;
	FILE *fread;
	FILE *fwrite;
	int n;
	Chaines *instance;
	Reseau *R;

	if (argc != 3){
		printf("Erreur format: %s <nomFichier.cha> <1 (liste chainee) /2 (table de hachage) /3 (arbre)>\n", argv[0]);
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

	switch (n){
		case 1:
			R = reconstitueReseauListe(instance);	
			fwrite = fopen("reconstitutionListe.res", "w");
			ecrireReseauTxt(R, fwrite);
			afficheReseauSVG(R, "TestSVGReseauListe");
			break;
		case 2:
			R = reconstitueReseauHachage(instance);
			fwrite = fopen("reconstitutionHachage.res", "w");
			ecrireReseauTxt(R, fwrite);
			afficheReseauSVG(R, "TestSVGReseauHachage");
			break;
	}
	return 0;
}

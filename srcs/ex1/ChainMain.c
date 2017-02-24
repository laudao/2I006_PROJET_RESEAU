#include "Chaine.h"

int main(int argc, char **argv){
	char *nameFile;
	FILE *f1;
	FILE *f2;
	double total;
	double longueur;
	int nbPoints;

	if (argc != 2){
		printf("Erreur format: %s <nomFichier.cha>\n", argv[0]);
		return 1;
	}
	
	nameFile = strdup(argv[1]);

	f1 = fopen(nameFile, "r");

	if (f1 == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
		return 1;
	}

	Chaines *instance = lectureChaine(f1);

	f2 = fopen("Test.cha", "w");
	ecrireChaineTxt(instance, f2);

	longueur = longueurChaine(instance -> chaines);
	total = longueurTotale(instance);
	nbPoints = comptePointsTotal(instance);

	printf("Longueur totale: %f\n", total);
	printf("Longueur 1 : %f\n", longueur);
	printf("Nombre total de points: %d\n", nbPoints);

	afficheChaineSVG(instance, "TestSVG");
	return 0;
}

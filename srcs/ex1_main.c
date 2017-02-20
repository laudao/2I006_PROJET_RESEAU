#include "chaine.h"

int main(){
	FILE *f = fopen("00014_burma.cha", "r");

	if (f == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
		return 1;
	}

	Chaines *instance = lectureChaine(f);

	afficheChaines(instance);

	return 0;
}

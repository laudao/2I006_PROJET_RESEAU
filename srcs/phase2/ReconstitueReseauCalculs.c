#include "Reseau.h"
#include "Hachage.h"
#include "arbreQuat.h"
#include <time.h>

int main()
{
	char *nameFile;
	FILE *fread;
	FILE *f_temps_calcul;
	Chaines *instance;
	Reseau *R;
	clock_t temps_initial;
	clock_t temps_final;
	double temps_cpu;


	nameFile = "00014_burma.cha";
	
	fread = fopen(nameFile, "r");
	f_temps_calcul = fopen("Temps_de_calcul", "w");
	
	if (fread == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
		return 1;
	}

	instance = lectureChaine(fread);

	fprintf(f_temps_calcul, "Reconstitution avec liste chainee:\n");
	temps_initial = clock();
	R = reconstitueReseauListe(instance);	
	temps_final = clock();
	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
	fprintf(f_temps_calcul, "Temps cpu: %f\n", temps_cpu);	
	
	fprintf(f_temps_calcul, "Reconstitution avec table de hachage:\n");
	temps_initial = clock();
	R = reconstitueReseauHachage(instance);
	temps_final = clock();
	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
	fprintf(f_temps_calcul, "Temps cpu: %f\n", temps_cpu);
	
	fprintf(f_temps_calcul, "Reconstitution avec arbre quaternaire:\n");
	temps_initial = clock();
	R = recreeReseauArbre(instance);
	temps_final = clock();
	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
	fprintf(f_temps_calcul, "Temps cpu: %f\n", temps_cpu);

	fclose(fread);
	fclose(f_temps_calcul);
	return 0;
}

#include "Reseau.h"
#include "Hachage.h"
#include "arbreQuat.h"
#include <time.h>

int main()
{
	char *nameFile;
	FILE *fread;
	FILE *f_temps_liste;
	FILE *f_temps_hachage;
	FILE *f_temps_arbre;
	Chaines *instance;
	Reseau *R;
	clock_t temps_initial;
	clock_t temps_final;
	double temps_cpu;
	int i;
	char *tab_instances[] = {"Instances_cha/00022_ulysses.cha","Instances_cha/00100_USA-road-d-NY-1-100.cha", "Instances_cha/00150_ch.cha", "Instances_cha/07000_USA-road-d-NY.cha", "Instances_cha/10000_USA-road-d-NY.cha"};

	f_temps_liste = fopen("Temps_liste", "w");
	f_temps_hachage = fopen("Temps hachage", "w");
	f_temps_arbre = fopen("Temps_arbre", "w");
	
	for (i = 0; i < 5; i++){
		nameFile = tab_instances[i];
		fread = fopen(nameFile, "r");
		
		if (fread == NULL){
			fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nameFile);
			return 1;
		}
	
		
		instance = lectureChaine(fread);
		temps_initial = clock();
		R = reconstitueReseauListe(instance);	
		temps_final = clock();
		temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
		fprintf(f_temps_liste, "%f\n", temps_cpu);	
		
		temps_initial = clock();
		R = reconstitueReseauHachage(instance);
		temps_final = clock();
		temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
		fprintf(f_temps_hachage, "%f\n", temps_cpu);
		
		temps_initial = clock();
		R = recreeReseauArbre(instance);
		temps_final = clock();
		temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
		fprintf(f_temps_arbre, "%f\n", temps_cpu);
		fclose(fread);
	}

	fclose(f_temps_liste);
	fclose(f_temps_hachage);
	fclose(f_temps_arbre);
	return 0;
}

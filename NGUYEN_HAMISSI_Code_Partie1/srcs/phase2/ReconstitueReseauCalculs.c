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
	char *tab_instances[] = {"Instances_cha/00014_burma.cha", "Instances_cha/00048_att.cha", "Instances_cha/00052_berlin.cha","Instances_cha/00076_eil.cha","Instances_cha/00100_USA-road-d-NY-1-100.cha","Instances_cha/00101_eil.cha","Instances_cha/00144_pr.cha","Instances_cha/00150_ch.cha",
	"Instances_cha/00280_a.cha","Instances_cha/00417_fl.cha","Instances_cha/00493_d.cha",
	"Instances_cha/00575_rat.cha","Instances_cha/00783_rat.cha",
	"Instances_cha/01400_fl.cha","Instances_cha/02392_pr.cha","Instances_cha/03795_fl.cha", "Instances_cha/04461_fnl.cha","Instances_cha/05000_USA-road-d-NY.cha","Instances_cha/05934_rl.cha",
	"Instances_cha/07000_USA-road-d-NY.cha","Instances_cha/07397_pla.cha","Instances_cha/08000_USA-road-d-NY.cha","Instances_cha/09000_USA-road-d-NY.cha","Instances_cha/10000_USA-road-d-NY.cha"};

	R = NULL;
	f_temps_liste = fopen("Temps_liste", "w");
	f_temps_hachage = fopen("Temps_hachage", "w");
	f_temps_arbre = fopen("Temps_arbre", "w");
	
	for (i = 0; i < 24; i++){
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
		fprintf(f_temps_liste, "%d %f\n", R -> nbNoeuds, temps_cpu);	
		
		temps_initial = clock();
		R = reconstitueReseauHachage(instance);
		temps_final = clock();
		temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
		fprintf(f_temps_hachage, "%d %f\n", R -> nbNoeuds, temps_cpu);
		
		temps_initial = clock();
		R = recreeReseauArbre(instance);
		temps_final = clock();
		temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
		fprintf(f_temps_arbre, "%d %f\n", R -> nbNoeuds, temps_cpu);
		fclose(fread);
	}

	if (R)
		libere_reseau(R);

	fclose(f_temps_liste);
	fclose(f_temps_hachage);
	fclose(f_temps_arbre);
	return 0;
}

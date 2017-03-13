#include "Chaine.h"
#include "SVGwriter.h"
#include <float.h>

CellPoint *creerCellPoint(double x, double y){
	CellPoint *nouveau = (CellPoint *)malloc(sizeof(CellPoint));
	
	nouveau -> x = x;
	nouveau -> y = y;
	nouveau -> suiv = NULL;
	
	return nouveau;
}

CellChaine *creerCellChaine(int num)
{
	CellChaine *nouveau = (CellChaine *)malloc(sizeof(CellChaine));
	
	nouveau -> numero = num;
	nouveau -> points = NULL;
	nouveau -> suiv = NULL;
	
	return nouveau;
}

void detruireLE(CellChaine *L)
{
	CellPoint *curr;
	CellPoint *tmp;
	
	/* si la chaine contient bien une liste de points */
	if (L -> points){
		curr = L -> points;
		/* parcours de la liste de points */
		while (curr){
			tmp = curr -> suiv;
			/* liberation de la memoire associee au CellPoint courant */ 
			free(curr);
			curr = tmp;
		}
	}
	/* Liberation de la structure CellChaine*/
	free(L);
}

void ajout_en_fin_point(CellChaine *chaine, double x, double y)
{
	CellPoint *curr;
	/* Creation de l'element CellPoint*/
	CellPoint *nouveau_point = creerCellPoint(x, y);
	
	curr = chaine -> points;

	if (curr == NULL){
		chaine -> points = nouveau_point;
	}
	else{
		/* parcours de la liste de points jusqu'a atteindre le dernier */
		while (curr -> suiv){
			curr = curr -> suiv;	
		}

		/* l'element suivant le dernier est le nouvel element */
		curr -> suiv = nouveau_point;
	}
}

void ajout_en_tete_point(CellChaine *chaine, double x, double y)
{
	/* Creation de l'element CellPoint*/
	CellPoint *nouveau_point = creerCellPoint(x, y);

	/* l'element suivant ce point est le premier element de la liste de points de chaine*/
	nouveau_point -> suiv = chaine -> points;

	/* Le premier element de la liste de points de chaine est le nouveau point cree*/
	chaine -> points = nouveau_point;

}

void ajout_en_fin_chaine(CellChaine *ch, Chaines *instance)
{
	CellChaine *curr;
	curr = instance -> chaines;

	if (curr == NULL){
		instance -> chaines = ch;
	}
	else{
		/* parcours de la liste de chaines jusqu'a la derniere chaine */
		while (curr -> suiv){
			curr = curr -> suiv;
		}

		/* L'element suivant la derniere chaine est ch */
		curr -> suiv = ch;
	}
}

void ajout_en_tete_chaine(CellChaine *ch, Chaines *instance)
{
	/* L'element suivant ch est le premier element de la liste de chaines de instance */
	ch -> suiv = instance -> chaines;

	/* Le premier element de la liste de chaines de instance est ch */
	instance -> chaines = ch;

}

void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;
	
	/* premiere chaine de la liste chainee de chaines de instance */
	chaine_curr = C -> chaines;

	while (chaine_curr){
		/* premier point de la liste chainee de points de chaine_curr */
		point_curr = chaine_curr -> points;
		while (point_curr){
			if(point_curr -> x > *xmax){
				*xmax = point_curr -> x; 
			}
			 
			if(point_curr -> y > *ymax){
				*ymax = point_curr -> y; 
			}
			 
			if(point_curr -> x < *xmin){
				*xmin = point_curr -> x; 
			}
			if(point_curr -> y < *ymin){
				*ymin = point_curr -> y; 
			}
			 
			point_curr = point_curr -> suiv;
		}

		chaine_curr = chaine_curr -> suiv;
	}
}


void afficheChaines(Chaines *instance)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;
	printf("Gamma = %d\n", instance -> gamma);
	printf("Nombre de chaines = %d\n", instance -> nbChaines);

	/* premiere chaine de la liste chainee de chaines de instance */
	chaine_curr = instance -> chaines;

	while (chaine_curr){
		printf("\t Chaine numero %d\n", chaine_curr -> numero);
		/* premier point de la liste chainee de points de chaine_curr */
		point_curr = chaine_curr -> points;

		while (point_curr){
			printf("\t\t x = %f y = %f\n", point_curr -> x, point_curr -> y);
			point_curr = point_curr -> suiv;
		}

		chaine_curr = chaine_curr -> suiv;
	}
		
}

Chaines *initialiseChaines()
{
	Chaines *nouveau = (Chaines *)malloc(sizeof(Chaines));
	nouveau -> gamma = 0;
	nouveau -> nbChaines = 0;
	nouveau -> chaines = NULL;
	return nouveau;
}

Chaines* lectureChaine(FILE *f)
{
	char buffer[255];
	int i;
	int j;
	int nbChaines;
	int gamma;
	int nbPoints;
	CellChaine *chaine;
	Chaines *instance;
	double x;
	double y;
	
	// allocation de la structure Chaines
	instance = initialiseChaines();

	// on saute NbChain
	GetChaine(f, 255, buffer);
	Skip(f);

	nbChaines = GetEntier(f);
	instance -> nbChaines = nbChaines;


	// on saute Gamma:
	GetChaine(f, 255, buffer);
	Skip(f);
	
	gamma = GetEntier(f);
	instance -> gamma = gamma;
	
	SkipLine(f);	
	for (i = 0; i < nbChaines; i++){
		/* allocation d'une chaine */	
		chaine = creerCellChaine(GetEntier(f));
	
		Skip(f);
		
		nbPoints = GetEntier(f);
		Skip(f);
		
		for (j = 0; j < nbPoints; j++){
			x = GetReel(f);
			y = GetReel(f);
			ajout_en_fin_point(chaine, x, y);
		}
			
		ajout_en_fin_chaine(chaine, instance);
		SkipLine(f);
	}

	return instance;
}

int comptePoints(CellChaine *ch)
{
	int cpt;
	CellPoint *point;
	point = ch -> points;
	cpt = 0;

	while (point){
		cpt++;
		point = point -> suiv;
	}

	return cpt;
}

void ecrireChaineTxt(Chaines *C, FILE *f)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;

	fprintf(f, "NbChain: %d\n", C -> nbChaines);
	fprintf(f, "Gamma: %d\n", C -> gamma);
	fprintf(f, "\n");
	/* premiere chaine de la liste chainee de chaines de C */
	chaine_curr = C -> chaines;

	while (chaine_curr){
		fprintf(f, "%d ", chaine_curr -> numero);
		fprintf(f, "%d ", comptePoints(chaine_curr));

		/* premier point de la liste chainee de points de chaine_curr */
		point_curr = chaine_curr -> points;

		while (point_curr){
			fprintf(f, "%.2f %.2f ", point_curr -> x, point_curr -> y);
			point_curr = point_curr -> suiv;
		}
		fprintf(f, "\n");

		chaine_curr = chaine_curr -> suiv;
	}
		
}

void afficheChaineSVG(Chaines *C, char *nomInstance)
{
	CellChaine *chaine_curr;
	CellPoint *point_curr;
	double xmin, ymin, xmax, ymax;
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
	double xnormalise;
	double ynormalise;
	double xnormalisesuiv;
	double ynormalisesuiv;

	SVGwriter *svg = (SVGwriter *)malloc(sizeof(SVGwriter));

	/* initialise svg avec nomInstance pour nom du fichier html a creer
		et avec une taille de 500x500 pixels */
	SVGinit(svg, nomInstance, 5000, 5000);
	SVGlineRandColor(svg); /* couleur aleatoire pour les lignes */
//	SVGlineColor(svg, Black);
	SVGpointColor(svg, Red); /* fixe la couleur des points a rouge */

	/* premiere chaine de la liste chainee de chaines de C */
	chaine_curr = C -> chaines;

	chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);

	while (chaine_curr){
		/* premier point de la liste chainee de points de chaine_curr */
		point_curr = chaine_curr -> points;

		while (point_curr){
			/* Ecrit point_curr dans le fichier */
			xnormalise = (point_curr -> x - xmin)*(5000)/(xmax-xmin);
			ynormalise = (point_curr -> y - ymin)*(5000)/(ymax-ymin);
			SVGpoint(svg,xnormalise,ynormalise);

			/* si point_curr est suivi d'un autre point */
			if (point_curr -> suiv){
				/* relier les deux */
				xnormalisesuiv = (point_curr -> suiv -> x - xmin)*(5000)/(xmax-xmin);
				ynormalisesuiv = ( point_curr -> suiv -> y - ymin)*(5000)/(ymax-ymin); 
				SVGline(svg, xnormalise, ynormalise, xnormalisesuiv, ynormalisesuiv);
			}
			point_curr = point_curr -> suiv;
		}

		chaine_curr = chaine_curr -> suiv;
	}

	SVGfinalize(svg);
	
}

double longueurChaine(CellChaine *C)
{
	double sommeLongueurs;
	double xA, yA;
	double xB, yB;
	CellPoint *point = C -> points;

	sommeLongueurs = 0;
	while (point){
		if (point -> suiv){
			xA = point -> x;
			yA = point -> y;
			xB = point -> suiv -> x;
			yB = point -> suiv -> y;
			sommeLongueurs = sommeLongueurs + sqrt((xB - xA)*(xB - xA) + (yB - yA)*(yB - yA));
		}
		point = point -> suiv;
	}

	return sommeLongueurs;
}

double longueurTotale(Chaines *C)
{
	double total;
	CellChaine *chCurr;
	
	total = 0;
	chCurr = C -> chaines;

	while (chCurr){
		total = total + longueurChaine(chCurr);
		chCurr = chCurr -> suiv;
	}

	return total;
}

int comptePointsTotal(Chaines *C)
{
	CellChaine *chCurr;
	CellPoint *pointCurr;
	int nbPoints;
	
	nbPoints = 0;
	chCurr = C -> chaines;
	

	while (chCurr){
		pointCurr = chCurr -> points;
		while (pointCurr){
			nbPoints++;
			pointCurr = pointCurr -> suiv;
		}
		chCurr = chCurr -> suiv;
	}

	return nbPoints;
}

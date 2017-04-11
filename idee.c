/* Enfaite l'algo de D fait deja une grande partie de travaille qui est trouver le plus court chemain. 
On reprend l'algo de D sauf que on commance par appeler la fonction d'evaluation gamma , une fois cette fonction appelée on aura une valeure de calc_gamma pour chaque arrete.
Ensuite dans la fonction maj_bordure lors de la comparaison avec la valeur de longueur on ajoute aussi la valeure de calc_gamma ce qui permet a notre algo de choisire à chaque fois l'arrete avec ( calc_gamma,longueur ) minimisé.*/


void maj_bordure(Graphe *G, int *pred, int *marque, int *lambda, Tas2Clefs *bordure, int s){
	int i;
	int val_compare;
	Cellule_arete *coura = G->T_som[s]->L_voisin;
	while (coura != NULL){ /* parcours des voisins */
		
		i = coura->a->v;
		if (i == s) i = coura->a->u;
		
		/* si le sommet n'a pas ete visite et qu'on a trouve un chemin plus interessant pour y acceder */
		val_compare = coura->a->longueur+ calc_gamma;
		
		if ((marque[i] == 0) && (lambda[i] > lambda[s] + val_compare)){
			lambda[i] = lambda[s] + val_compare;
			pred[i] = s;
			insert(bordure, i, lambda[i]); /* insertion dans la bordure */
		}
		
		coura = coura->suiv;
	}
}



int plus_court_chemain_longeur(Graphe *G,int r, int u){
		
	evaluation_gamma(Graphe *G);
	
	Tas2Clefs bordure;
	/* tableau de liste d'entiers pour stocker le plus court chemin de r aux autres sommets*/
	ListeEntier *L = (ListeEntier*)malloc((G->nbsom+1)*sizeof(ListeEntier));
	/* tableau des valeurs des plus courts chemins de racine r*/
	int *lambda = (int*)malloc(sizeof(int)*(G->nbsom+1)); 
	/* tableau des predecesseurs dans l'arborescence des plus courts chemins de racine r*/
	int *pred = (int*)malloc(sizeof(int)*(G->nbsom+1));
	int *marque = (int*)malloc(sizeof(int)*(G->nbsom+1)); /* indique si le sommet a ete visite ou non*/
	int i;
	int s;
	
	init(&bordure, G->nbsom); 
	
	for (i=1; i<=G->nbsom; i++){
		pred[i] = -1;
		marque[i] = 0;
		Init_Liste(&(L[i])); /*pk ?*/
	}
	
	insert(&bordure, r, 0);
					/*l'utilité de u ?*/
	while ((bordure.n != 0) && (s != u)){ /* tant que la bordure n'est pas vide */
		s = supprMin(&bordure); /* on enleve l'element de plus courte distance de r */
		marque[s] = 1;
		if (s != u){
			maj_bordure(G, pred, marque, lambda, &bordure, s);
		}
	}
	
	free(lambda);
	free(marque);
	return pred;
}

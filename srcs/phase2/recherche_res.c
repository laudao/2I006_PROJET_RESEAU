Reseau* recreeReseauArbre(Chaines* C){

	CellChaine *chaine; /* chaine courante */
	CellPoint *point; /* point courant dans la chaine courante */
	Noeud *noeudCurr; /* noeudCourant correspondant au point courant */
	Noeud *extrA; /* extremite A d'une chaine */
	Noeud *extrB; /* extremite B d'une chaine */
	Noeud *prec; /* element precedent d'un noeud dans une chaine */
	xmin=0;
	ymin=0;
	xmax=0;
	ymax=0;
	ArbreQuat** p_Arbre;
	ArbreQuat* parent=NULL;

	/* initialisation du reseau R */
	Reseau *R = (Reseau *)malloc(sizeof(Reseau));
	R -> nbNoeuds = 0;
	R -> gamma = C -> gamma;
	R -> noeuds = NULL;
	R -> commodites = NULL;
	
	/* initialisation de l'arbre uaternaire Arbre */
	chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);
	ArbreQuat* Arbre = creerArbreQuat(xmax-xmin/2,ymax-ymin/2,xmax-xmin,ymax-ymin);
	*p_Arbre = Arbre;

	chaine = C -> chaines;

	/* on parcourt chaque point de chaque chaine de C et on l'ajoute a la liste de noeuds de R s'il n'est pas present */
	while (chaine){
		point = chaine -> points;
		prec = NULL;

		/* extremite de la chaine */
		if (point){
			extrA = chercherNoeudArbre(point,R,p_Arbre,parent);
		}

		while (point){
			
			/* on ajoute le noeud si on ne l'a pas deja rencontre */
			/* noeudCurr = le Noeud correspondant a point */
			noeudCurr  = chercherNoeudArbre(point,R,p_Arbre,parent);

			/* on ajoute ses voisins */
			if (prec){
				ajoutCellNoeudVoisin(noeudCurr, prec); /* son voisin precedent */
			}
			/* son voisin suivant */
			if (point -> suiv){
				ajoutCellNoeudVoisin(noeudCurr,chercherNoeudArbre(point->suiv,R,p_Arbre,parent));
			}

			prec = noeudCurr;
			point = point -> suiv;
		}

		/* la chaine compte plus d'un point */
		if (comptePoints(chaine) > 1){
			extrB = prec;
			ajoutCellCommodite(R, creerCellCommodite(extrA, extrB));			
		}

		chaine = chaine -> suiv;
	}

	return R;
}




















}

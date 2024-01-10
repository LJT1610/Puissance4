//////////////////////////////////////////
// Code fourni:

#ifndef _DEFAULT_SOURCE // pour usleep()
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // pour time()
#include <unistd.h> // pour usleep()

#define LIGNES 6
#define COLONNES 7
#define MAX_JOUEURS 7 // ne pas modifier
#define CASE_VIDE '.'

void effaceConsole(void);
void afficheCaseEnCouleur(char c);
void attendre(double duree);
void initGrille(char grille[LIGNES][COLONNES]);
void afficheGrille(const char grille[LIGNES][COLONNES], int jetons, int nbHumains, int nbBots, int typeVictoire);
bool estDansGrille(int ligne, int colonne);
bool grillePleine(const char grille[LIGNES][COLONNES]);
bool victoire(const char grille[LIGNES][COLONNES], int jetons, int ligne, int colonne);
int compteDansDirection(const char grille[LIGNES][COLONNES], int ligne, int colonne, int sautLigne, int sautColonne);
int placeJeton(char grille[LIGNES][COLONNES], int colonne, int joueur);
int saisieColonne(const char grille[LIGNES][COLONNES], int joueur, bool estHumain, int nbJoueurs, int jetons, int typeVictoire);
int saisieNombreJoueursHumains(void);
int saisieNombreJetonsGagnants(void);
int lancerPartie(void);
int getint(void);

int nbHumains, nbBots, typeVictoire, jetons, colonne, resultat, ligne, nbJoueurs=1, joueur=1;

int main(void){
    srand(time(NULL));
	lancerPartie();
	return 0;
}
int saisieNombreJoueursHumains(void){
    printf("combien de joueurs humains voulez vous ?");
    nbHumains = getint();

    while(1){
		if (nbHumains >= 1 && nbHumains <= MAX_JOUEURS){
        	return nbHumains;
    	}

    	else{

        	printf("Erreurs veuillez rentré un nombre de joueurs entre 1 et 7\n");
        	printf("Combien de joueurs humains voulez-vous ?\n");
            nbHumains = getint();
           
    	}
	}

	return nbHumains;

}

int saisieNombreJetonsGagnants(void) {

    printf("Combien de jetons gagnants souhaitez-vous ?");
    jetons = getint();

    while (1){

        if (jetons >= 3 && jetons <= 6){
            return jetons;
        }
        else{

            printf("Erreur : veuillez entrer une valeur entre 3 et 6.\n");
            printf("Combien de jetons gagnants souhaitez-vous ?\n");
            jetons = getint();
        }

    }
    return jetons;
}




void initGrille(char grille[LIGNES][COLONNES]){

    for(int i = 0; i < LIGNES; i++){
        for(int j = 0; j < COLONNES; j++){
            grille[i][j] = CASE_VIDE;
        }
    }

}

void effaceConsole(void){

	if (system("clear") != 0) {
		printf("La console n'a pas pu etre effacee.\n");
	}

}
void afficheGrille(const char grille[LIGNES][COLONNES], int jetons, int nbHumains, int nbBots, int typeVictoire){

    printf("Type Victoire : classique %c \n", typeVictoire);
    printf("Il y a %d humains et %d Bots \n", nbHumains, nbBots);
    printf("puissance %d \n",jetons);
    for(int i=0; i < LIGNES; i++)
        {
        for(int j = 0; j < COLONNES; j++){
            printf("%4c",grille[i][j]);
            }
        printf("\n");
    }

}

bool grillePleine(const char grille[LIGNES][COLONNES]) {

    for (int i = 0; i < LIGNES; ++i) {
        for (int j = 0; j < COLONNES; ++j) {
            if (grille[i][j] == CASE_VIDE) {
                return false;
            }
        }
    }

    return true;
}

int saisieColonne(const char grille[LIGNES][COLONNES], int joueur, bool estHumain, int nbJoueurs, int jetons, int typeVictoire) {

    int colonne;
    if (estHumain) {
        do {
            printf("Joueur %d, veuillez saisir la colonne : ", joueur);
            colonne = getint();
            if (colonne < 0 || colonne >= COLONNES || grille[0][colonne] != CASE_VIDE) {
                printf("Saisie invalide. Veuillez choisir une colonne valide.\n");
            }
        } while (colonne < 0 || colonne >= COLONNES || grille[0][colonne] != CASE_VIDE);
    } else {
        if (typeVictoire != 0) {
            colonne = rand() % COLONNES;
        } else {
            int colonnesValides[COLONNES];
            int nbColonnesValides = 0;
            for (int i = 0; i < COLONNES; ++i) {
                if (grille[0][i] == CASE_VIDE) {
                    colonnesValides[nbColonnesValides++] = i;
                }
            }
            colonne = colonnesValides[rand() % nbColonnesValides];
        }
    }
    return colonne;
}

int placeJeton(char grille[LIGNES][COLONNES], int colonne, int joueur) {
    int i = LIGNES ;
    while (i >= 0) {
        i--;
        if (grille[i][colonne] == CASE_VIDE) {
            grille[i][colonne] = joueur + '0'; 
            i = 0;
        }
    }
    return 0;
} 

bool estDansGrille(int ligne, int colonne) {

    return (ligne >= 0 && ligne < LIGNES && colonne >= 0 && colonne < COLONNES);

}


int compteDansDirection(const char grille[LIGNES][COLONNES], int ligne, int colonne, int sautLigne, int sautColonne) {
    if (sautLigne == 0 && sautColonne == 0) {
        return 1;
    }

    int count = 1;

    while (1) {
        ligne += sautLigne;
        colonne += sautColonne;

        if (!estDansGrille(ligne, colonne)) {
            return count;
        }

        if (grille[ligne][colonne] == grille[ligne - sautLigne][colonne - sautColonne]) {
            count++;
        } else {
            return count;
        }
    }
}

bool victoire(const char grille[LIGNES][COLONNES], int jetons, int ligne, int colonne) {
     ligne = LIGNES - 1;

    int directions[][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

    for (int i = 0; i < 8; ++i) {
        int count = compteDansDirection(grille, ligne, colonne, directions[i][0], directions[i][1]);

        if (count >= jetons) {
            return true;
        }
    }

    return false;
}


        /*

        if (typeVictoire != 0 && count == jetons - 1) {

            int oppositeCount = compteDansDirection(grille, ligne, colonne, -directions[i][0], -directions[i][1]);

            if (oppositeCount > 0) {

                return true;

            }

        }

        */



void afficheCaseEnCouleur(char c)
{
	if ('0' <= c && c <= '9')
		printf("\e[1;%dm%c\e[0m ", c-'0'+91, c); // 7 couleurs donc 7 joueurs max.
	else
		printf("%c ", c);
}

// Attendre 'duree' secondes avant de continuer.
void attendre(double duree)
{
	// usleep() permet d'attendre un nombre entier de micro-secondes,
	// a contrario de sleep() qui n'attend qu'un nombre de secondes:
	duree = duree < 0. ? 0. : duree;
	usleep(duree * 1000000.);
}
// ...

// ...

int lancerPartie(void) {
    char grille[LIGNES][COLONNES];
    nbHumains = saisieNombreJoueursHumains();
    jetons = saisieNombreJetonsGagnants();
    initGrille(grille);

    while (1) {

        effaceConsole();
        afficheGrille(grille, jetons, nbHumains, nbBots, typeVictoire);

        if (grillePleine(grille)) {
            printf("La grille est pleine. Match nul !\n");
            return -1;
        }

        colonne = saisieColonne(grille, joueur, true, nbHumains, jetons, typeVictoire);
        resultat = placeJeton(grille, colonne, joueur);

        if (resultat == -1) {
            printf("La colonne est pleine. Veuillez choisir une autre colonne.\n");
            continue;
        }
        char c = grille[resultat][colonne];
		afficheCaseEnCouleur(c);
        if (victoire(grille, jetons, resultat, colonne)) {
            effaceConsole();
            afficheGrille(grille, jetons, nbHumains, nbBots, typeVictoire);
            printf("Le Joueur %d a gagné !\n", joueur);
            return nbJoueurs;
        }

        if (joueur<nbHumains){
        	joueur++;
        }
        else{
        	joueur=1;
        }
    }
}

int getint(void){
    int input = 0, c;
    while(scanf("%d", &input) != 1){
        while ((c = getchar()) != '\n' && c != EOF) { }
        printf("Veuillez entrer un int : ");
    }
    return input;
}
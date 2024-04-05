#ifndef __TERRAIN__
#define  __TERRAIN__
#include <MLV/MLV_all.h>

#define HAUTEUR 22
#define LARGEUR 28
#define TAILLE_CASE 25
#define TAILLE_MONSTRE_RAYON 5


/**
 Enumeration representing the square's types that can appear inside the grid
*/
typedef enum {
    EMPTY,      // Case vide
    //WALL,       Mur ( Bordure à ne pas franchire )
    MONSTER,    // Le nid des monstres
    JOUEUR,    // Le camps à defendre
    TOUR,
    BUSY      // Case contenant une case du chemin
} caseType;


/**
 Definition of the possible directions
*/
typedef enum {
    NORD,
    SUD,
    EST,
    OUEST
} Card;


/**
 Initialize the grid with empty squares
*/
void initialise_grille(caseType grille[LARGEUR][HAUTEUR]);


/**
 Initialize the terminal with the grid
*/
void initialise_terminal(caseType grille[LARGEUR][HAUTEUR]);


/**
 Generate a random number between min and max
*/
int nombre_aleatoire(int min, int max);


/**
 Create a road in the grid to a given direction
*/
int chemin(int etendu, int *x, int *y, int direction, caseType grille[LARGEUR][HAUTEUR]);


/**
 Determinate the road's direction
*/
int direction(int x, int y, caseType grille[LARGEUR][HAUTEUR]);


/**
 initialize the monster's nest inside the grid
*/
void nid_de_monstre(caseType grille[LARGEUR][HAUTEUR]);

#endif

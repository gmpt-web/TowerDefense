#include "terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




/**
 * Initialize all the square of the grid with 'EMPTY'.
*/
void initialise_grille(caseType grille[LARGEUR][HAUTEUR]){
    int i, j;
    for(i =0; i < LARGEUR; ++i){
        for(j = 0; j < HAUTEUR; ++j){
            grille[i][j] = EMPTY;
        }
    }
}



/**
 * Display the game's grid on the terminal.
*/
void initialise_terminal(caseType grille[LARGEUR][HAUTEUR]){
    int i, j;
    for(i = 0; i < LARGEUR; ++i){
        for(j = 0; j < HAUTEUR; ++j){
            if(grille[i][j] == MONSTER){
                fprintf(stdout, "M ");
            }else if(grille[i][j] == EMPTY){
                fprintf(stdout, "E ");
            }else if(grille[i][j] == BUSY){
                fprintf(stdout, "B ");
            }else if(grille[i][j] == JOUEUR){
                fprintf(stdout, "J ");
            }else if(grille[i][j] == TOUR){
                fprintf(stdout, "T ");
            }else{
                fprintf(stdout, "W ");
            }
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}



/**
 * Function that give a random number in a given range.
*/
int nombre_aleatoire(int min, int max) {
    return min + rand() % (max - min + 1);
}



/**
 * The function etendu_vers_nord calculates the possible extend to the north from a given position in the grid.
 * The function check if the square on the north until it found a square who is not empty or has reached the limit of the grid.
 * @param x: The coordinate x of the starting point.
 * @param y: The coordinate y of the starting point.
 * @return: The number of empty squares consecutive to the north from the starting point.
*/
int etendu_vers_nord(int x, int y, caseType grille[LARGEUR][HAUTEUR]) {
    int j, n = 0;
    int i = x;
    for (j = y - 1; j > 2 && i >= 2 && i < LARGEUR - 2; j--) {
        if (grille[i][j] == EMPTY && grille[i][j - 1] == EMPTY
            && grille[i][j - 2] == EMPTY && grille[i - 1][j] == EMPTY
            && grille[i - 2][j] == EMPTY && grille[i + 1][j] == EMPTY
            && grille[i + 2][j] == EMPTY && grille[i - 1][j - 1] == EMPTY
            && grille[i + 1][j - 1] == EMPTY) {
            n++;
        } else {
            return n;
        }
    }
    return n;
}


/**
 * Calculate the extend of the road to the south.
*/
int etendu_vers_sud(int x, int y, caseType grille[LARGEUR][HAUTEUR]) {
    int j, n = 0;
    int i = x;
    for (j = y + 1; j < HAUTEUR - 2 && i >= 2 && i < LARGEUR - 2; j++) {
        if (grille[i][j] == EMPTY && grille[i][j + 1] == EMPTY
            && grille[i][j + 2] == EMPTY && grille[i - 1][j] == EMPTY
            && grille[i - 2][j] == EMPTY && grille[i + 1][j] == EMPTY
            && grille[i + 2][j] == EMPTY && grille[i - 1][j + 1] == EMPTY
            && grille[i + 1][j + 1] == EMPTY) {
            n++;
        } else {
            return n;
        }
    }
    return n;
}


/**
 * Calculate the extend of the road to the West.
*/
int etendu_vers_ouest(int x, int y, caseType grille[LARGEUR][HAUTEUR]) {
    int i, n = 0;
    int j = y;
    for (i = x - 1; i > 2 && j >= 2 && j < HAUTEUR - 2; i--) {
      if (grille[i][j] == EMPTY && grille[i - 1][j] == EMPTY
          && grille[i - 2][j] == EMPTY && grille[i][j - 1] == EMPTY
          && grille[i][j - 2] == EMPTY && grille[i][j + 1] == EMPTY
          && grille[i][j + 2] == EMPTY && grille[i - 1][j - 1] == EMPTY
          && grille[i - 1][j + 1] == EMPTY) {
          n++;
      } else {
            return n;
        }
    }
    return n;
}


/**
 * Calculate the extend of the road to the East.
*/
int etendu_vers_est(int x, int y, caseType grille[LARGEUR][HAUTEUR]) {
    int i, n = 0;
    int j = y;
    for (i = x + 1; i < LARGEUR - 2 && j >= 2 && j < HAUTEUR - 2; i++) {
      if (grille[i][j] == EMPTY && grille[i + 1][j] == EMPTY
          && grille[i + 2][j] == EMPTY && grille[i][j - 1] == EMPTY
          && grille[i][j - 2] == EMPTY && grille[i][j + 1] == EMPTY
          && grille[i][j + 2] == EMPTY && grille[i + 1][j - 1] == EMPTY
          && grille[i + 1][j + 1] == EMPTY) {
          n++;
      } else {
            return n;
        }
    }
    return n;
}


/**
 * The function chemin create a road inside the grid depending on the extend and the given direction.
 * @param etendu: The extent of the road that have to be created.
 * @param direction: The direction in with the road will be created.
 * @param grille: The grid where the road will be created.
 * @return: The number of square traveled in the given direction.
*/
int chemin(int etendu, int *x, int *y, int direction, caseType grille[LARGEUR][HAUTEUR]){
    int i;
    int nb = rand() % etendu;
    double proba_succes = 0.75;
    int s = 0;
    for (int i = 0; i < nb/*etenduCourante*/; ++i) {
        s += (rand() < proba_succes * RAND_MAX) ? 1 : 0;
    }
    int plus = s > 3 ? s : 3;
    switch (direction) {
        case NORD:
            for(i = 0; i < plus; ++i){
                if(grille[*x][*y - 3] != EMPTY || *y <= 2) return i - 1;
                *y = *y - 1;
                grille[*x][*y] = BUSY;
            }
            break;
        case EST:
            for(i = 0; i < plus; ++i){
                if(grille[*x + 3][*y] != EMPTY || *x <= 2) return i - 1;
                *x = *x + 1;
                grille[*x][*y] = BUSY;
            }
            break;
        case SUD:
            for(i = 0; i < plus; ++i){
                if(grille[*x][*y + 3] != EMPTY || *y > HAUTEUR - 3) return i - 1;
                *y = *y + 1;
                grille[*x][*y] = BUSY;
            }
            break;
        case OUEST:
            for(i = 0; i < plus; ++i){
                if(grille[*x - 3][*y] != EMPTY || *x > LARGEUR - 3) return i - 1;
                *x = *x - 1;
                grille[*x][*y] = BUSY;
            }
            break;
        default:
            break;
    }
    return plus;
}


/**
 * The function direction determine the direction of the road that have to be created inside the grid.
 * @param x: The coordinate x of the starting point of the road.
 * @param y: The coordinate y of the starting point of the road.
 * @param grille: The grid wher ethe road need to be created.
 * @return: 0 if the road contain at least 7 turns and 75 squares, else, return -1.
 */
int direction(int x, int y, caseType grille[LARGEUR][HAUTEUR]){
    int nb_virage = -1, taille = 0, ajout;
    int random = rand() % 4;
    int cmt = 0;
    int Last = -1;
    int etendue;  // Etendue du chemin vers une direction donnée
    while(1){
        switch (random){
            case 0:
                if(Last == SUD || (etendue = etendu_vers_nord(x, y, grille)) < 3) {
                    break;}
                ajout = chemin(etendue, &x, &y, random, grille);
                if(ajout != 0 && Last != NORD) nb_virage++;
                taille += ajout;
                Last = random;
                break;
            case 1:
                if(Last == NORD || (etendue = etendu_vers_sud(x, y, grille)) < 3) { break; }
                ajout = chemin(etendue, &x, &y, random, grille);
                if(ajout != 0 && Last != SUD) nb_virage++;
                taille += ajout;
                Last = random;
                break;
            case 2:
                if(Last == OUEST || (etendue = etendu_vers_est(x, y, grille)) < 3) {
                    break;}
                ajout = chemin(etendue, &x, &y, random, grille);
                if(ajout != 0 && Last != EST) nb_virage++;
                taille += ajout;
                Last = random;
                break;
            case 3:
                if(Last == EST || (etendue = etendu_vers_ouest(x, y, grille)) < 3) {
                    break;}
                ajout = chemin(etendue, &x, &y, random, grille);
                if(ajout != 0 && Last != OUEST) nb_virage++;
                taille += ajout;
                Last = random;
                break;
            case 4:
                if((nb_virage >= 7 && taille >= 75)) {
                    grille[x][y] = JOUEUR;
                    return 0;
                    }
                break;
            default:
                break;
        }
        if(etendu_vers_nord(x,y, grille) < 3 && etendu_vers_est(x,y, grille) < 3
        && etendu_vers_sud(x,y, grille) < 3 && etendu_vers_ouest(x,y, grille) < 3) return -1;
        random = rand() % 5;
        cmt++;
    }
}


/**
 * The function nid_de_monstre crée un nid de monstre dans la grille.
 * The function chose a random position for the nest, after that, it will try to create a road from this position.
 * If the road's creation fail, it will reset the grid and retry until a valid path is created.
 * @param grille: The grid on which the monster's nest and the road need to be created.
*/
void nid_de_monstre(caseType grille[LARGEUR][HAUTEUR]){
    int x = nombre_aleatoire(3, 25);
    int y = nombre_aleatoire(3, 19);
    grille[x][y] = MONSTER;
    while(direction(x, y, grille) == -1){
        initialise_grille(grille);
        x = nombre_aleatoire(3, 25);
        y = nombre_aleatoire(3, 19);
        grille[x][y] = MONSTER;
    }
}

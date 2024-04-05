#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <MLV/MLV_all.h>
#include "moteur.h"
#include "graphique.h"
#include "terrain.h"
#include "vague.h"
#include "mana.h"
#include "game.h"






/**
 * @mainpage Title of the main page.
 *
 * This is the content of the main page.
*/

/**
Displays instructions for the player in the terminal
*/
void instructions_joueur() {
    printf("Pour déclencher une vague de monstres:\n");
    printf("  **APPUYER SUR LA TOUCHE ESPACE**\n\n");

    printf("Pour créer des gemmes:\n");
    printf("  **Cliquer gauche sur \"Create\" en bas à droite du terrain**\n\n");

    printf("Pour augmenter de niveau:\n");
    printf("  **Cliquer sur \"Upgrade\" en bas à gauche du terrain**\n\n");

    printf("Pour poser une tour:\n");
    printf("  **Appuyer sur la touche t, une tour s'affiche, manipuler la tour comme bon vous semble**\n");
    printf("  **avec la souris, clique-gauche pour poser la tour sur un emplacement, clique-droit sinon.**\n\n");

    printf("Pour poser une gemme sur une tour:\n");
    printf("  **Cliquer gauche sur la gemme, elle est sous emprise, deplacer la gemme avec la souris**\n");
    printf("  **clique-gauche pour la poser sur une tour, clique-droite sinon.**\n");
}


int main(int argc, char *argv[]) {
    srand(time(NULL));
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
            printf("Aide :\n");
            instructions_joueur();
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, "Utilisation : %s [-h] \n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    caseType grille[LARGEUR][HAUTEUR];
    initialise_grille(grille);

    MLV_create_window("Grille", "", LARGEUR * TAILLE_CASE, (HAUTEUR + 4) * TAILLE_CASE);
    MLV_clear_window(MLV_COLOR_GRAY55);
    MLV_update_window();

    initialise_terrain();
    MLV_update_window();
    nid_de_monstre(grille);

    Game game = init_game();
    TableVague tabVague = init_TabVague(grille);
    Reserve mana = init_mana();

    boucle_principale(&game, &tabVague, &mana, grille);

    MLV_wait_seconds(1);
    MLV_free_window();

    return 0;
}

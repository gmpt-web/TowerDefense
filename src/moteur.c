#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <MLV/MLV_all.h>
#include "moteur.h"



/**
 * The function event_clavier manages the events with the keyboard during the game.
 * The function check if the touch 'space', 't', or 'q' are pressed and performs the coresponding action.
*/
void event_clavier(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* depart, int* firstcall, int* derniere_appel_ensemble,
  int* appel, int *emprise, int *placement, int milliseconde, int x, int y, MLV_Keyboard_button button) {
        if (button == MLV_KEYBOARD_SPACE) {
            if (*depart == 0) {
                *depart = 1;
            }
            *derniere_appel_ensemble = *firstcall;
            *firstcall = milliseconde;
            (*appel)++;
        } else if (button == MLV_KEYBOARD_t) {
            if (*placement == 0) {
                if (*emprise == 0) {
                    tour(x, y);
                    *placement = 1;
                }
            }
        } else if (button == MLV_KEYBOARD_q) {
            exit(0);
        }
}



/**
 * The function gestion_clic_droit manages the events with the right click during the game.
 * The function reset the influence et placement of the selectionned gem.
*/
void gestion_clic_droit(Game* game, int* pos_gemme, int* placement, int* emprise) {
    game->gemmes[*pos_gemme].emprise = 0;
    *placement = 0;
    *emprise = 0;
}



/**
 * The function placement_tour_terrain place a tower on the field at a given position.
 * At first, the function will check if the square is empty, after that it will try to add a tower at this position.
 * If the addition is succeful, the function will update the square of the grid and will indicate that there is a tower here.
*/
void placement_tour_terrain(Game* game, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int x, int y) {
    if (grille[x / TAILLE_CASE][y / TAILLE_CASE] == EMPTY) {
        if (AjoutTour(game, &(mana->quantite), x / TAILLE_CASE, y / TAILLE_CASE) == 1) {
            grille[x / TAILLE_CASE][y / TAILLE_CASE] = TOUR;
        }
    }
}



/**
 * The function gemme_fusion manages the merging of two gems.
 * If the player have enough mana, the function will try to merge the selctionned gem with the gem inside the tower.
 * If the merging is a succes, the function will reduce the mana of the player depending on the cost,
   update the time of the placement of the gem and rearrange the player's gems.
 * If the player don't have enough mana, the function will unselect the gem.
*/
void gemme_fusion(Game* game, Reserve* mana, int* emprise, int* pos_gemme, int milliseconde, int j) {
    if (mana->quantite >= 100) {
        if (fusion(&(game->tours[j].gemme), game->gemmes[*pos_gemme]) == 0) {
            mana->quantite -= 100;
            game->tours[j].time_installation = milliseconde;
            rearrange_gemmes(game->gemmes, *pos_gemme, --game->nb_gemmes);
            *emprise = 0;
        }
    } else {
        game->gemmes[*pos_gemme].emprise = 0;
    }
}


/**
 * The function placement_gemme manages the placement of a gem inside a tower.
 * If the tower is empty, it will place the selected gem inside the tower,
   update the time of placement of the gem on the tower and rearrange the player's gems.
 * If the tower already have a gem, it will try to merge the selected gem with the tower's gem.
*/
void placement_gemme(Game* game, Reserve* mana, int* emprise, int* pos_gemme, int milliseconde, int j) {
    if (game->tours[j].pret == 0) {
        game->tours[j].gemme = game->gemmes[*pos_gemme];
        game->tours[j].time_installation = milliseconde;
        game->tours[j].pret = 1;
        rearrange_gemmes(game->gemmes, *pos_gemme, --game->nb_gemmes);
        *emprise = 0;
    } else {
        gemme_fusion(game, mana, emprise, pos_gemme, milliseconde, j);
    }
}


/**
 * The function emprise_sur_gemme manages the placement of a gem inside a tower when a gem is selected (emprise == 1).
 * The function check if the compartment where mouse is has a tower,
   after it will try to place the gem in the tower at this position.
*/
void emprise_sur_gemme(Game* game, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* emprise, int* pos_gemme, int milliseconde, int x, int y) {
    if (grille[x / TAILLE_CASE][y / TAILLE_CASE] == TOUR) {
        for (int j = 0; j < game->nb_tours; j++) {
            if (game->tours[j].x == x / TAILLE_CASE && game->tours[j].y == y / TAILLE_CASE) {
                placement_gemme(game, mana, emprise, pos_gemme, milliseconde, j);
            }
        }
    }
}


/**
 * The function gemme_actions manages the action that can be done with a gem.
 * It check if the cursor of the mouse is on a gem, after if call the function actionWithGemmes to perform the corresponding action.
*/
void gemme_actions(Game* game, Reserve* mana, int* pos_gemme, int* emprise, int* pause, int milliseconde, int x, int y) {
    for (int i = 0; i < 10; i++) {
        if (x > TAILLE_CASE * (12 + i) + 5 * i && x < TAILLE_CASE * (12 + i) + 5 * i + TAILLE_CASE &&
            y > HAUTEUR * TAILLE_CASE + (TAILLE_CASE / 5) * 4 &&
            y < HAUTEUR * TAILLE_CASE + (TAILLE_CASE / 5) * 4 + TAILLE_CASE) {
            actionsWithGemmes(game, pos_gemme, 2 * i, emprise, &(mana->quantite));
        } else if (x > TAILLE_CASE * (12 + i) + 5 * i && x < TAILLE_CASE * (12 + i) + 5 * i + TAILLE_CASE &&
                   y > (HAUTEUR + 1) * TAILLE_CASE + (TAILLE_CASE / 5) * 4 + 10 &&
                   y < (HAUTEUR + 1) * TAILLE_CASE + (TAILLE_CASE / 5) * 4 + TAILLE_CASE + 10) {
            actionsWithGemmes(game, pos_gemme, 2 * i + 1, emprise, &(mana->quantite));
        }
    }
    *pause = milliseconde;
}


/**
 * The function gemme_creationmanages the creation of a new gem.
 * If the player have enough mana, it will create a new gem and will place it inside the player inventory.
*/
void gemme_creation(Game* game, Reserve* mana, int* pos_gemme, int* emprise, int* pause, int milliseconde, int x, int y) {
    if (x > TAILLE_CASE * (LARGEUR - 4) + TAILLE_CASE / 4 && x < TAILLE_CASE * (LARGEUR - 1) + TAILLE_CASE / 4 &&
        y > HAUTEUR * TAILLE_CASE + TAILLE_CASE / 2 && y < (HAUTEUR + 3) * TAILLE_CASE + TAILLE_CASE / 2) {
        if (milliseconde - *pause > 250 && game->nb_gemmes < 20 && mana->quantite >= 100 * pow(2, mana->niveau)) {
            mana->quantite -= 100 * pow(2, mana->niveau);
            game->gemmes[game->nb_gemmes] = cree_gemme();
            game->nb_gemmes++;
            *pause = milliseconde;
        }
    } else if (milliseconde - *pause > 250) {
        gemme_actions(game, mana, pos_gemme, emprise, pause, milliseconde, x, y);
    }
}



/**
 * The function clic_exterieur_terrain manages the clicks of the mouse outside the game field.
 * The function check if the player clicked on the area for updating the mana level
   or on the area for the gem creation, and perform the corresponding action.
*/
void clic_exterieur_terrain(Game* game, Reserve* mana, int* pos_gemme, int* emprise, int* pause, int milliseconde, int x, int y) {
    if (x > TAILLE_CASE * 4 && x < TAILLE_CASE * 7 && y > (HAUTEUR + 2) * TAILLE_CASE - 10 && y < (HAUTEUR + 2) * TAILLE_CASE + 10) {
        if (milliseconde - *pause > 250) {
            level_up(mana);
            *pause = milliseconde;
        }
    } else {
        gemme_creation(game, mana, pos_gemme, emprise, pause, milliseconde, x, y);
    }
}


/**
 * The function clic_sur_grill_jeu manages the clicks of the mouse on the game field.
 * The function check if the player is trying to place a tower or a gem, and perform the corresponding action.
*/
void clic_sur_grill_jeu(Game* game, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* emprise, int* pos_gemme, int* placement, int milliseconde, int x, int y) {
    if (*placement == 1) {
        placement_tour_terrain(game, mana, grille, x, y);
    } else if (*emprise == 1) {
        emprise_sur_gemme(game, mana, grille, emprise, pos_gemme, milliseconde, x, y);
    }
}


/**
 * The function gestion_clic_gauche manages the left click of the mouse during the game.
 * It check if the click was made inside or outside the game field, and call the corresponding function.
*/
void gestion_clic_gauche(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* emprise, int* pos_gemme, int* placement, int* pause, int milliseconde, int x, int y) {
    if (x < LARGEUR * TAILLE_CASE && y < HAUTEUR * TAILLE_CASE) {
        clic_sur_grill_jeu(game, mana, grille, emprise, pos_gemme, placement, milliseconde, x, y);
    } else {
        clic_exterieur_terrain(game, mana, pos_gemme, emprise, pause, milliseconde, x, y);
    }
}


/**
 * The function event_souris manages the events with the mouse when in game.
 * The function check if the left or right ckick is used and perform the corresponding action.
 * The actions include the tower placement, the unselection of a gem,
   the upgrading of the mana pool, the adding of a gem, and the managing of the right click.
*/
void event_souris(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* emprise, int* pos_gemme, int* placement, int* pause, int milliseconde, int x, int y, MLV_Mouse_button clic) {
    if (clic == MLV_BUTTON_LEFT) {
        gestion_clic_gauche(game, tabVague, mana, grille, emprise, pos_gemme, placement, pause, milliseconde, x, y);
    } else if (clic == MLV_BUTTON_RIGHT) {
        gestion_clic_droit(game, pos_gemme, placement, emprise);
    }
}


/**
 * The function gestion_actions manages the game depending on the elasped time.
 * It move the monster inside a wave and makes act all towers.
 * Every 35 seconds, the function create a new wave.
*/
void gestion_actions(Game* game, TableVague* tabVague, caseType grille[LARGEUR][HAUTEUR], Reserve* mana, int* depart, int* firstcall, int* derniere_appel_ensemble, int* appel, int emprise, int milliseconde, int *seconde, int *alert ) {
    if (*depart == 1) {
        deplacement_ensemble(tabVague, grille, mana, milliseconde);
        action_tour(game, tabVague, milliseconde);
    }
    *seconde = (milliseconde - *firstcall) / 1000;
    if (*seconde == 35 && *depart == 1) {
        ensemble_de_vague(tabVague, grille);
        *firstcall = milliseconde;
        return;
    }
    else if(*appel >= 2 && *seconde >= 0 && *depart == 1) {
        *alert += 1;
        if(*alert >= 2){
        int temps_restant = 35 - *derniere_appel_ensemble % 35;
        int pourcentage_gagne = (temps_restant * 100) / 35;
        int mana_gagne = (pourcentage_gagne * mana->maximum) / 100;
        mana->quantite += mana_gagne;
        if (mana->quantite > mana->maximum) {
            mana->quantite = mana->maximum;
        }
      }
      ensemble_de_vague(tabVague, grille);
      *firstcall = milliseconde;
      (*appel) = 0;
      return;
    }
}






/**
 * The function boucle_principale is the main loop of the game.
 * It update the display, manages the events of the mouse and the keyboard, and the game's action?
 * The function continue to turn until the game is over.
*/
void boucle_principale(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR]){
    MLV_Event event;
    MLV_Keyboard_button button;
    MLV_Mouse_button clic;
    int x, y;
    int milliseconde;
    int pause = 100;
    int placement = 0;
    int emprise = 0;
    int pos_gemme = 0;
    int firstcall = 0;
    int derniere_appel_ensemble = -1;
    int depart = 0;
    int appel = 1;
    int seconde = 0;
    int alert = 0;
    while(mana->quit != -1){
        milliseconde =  MLV_get_time();
        affiche_all(grille, tabVague, *mana, *game, x, y);
        if (placement == 1) tour(x, y);
        MLV_update_window();
        event = MLV_get_event(&button, NULL, NULL, NULL, NULL, &x, &y, &clic, NULL);
        if(event == MLV_KEY ){
          event_clavier(game, tabVague, mana, grille, &depart, &firstcall, &derniere_appel_ensemble, &appel, &emprise, &placement, milliseconde, x, y, button);
        }else if (event == MLV_MOUSE_BUTTON) {
          event_souris(game, tabVague, mana, grille, &emprise, &pos_gemme, &placement, &pause, milliseconde, x, y, clic);
        }
        gestion_actions(game, tabVague, grille, mana, &depart, &firstcall, &derniere_appel_ensemble, &appel, emprise, milliseconde, &seconde, &alert);

    }
}

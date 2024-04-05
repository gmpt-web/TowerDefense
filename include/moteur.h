#ifndef __MOTEUR__
#define __MOTEUR__
#include "graphique.h"
#include "terrain.h"
#include "vague.h"
#include "gemmes.h"
#include "mana.h"
#include "game.h"


/**
 Manage the keyboard's events during the game
*/
void event_clavier(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* depart, int* firstcall, int* derniere_appel_ensemble,
int* appel, int *emprise, int *placement, int milliseconde, int x, int y, MLV_Keyboard_button button);


/**
 Manage the mouse's events during the game
*/
void event_souris(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR], int* emprise, int* pos_gemme,
int* placement, int* pause, int milliseconde, int x, int y, MLV_Mouse_button clic);


/**
 Manage the player's actions during the game
*/
void gestion_actions(Game* game, TableVague* tabVague, caseType grille[LARGEUR][HAUTEUR], Reserve* mana, int* depart,
int* firstcall, int* derniere_appel_ensemble, int* appel, int emprise, int milliseconde, int *seconde, int *alert);


/**
 The main game's loop
*/
void boucle_principale(Game* game, TableVague* tabVague, Reserve* mana, caseType grille[LARGEUR][HAUTEUR]);

#endif

#ifndef __GRAPHIQUE__
#define __GRAPHIQUE__
#include "gemmes.h"
#include "mana.h"
#include "terrain.h"
#include "game.h"
#include "vague.h"


/**
 Function to display the mana
*/
void affiche_mana(Reserve mana);


/**
 Function to display a tower with given coordinate
*/
void tour(int x, int y);


/**
 Function to display all towers
*/
void affiche_tour(Tour *towers, int nb_tours);


/**
 Function to display all projectiles
*/
void affiche_proj(Projectile *proj, int nb_proj, int teinte);


/**
 Function to display gems
*/
void affiche_gemmes(Gem *gemmes, int nb_gemmes, int x, int y);


/**
 Function to display moving monsters
*/
void affiche_monstre(Vague vague);


/**
 Function to display all the monster's waves
*/
void affiche_ensemble(TableVague *tabVague);

/**
 Function initializing the game field
*/
void initialise_terrain();



/**
 Function displaying the road for the monsters
*/
void affiche_terrain(caseType grille[LARGEUR][HAUTEUR]);


/**
 Function that display everything (field, monsters, towers...)
*/
void affiche_all(caseType grille[LARGEUR][HAUTEUR], TableVague *tabVague, Reserve mana, Game game, int x, int y);

/**
 Function to obtain the color of a tint depending on his value
*/
MLV_Color get_color(int teinte);

#endif

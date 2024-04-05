#ifndef __TOURS__
#define __TOURS__
#include "gemmes.h"
#include "vague.h"
#include "terrain.h"

/**
Structure for the projectiles.
*/
typedef struct _proj_{
    double x;
    double y;
    int pos_lst_cible;
    int num_vague; //Temp
}Projectile;

/**
 Structure for the towers.
 */
typedef struct _tower_{
    int x;
    int y;
    Gem gemme;
    int time_installation; //Temps après le placement de la gemme.
    int last_shot; //Temps du dernier tir
    int pret;
    Projectile proj_shooted[100];
    int nb_proj; //Temp
}Tour;

/**
Structure for the game.
*/
typedef struct _game_{
    Tour tours[100];
    Gem gemmes[20];
    int nb_tours;
    int nb_gemmes;
}Game;

/**
Function that initialize a list of towers
*/
void init_LstTours(Tour *tours);

/**
Function that initialize the game || initialise la liste des gemmes et des tours
*/
Game init_game();

/**
Function that add a tower in a list of tower
*/
int AjoutTour(Game *g, int *mana, int x, int y);

/**
Function that permit action with gems
*/
void actionsWithGemmes(Game *g, int *pos_gemme, int select, int *emprise, int *mana);

/**
Function that calculate the distance between two points.
*/
double distance(double x1, double y1, double x2, double y2);

/**
Function that create a projectile
*/
void create_projectile(Tour *tour, int pos_cible, int num_vague, int milliseconde);

/**
Function that can target a monster at range
*/
void atRange(Vague *vague, Tour *tour, int milliseconde);

/**
Function that rearrange the list of projectiles.
*/
void rearrange_proj(Projectile *proj, int pos, int taille);

/**
Function that modify the position of the projectiles
*/
void deplacement_projectiles(TableVague *tabVague, Tour *tour, int milliseconde);

/**
Function that calculate the damage inflicted
*/
int degats(Gem gemme, int mt_teinte, double mult);

/**
Function that inflict damage to monster inside a certain range and give them debuff
*/
void splash_damage(TableVague *tabVague, Tour *tour, double x, double y, double range, double mult, double speed_reduce, int milliseconde);

/**
Function that give a element or create a elemental reaction
*/
void reaction_elem(TableVague *tabVague, Tour *tour, Monstre *monstre, int milliseconde);

/**
Function that call upon the action of all tower
*/
void action_tour(Game *game, TableVague *tabVague, int milliseconde);

#endif

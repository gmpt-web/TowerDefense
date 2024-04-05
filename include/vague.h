#ifndef __VAGUE__
#define __VAGUE__
#include "terrain.h"
#include "gemmes.h"
#include "mana.h"


/**
  Enumeration defining the type of wave that exist
*/
typedef enum {
    NORMALE,
    FOULE,
    AGILE,
    BOSS
} TypeVague;


/**
 Enumeration defining the possible effect that can affect the monster
*/
typedef enum {
    SLOW,
    POISON,
    VAPORISATION,
    ENRACINEMENT,
    NOEFFECT
} Effect;

/**
  * Structure representing the coordinate of a case in the grid
*/
typedef struct _case_{
    int x;
    int y;
}Case;

/**
 Structure representing the features of a monster
*/
typedef struct _monstre_{
    int hp; //point de vie du monstre
    double speed; //Vitesse du monstre
    int teinte; //couleur de l'ennemi def par un entier entre 0 et 359
    double pos_x;
    double pos_y;
    int pas;
    Card route;
    Element etat;
    Effect effet;
    int duree_effet;
    int degats_poison;
}Monstre;


/**
 Structure representing a wave of monster
*/
typedef struct
{
    TypeVague type; //type de la vague
    Monstre *monstre; //stats des monstres de la vague
    int nb_vague; //enregistre le numéro de la vague actuelle
    int nbr_monstre; //nombre d'ennemis
    int base_hp; //point de vie de base des monstres (Pour l'affichage)
    double base_speed;
}Vague;

/**
 Structure representing a table of waves
*/
typedef struct {
    Vague table[1000];
    int vagTotal;  // Nombre total de vague depuis le depuis
    double mult; // commence à 1 et augmente selon le numéro de la vague actuelle
    Case mst;  // Case de depart des monstres
    Case joueur; // Case du joueur à defendre
} TableVague;



/**
Function that permit the choice of the wave:
- 50% of the waves are normal
- 20% of the waves are crowds
- 20% of the waves are agile
- 10% of the waves are Boss (composed of two monsters)
*/
TypeVague type_vague();


Monstre *init_monster(int nb_mt, double speed, int hp, Case mst);

/**
Function initializes the fields necessary for triggering the monster wave
It returns a Wave type.
*/
Vague init_vague(int num_vag, double mult, Case mst);



/**
 Initialize the board that contain all the waves
 */
TableVague init_TabVague(caseType grille[LARGEUR][HAUTEUR]);

/**
 *Trouvre les coordonnées ayant comme champs "MONSTER" pour la considerer
 *comme le nid des monstres
*/
Case coordonnee_monstre(caseType grille[LARGEUR][HAUTEUR], int type);

/**
 * Détermine la direction initiale d'un monstre lorsqu'il sort du nid.
*/
Card depart_monstre(Monstre monstre, caseType grille[LARGEUR][HAUTEUR]);

/**
  * Changes the direction of a monster based on occupied squares around it.
*/

Card changement_direction(Monstre monstre, caseType grille[LARGEUR][HAUTEUR]);

/**
 * Moves a monster based on its current speed and direction.
 * If the monster reaches the player's position, it is returned to its starting position.
*/
void deplacement_monstre(Vague *vague, caseType grille[LARGEUR][HAUTEUR], Case joueur, Case mst, Reserve *mana);


/**
 * Rearange the list of monster
*/
void rearrange_mst(Monstre *monstres, int pos, int taille);

/**
 * Delete a monster inside the list of monster of the wave and gain mana
*/
void supprime_monstre(Vague *vague, Reserve *mana);

/**
 * Reset the speed of a monster if the conditions are met
*/
void speed_state(Monstre *monstre, int milliseconde, double base_speed, int duree);

/**
 * Activate the effects on the monster
*/
void effets(Vague *vague, int milliseconde);

/**
 * Move all monsters in all waves
*/
void deplacement_ensemble(TableVague *tabVague, caseType grille[LARGEUR][HAUTEUR], Reserve *mana, int milliseconde);

/**
 * Manage the set of waves and also the creation of new waves and the monsters movement
*/
void ensemble_de_vague(TableVague *tabVague, caseType grille[LARGEUR][HAUTEUR]);

#endif

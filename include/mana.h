#ifndef __MANA__
#define __MANA__


/**
 Structure representing a reserve of mana
*/
typedef struct _mana_{
    int maximum; //Maximum de mana obtenable
    int quantite; //Quantité de mana actuelle
    int niveau; //Niveau actuelle de la réserve de mana
    int quit;
}Reserve;


/**
 Initialize a new mana reserve
*/
Reserve init_mana();

/**
 Function that level up the mana reserve
*/
void level_up(Reserve *mana);

/**
 Increase the mana quantity of the player depending on the health of the killed monster
*/
void recolte(Reserve *mana, int mort);

/**
 Reduce the mana quantity of the player to ban a monster
*/
int bannissement(Reserve *mana, int banni);

#endif

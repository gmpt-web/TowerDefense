#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mana.h"


/**
 * Initialize a new pool of mana for the player with default value
*/
Reserve init_mana(){
    Reserve new;
    new.maximum = 2000;
    new.quantite = 150;
    new.niveau = 0;
    new.quit = 0;
    return new;
}


/**
 * This function increase the level of the mana reserve if the mana
   quantity is higher than a quarter of the max quantity
*/
void level_up(Reserve *mana){
    if(mana->quantite > mana->maximum * 0.25){
        mana->quantite -= mana->maximum * 0.25;
        mana->maximum *= 1.4;
        mana->niveau += 1;
    }
}


/**
 * Add mana to the reserve depending on the maximum health of the dead monster
 * If the quantity is higher than the maximum, we give the maximum value
*/
void recolte(Reserve *mana, int mort){
    mana->quantite += (mort * 0.1) * pow(1.3, mana->niveau);
    if(mana->quantite > mana->maximum) mana->quantite = mana->maximum;
}


/**
 * Use mana to ban a monster who enter the player's camp
 * Return 0 if the player succeed in banishing the monster or -1
*/
int bannissement(Reserve *mana, int banni){
    if(mana->quantite > (banni * 0.15) * pow(1.3, mana->niveau)){
        mana->quantite -= (banni * 0.15) * pow(1.3, mana->niveau);
        return 0;
    }
    return -1;
}

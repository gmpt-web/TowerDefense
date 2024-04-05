#include <stdio.h>
#include <stdlib.h>
#include "gemmes.h"



/**
 *  This function return the element of a gem according to his color
 */
Element get_elem(int teinte){
    if(teinte >= 330 || teinte <= 30) return PYRO;
    if(teinte >= 210 && teinte <= 270) return HYDRO;
    if(teinte >= 90 && teinte <= 150) return DENDRO;
    return NONE;
}


/**
 * This function initialize a gem whit random values for his element and his color
 */
Gem cree_gemme(){
    Gem new;
    new.type = PURE;
    new.elem = rand() % 3;
    if(new.elem == 0) new.teinte = (330 + rand() % 60) % 360;
    if(new.elem == 1) new.teinte = 210 + rand() % 60;
    if(new.elem == 2) new.teinte = 90 + rand() % 60;
    new.niveau = 0;
    new.emprise = 0;
    return new;
}


/**
 * Rearrange gems after merging
 */
Gem *rearrange_gemmes(Gem gemmes[20], int pos, int taille){
    for(int i = pos; i < taille; i++){
        gemmes[i] = gemmes[i + 1];
    }
    return gemmes;
}


/**
 * This function merge two gems if they are of the same levels and create a new gem with one more level
 */
int fusion(Gem *gemme1, Gem gemme2){
    if(gemme1->niveau != gemme2.niveau)return 1;
    Gem new;
    if(gemme1->elem == PYRO && gemme1->teinte <= 30 && gemme2.teinte > 180) gemme1->teinte += 360;
    if(gemme2.elem == PYRO && gemme2.teinte <= 30 && gemme1->teinte > 180) gemme2.teinte += 360;
    if(gemme1->elem == PYRO && gemme1->teinte >= 330 && gemme2.teinte <= 180) gemme1->teinte = 360 - gemme1->teinte;
    if(gemme2.elem == PYRO && gemme2.teinte >= 330 && gemme1->teinte <= 180) gemme2.teinte = 360 - gemme2.teinte;
    new.teinte = (gemme1->teinte + gemme2.teinte)/2 % 360;
    new.emprise = 0;
    new.niveau = gemme1->niveau + 1;  // Augmentation de niveau de la nouvelle gemme
    new.elem = get_elem(new.teinte);
    if(gemme1->type == PURE && gemme2.type == PURE && gemme1->elem == gemme2.elem) new.type = PURE;
    else new.type = MIXTE;
    *gemme1 = new;
    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "vague.h"



/**
 * Return a random type of wave.
 * The possible types are NORMALE, FOULE, AGILE and BOSS.
*/
TypeVague type_vague(){
    int type = rand() % 100;
    if(type < 50) return NORMALE;
    else if(type < 70) return FOULE;
    else if(type < 90) return AGILE;
    else return BOSS;
}


/**
 * Initialize the data of the monsters in a wave/
 * Every monster is initialized a certain quantity of health points (hp),
   speed (speed), and starting position (mst).
 * This Function return a pointer to a list of monsters.
*/
Monstre *init_monster(int nb_monstre, double speed, int hp, Case mst){
    Monstre *monstre = (Monstre*)malloc(sizeof(Monstre) * nb_monstre);
    if(monstre == NULL){
        fprintf(stderr, "Erreur d'allocation. \n");
        return NULL;
    }
    for(int i = 0; i < nb_monstre; i++){
        monstre[i].hp = hp;
        monstre[i].speed = speed;
        monstre[i].teinte = rand() % 360;
        monstre[i].pos_x = (double)(mst.x) * TAILLE_CASE + TAILLE_CASE/2;
        monstre[i].pos_y = (double)(mst.y) * TAILLE_CASE + TAILLE_CASE/2;
        monstre[i].pas = 0;
        monstre[i].etat = NONE;
        monstre[i].effet = NOEFFECT;
    }
    return monstre;
}


/**
 * Create a wave that initialize all data.
 * The number of monster, their basic speed and basic health are determined by the wave type.
 * The monsters are then initialized with those values.
*/
Vague init_vague(int num_vague, double mult, Case mst){
    Vague vague;
    vague.nb_vague = num_vague + 1;
    vague.type = type_vague();
    int nb_mt = 12, hp = mult * 10;
    double speed = 1.8;
    if(vague.type == FOULE) nb_mt *= 2;
    else if(vague.type == AGILE) speed *= 2;
    else if(vague.type == BOSS) {
        nb_mt /= 6;
        hp *= 12;
    }
    vague.base_speed = speed;
    vague.nbr_monstre = nb_mt;
    vague.base_hp = hp;
    vague.monstre = init_monster(nb_mt, speed, hp, mst);
    return vague;
}


/**
 * Initialze the list of waves.
 * The list of waves contains informations on all waves like
   the numbers of wave, the difficulty multiplier
   and the position of the player's case and the monster's case
*/
TableVague init_TabVague(caseType grille[LARGEUR][HAUTEUR]){
    TableVague new;
    new.vagTotal = 0;
    new.mult = 1;
    new.mst = coordonnee_monstre(grille, MONSTER);
    new.joueur = coordonnee_monstre(grille, JOUEUR);
    return new;
}


/**
 * Browse the grid and find the coordinate of the specifed type.
 * This function is used to find the positons of the monster's nest and the player's square.
*/
Case coordonnee_monstre(caseType grille[LARGEUR][HAUTEUR], int type){
    Case mst;
    int i, j;
    for(i =0; i < LARGEUR; ++i){
        for(j = 0; j < HAUTEUR; ++j){
            if(grille[i][j] == type){
                mst.x = i, mst.y = j;
            }
        }
    }
    return mst;
}





/**
 *  Give the direction taken by the monster when they get out of the nest.
 */
Card depart_monstre(Monstre monstre, caseType grille[LARGEUR][HAUTEUR]){
    int x = (int)monstre.pos_x;
    int y = (int)monstre.pos_y;
    if(grille[x/TAILLE_CASE][(y/TAILLE_CASE) - 1] == BUSY) return NORD;
    if(grille[x/TAILLE_CASE][(y/TAILLE_CASE) + 1] == BUSY) return SUD;
    if(grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == BUSY) return EST;
    if(grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == BUSY) return OUEST;
    return -1;
}


/**
 * Check if there is a turn and modify the direction of the monster consequently.
*/
Card changement_direction(Monstre monstre, caseType grille[LARGEUR][HAUTEUR]){
    Card new;
    int x = (int)monstre.pos_x;
    int y = (int)monstre.pos_y;
    switch (monstre.route){
        case NORD:
            if(grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == BUSY
                || grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == JOUEUR) new = NORD;
            else{
                if(grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == JOUEUR  ) new = EST;
                if(grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == JOUEUR) new = OUEST;
            }
            break;

        case SUD:
            if(grille[x/TAILLE_CASE][(y/TAILLE_CASE) + 1] == BUSY
                || grille[(x/TAILLE_CASE)][y/TAILLE_CASE + 1] == JOUEUR) new = SUD;
            else{
                if(grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == JOUEUR) new = EST;
                if(grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == JOUEUR) new = OUEST;
            }
            break;

        case EST:
            if(grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) + 1][y/TAILLE_CASE] == JOUEUR) new = EST;
            else{
                if(grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == BUSY
                || grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == JOUEUR) new = NORD;
                if(grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) + 1] == BUSY
                || grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) + 1] == JOUEUR) new = SUD;
            }
            break;

        case OUEST:
            if(grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == BUSY
                || grille[(x/TAILLE_CASE) - 1][y/TAILLE_CASE] == JOUEUR) new = OUEST;
            else{
                if(grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == BUSY
                || grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) - 1] == JOUEUR) new = NORD;
                if(grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) + 1] == BUSY
                || grille[(x/TAILLE_CASE)][(y/TAILLE_CASE) + 1] == JOUEUR) new = SUD;
            }
            break;

        default:
            break;
    }
    return new;
}


/**
 * Modify the position of the monster depending on the direction of the road and his speed.
 * If the monster reach the player's square, his is sent back to the starting point and the player's mana is reduced.
 * Or else, the monster move in the actual direction at his actual speed.
 * If a monster is in the middle of the square, his direction is updated depending on the occupied square arround him.
 */
void deplacer_monstre(Monstre *monstre) {
    switch (monstre->route) {
        case NORD:
            monstre->pos_y -= monstre->speed/60;
            break;
        case SUD:
            monstre->pos_y += monstre->speed/60;
            break;
        case EST:
            monstre->pos_x += monstre->speed/60;
            break;
        case OUEST:
            monstre->pos_x -= monstre->speed/60;
            break;
        default:
            break;
    }
    monstre->pas++;
}

/**
 * This function look a the position of the given monster.
 * If he is a the center of a square, we call the function changement_dierection
   to change he's direction depending on the following path.
 * If the monster is a the end of the road, if the player have enough mana,
   the monster is moved to the begining of the road, or else, the game is stopped.
*/
void verifier_position_monstre(Monstre *monstre, caseType grille[LARGEUR][HAUTEUR], Case joueur, Case mst, Reserve *mana) {
    int case_x = (int)(monstre->pos_x / TAILLE_CASE);
    int case_y = (int)(monstre->pos_y / TAILLE_CASE);
    double mid_x = (case_x * TAILLE_CASE) + TAILLE_CASE / 2.0;
    double mid_y = (case_y * TAILLE_CASE) + TAILLE_CASE / 2.0;
    double epsilon = 1.0;
    if (fabs(monstre->pos_x - mid_x) < epsilon && fabs(monstre->pos_y - mid_y) < epsilon) {
        if(case_x == joueur.x &&  case_y == joueur.y){
            monstre->pos_x = (double)(mst.x) * TAILLE_CASE + TAILLE_CASE/2;
            monstre->pos_y = (double)(mst.y) * TAILLE_CASE + TAILLE_CASE/2;
            monstre->route = depart_monstre(*monstre, grille);
            if(bannissement(mana, monstre->hp) == -1) mana->quit = -1;
        }else{
          monstre->route = changement_direction(*monstre, grille);
        }
    }
}

/**
 * The function deplacement_monstre move all monster inside a wave.
*/
void deplacement_monstre(Vague *vague, caseType grille[LARGEUR][HAUTEUR], Case joueur, Case mst, Reserve *mana) {
    for (int i = 0; i < vague->nbr_monstre; i++) {
        deplacer_monstre(&vague->monstre[i]);
        verifier_position_monstre(&vague->monstre[i], grille, joueur, mst, mana);
        if (vague->monstre[i].pas < TAILLE_MONSTRE_RAYON * 120) return;
    }
}



/**
 * Rearrange the list of monsters by moving the monsters after
 * the specified position to the left.
 * This function is used for deleting a monster inside the list whithout leaving a hole.
*/
void rearrange_mst(Monstre *monstres, int pos, int taille){
    for(int i = pos; i < taille; i++){
        monstres[i] = monstres[i + 1];
    }
}


/**
 * Delete all monsters who have less than 0 health left inside the list of monsters.
 * For every monster killed, le player gain mana depending on the base health of the monster.
*/
void supprime_monstre(Vague *vague, Reserve *mana){
    int i;
    for(i = 0; i < vague->nbr_monstre; i++){
        if(vague->monstre[i].hp < 0){
            recolte(mana, vague->base_hp);
            rearrange_mst(vague->monstre, i, vague->nbr_monstre--);
        }

    }
}


/**
 * Update the speed of the monster depending on the time elasped since the bigining of the effect.
 * If the effect has expired, the monster's speed is reset to his initial speed and the effect is deleted .
*/
void speed_state(Monstre *monstre, int milliseconde, double base_speed, int duree){
    if(milliseconde - monstre->duree_effet >= duree){
        monstre->speed = base_speed;
        monstre->effet = NOEFFECT;
    }
}


/**
 * Apply the active effect on every monster in the wave.
 * The possible effects are vaporization, rooting, slow-down, and poison.
 * Every effect has a different duration and different effect on a monster.
*/
void effets(Vague *vague, int milliseconde){
    int i;
    for(i = 0; i < vague->nbr_monstre; i++){
        switch (vague->monstre[i].effet){
            case VAPORISATION:
                speed_state(&(vague->monstre[i]), milliseconde, vague->base_speed, 5000);
                break;
            case ENRACINEMENT:
                speed_state(&(vague->monstre[i]), milliseconde, vague->base_speed, 3000);
                break;
            case SLOW:
                speed_state(&(vague->monstre[i]), milliseconde, vague->base_speed, 10000);
                break;
            case POISON:
                if(milliseconde - vague->monstre[i].duree_effet >= 10000){
                    vague->monstre[i].effet = NOEFFECT;
                } else if((milliseconde - vague->monstre[i].duree_effet) % 500 == 0) vague->monstre[i].hp -= vague->monstre[i].degats_poison;
        default:
            break;
        }
    }
}

/**
 * Move all monsters inside all waves.
 * Also, activate all active effects and delete dead monster.
*/
void deplacement_ensemble(TableVague *tabVague, caseType grille[LARGEUR][HAUTEUR], Reserve *mana, int milliseconde) {
    for (int i = 0; i < tabVague->vagTotal; i++) {
        deplacement_monstre(&tabVague->table[i], grille, tabVague->joueur, tabVague->mst, mana);
        effets(&tabVague->table[i], milliseconde);
        supprime_monstre(&tabVague->table[i], mana);
    }
}




/**
 * Manage a wave set.
 * For every wave, the function initialize the wave and check if the type of wave is "BOSS" et if it's the one of the first five wave.
 * If it's the case, the function initialize the wave again until the type of wave isn't "BOSS".
 * After that, for every monster inside a wave, it determine the initial direction of the monster.
 * Finally, it increase the total number of wave by one et increase the dificulty multiplier.
*/
void ensemble_de_vague(TableVague *tabVague, caseType grille[LARGEUR][HAUTEUR]) {
    tabVague->table[tabVague->vagTotal] = init_vague(tabVague->vagTotal, tabVague->mult, tabVague->mst);
    while(tabVague->vagTotal < 6 && tabVague->table[tabVague->vagTotal].type == BOSS){
        fprintf(stderr, "######################## Boss venu dans les 5 premieres vagues #####################################\n");
        tabVague->table[tabVague->vagTotal] = init_vague(tabVague->vagTotal, tabVague->mult, tabVague->mst);
    }
    for (int i = 0; i < tabVague->table[tabVague->vagTotal].nbr_monstre; i++) {
      tabVague->table[tabVague->vagTotal].monstre[i].route = depart_monstre(tabVague->table[tabVague->vagTotal].monstre[i], grille);

    }
    tabVague->vagTotal += 1;
    tabVague->mult *= 1.2;
}

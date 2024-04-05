#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"

/**
 *  This function initialize a list of the structure "Tour"
 */
void init_LstTours(Tour *tours){
    for(int i = 0; i < 100; i++){
        tours[i].pret = 0;
        tours[i].last_shot = 0;
        tours[i].time_installation = 0;
        tours[i].nb_proj = 0;
    }
}

/**
 * This function initialize the structure "Game"
 */
Game init_game(){
    Game new;
    init_LstTours(new.tours);
    new.nb_tours = 0;
    new.nb_gemmes = 0;
    return new;
}

/**
 * This function add a tower to the list of tower inside the Game if the player have enough mana
 */
int AjoutTour(Game *g, int *mana, int x, int y){
    if(g->nb_tours >= 3){
        if((*mana) <= 100 * pow(2, g->nb_tours - 3)) return 0;
        (*mana) -= 100 * pow(2, g->nb_tours - 3);
    }
    g->tours[g->nb_tours].x = x;
    g->tours[g->nb_tours].y = y;
    g->nb_tours++;
    return 1;
}

/**
 * This function see if a gem is selected and if selected it deselect the gem or merge it with another gem
 */
void actionsWithGemmes(Game *g, int *pos_gemme, int select, int *emprise, int *mana){
    if((*emprise) == 0){
        g->gemmes[select].emprise = 1;
        (*pos_gemme) = select;
        (*emprise) = 1;
    }
    else{
        if(g->gemmes[select].emprise == 1){
            g->gemmes[select].emprise = 0;
            (*emprise) = 0;
        }else{
            if((*mana) >= 100){
                if(fusion(&(g->gemmes[select]), g->gemmes[(*pos_gemme)]) == 1) return;
                (*mana) -= 100;
                rearrange_gemmes(g->gemmes, (*pos_gemme), --g->nb_gemmes);
            }else g->gemmes[(*pos_gemme)].emprise = 0;
            (*emprise) = 0;
        }
    }

}

/**
 * This function calculate the distance between the position of a projectile and the monster it target
 */
double distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

/**
 * This function create a projectile at the position of the tower that shoot it and target
   a monster in a wave in a certain position int the list of monster.
 */
void create_projectile(Tour *tour, int pos_cible, int num_vague, int milliseconde){
    Projectile new;
    new.x = tour->x * TAILLE_CASE + TAILLE_CASE/2;
    new.y = tour->y * TAILLE_CASE + TAILLE_CASE/2;
    new.pos_lst_cible = pos_cible;
    new.num_vague = num_vague;
    tour->proj_shooted[tour->nb_proj++] = new;
    tour->last_shot = milliseconde;
}

/**
 * This function look if there are monster inside the range
   of the tower and target the monster with the higest health
 */
void atRange(Vague *vague, Tour *tour, int milliseconde){
    int i;
    int pos_cible = -1;
    for(i = 0; i < vague->nbr_monstre; i++){
        if(distance(vague->monstre[i].pos_x, vague->monstre[i].pos_y, tour->x * TAILLE_CASE, tour->y * TAILLE_CASE) <= 3 * TAILLE_CASE){
            if((vague->monstre[i].hp > vague->monstre[pos_cible].hp || pos_cible == -1) && vague->monstre[i].pas > 0) {
                pos_cible = i;
            }
        }
    }
    if(pos_cible != -1 && milliseconde - tour->last_shot > 500) create_projectile(tour, pos_cible, vague->nb_vague, milliseconde);
}

/**
 * This function rearrange the list of projectiles after a projectile reach his target
 */
void rearrange_proj(Projectile *proj, int pos, int taille){
    for(int i = pos; i < taille; i++){
        proj[i] = proj[i + 1];
    }
}

/**
 * Function that move a projectile if his distance with the monster targeted
   isn't near enough or delete it and inflict damage to the monster targeted
 */
void deplacement_projectiles(TableVague *tabVague, Tour *tour, int milliseconde){
    int i,j;
    double dist_parcourue = (3*TAILLE_CASE)/60;
    for(i = 0; i < tour->nb_proj; i++){
        j = tour->proj_shooted[i].pos_lst_cible;
        double dist_MT = distance(tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].pos_x,
            tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].pos_y,
            tour->proj_shooted[i].x, tour->proj_shooted[i].y);
        if(dist_MT <= dist_parcourue){
            tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].hp -= degats(tour->gemme, tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].teinte, 1);
            if(tour->gemme.type == PURE){
                reaction_elem(tabVague, tour, &(tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j]), milliseconde);
            }
            rearrange_proj(tour->proj_shooted, i, tour->nb_proj);
            tour->nb_proj--;
        }else {
            tour->proj_shooted[i].x += (((tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].pos_x - tour->proj_shooted[i].x)/dist_MT) * dist_parcourue);
            tour->proj_shooted[i].y += (((tabVague->table[tour->proj_shooted[i].num_vague - 1].monstre[j].pos_y - tour->proj_shooted[i].y)/dist_MT) * dist_parcourue);
        }
    }
}

/**
 * Function that calculate the damage of a projectile according to
   the color of the monster and the tower who shooted the projectile.
 */
int degats(Gem gemme, int mt_teinte, double mult){
    int degats = 10 * pow(2, gemme.niveau) * (1.0 - cos(gemme.teinte - mt_teinte)) * mult;
    if(gemme.type == MIXTE && mult >= 1){
        degats *= 2;
        if(rand() % 10 == 9) degats *= 2;
    }
    return degats;
}

/**
 * Function that inflict damage to monster if there are inside the range of the area affected by the splash damage
 * slow them if the damage are from the effect "VAPORISATION".
 */
void splash_damage(TableVague *tabVague, Tour *tour, double x, double y, double range, double mult, double speed_reduce, int milliseconde){
    int i, j;
    for (i = 0; i < tabVague->vagTotal; i++){
        for(j = 0; j < tabVague->table[i].nbr_monstre; j++){
            double dist = distance(tabVague->table[i].monstre[j].pos_x, tabVague->table[i].monstre[j].pos_y, x, y);
            if(dist <= range) {
                tabVague->table[i].monstre[j].hp -= degats(tour->gemme, tabVague->table[i].monstre[j].teinte, mult);
                tabVague->table[i].monstre[j].speed /= speed_reduce;
                if(speed_reduce != 1) {
                    tabVague->table[i].monstre[j].duree_effet = milliseconde;
                    tabVague->table[i].monstre[j].effet = VAPORISATION;
                }
            }
        }
    }
}


/**
 * Function that activate the effect of the element of a pure tower to give that
   element to the monster if he don't have one or create a reaction he have one.
 */
void reaction_elem(TableVague *tabVague, Tour *tour, Monstre *monstre, int milliseconde){
    if(monstre->effet == VAPORISATION || monstre->effet == ENRACINEMENT) return;
    if(monstre->etat == NONE){
        monstre->etat = tour->gemme.elem;
        if(tour->gemme.elem == PYRO) {
            splash_damage(tabVague, tour, monstre->pos_x,
                            monstre->pos_y, TAILLE_CASE*2, 0.15, 1, milliseconde);
        } else {
            monstre->duree_effet = milliseconde;
            if(tour->gemme.elem == HYDRO){
                monstre->speed /= 1.5;
                monstre->effet = SLOW;
            } else{
                monstre->degats_poison = degats(tour->gemme, monstre->teinte, 0.025);
                monstre->effet = POISON;
            }
        }
    }
    else if(tour->gemme.elem == monstre->etat){
        if(monstre->etat != PYRO) monstre->duree_effet = milliseconde;
    }
    else {
        if(tour->gemme.elem == PYRO){
            if(monstre->etat == HYDRO) splash_damage(tabVague, tour, monstre->pos_x, monstre->pos_y, TAILLE_CASE*3.5, 0.05, 1.25, milliseconde);
            else monstre->hp -= degats(tour->gemme, monstre->teinte, 3);
        } else if(tour->gemme.elem == HYDRO){
            monstre->duree_effet = milliseconde;
            if(monstre->etat == PYRO) splash_damage(tabVague, tour, monstre->pos_x, monstre->pos_y, TAILLE_CASE*3.5, 0.05, 1.25, milliseconde);
            else {
                monstre->hp -= monstre->speed = 0;
                monstre->effet = ENRACINEMENT;
            }
        } else{
            if(monstre->etat == PYRO) monstre->hp -= degats(tour->gemme, monstre->teinte, 3);
            else {
                monstre->duree_effet = milliseconde;
                monstre->hp -= monstre->speed = 0;
                monstre->effet = ENRACINEMENT;
            }
        }
        monstre->etat = NONE;
    }
}

/**
 * Function that call for the action of every tower
 */
void action_tour(Game *game, TableVague *tabVague, int milliseconde){
    int i, j, k;
    for (i = 0; i < tabVague->vagTotal; i++){
        for(j = 0; j < game->nb_tours; j++){
            if(game->tours[j].pret == 1)atRange(&tabVague->table[i], &game->tours[j], milliseconde);
        }
    }
    for(k = 0; k < game->nb_tours; k++){
        deplacement_projectiles(tabVague, &game->tours[k], milliseconde);
    }
}

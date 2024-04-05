#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "graphique.h"



/**
 * Display the level and the quantity of mana that the player have.
*/
void affiche_mana(Reserve mana){
    MLV_draw_text(30,  (HAUTEUR + 2) * TAILLE_CASE - 20, "Niveau %d", MLV_COLOR_BLUE3, mana.niveau);
    MLV_draw_text(30,  (HAUTEUR + 3) * TAILLE_CASE - 20, "%d/%d", MLV_COLOR_BLUE3, mana.quantite, mana.maximum);
    MLV_draw_text_box(TAILLE_CASE * 4, (HAUTEUR + 2) * TAILLE_CASE - 10, TAILLE_CASE * 3, 20, "Uppgrade", 0,
                      MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_LIGHT_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


/**
 * Display a gemme at a given position.
*/
void affiche_gemme(int teinte, int niveau, int x, int y){
    MLV_draw_filled_circle(x, y, 10, get_color(teinte));
    MLV_draw_text_box(x - TAILLE_CASE/2, y - TAILLE_CASE/2, TAILLE_CASE, TAILLE_CASE, "%d", 0, MLV_rgba(0,0,0,0),
     MLV_COLOR_BLACK, MLV_rgba(0,0,0,0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, niveau);
}


/**
 * This function Display all the gems of the player.
 * It draw a rectangle for every gems and call the function affiche_gemme to draw the gem.
 * If a gem is moving (emprise == 1), she is drawn at the mouse's position.
 * else, she's drawn at her position in the grid.
 * @param gemmes: The table of gems of the player.
 * @param nb_gemmes: The number of gems inside the table.
 * @param x: The coordinate x of the position of the mouse.
 * @param y: The coordinate y of the position of the mouse.
*/
void affiche_gemmes(Gem *gemmes, int nb_gemmes, int x, int y){
    for(int i = 0; i < 10; i++){
        int pos_case_x = TAILLE_CASE * (12 + i) + 5 * i;
        int pos_case_y1 = HAUTEUR * TAILLE_CASE + (TAILLE_CASE/5) * 4;
        int pos_case_y2 = (HAUTEUR + 1) * TAILLE_CASE + (TAILLE_CASE/5) * 4 + 10;
        MLV_draw_filled_rectangle(pos_case_x, pos_case_y1, TAILLE_CASE, TAILLE_CASE, MLV_COLOR_GRAY55);
        if(2*i < nb_gemmes) {
            if(gemmes[2*i].emprise == 1) affiche_gemme(gemmes[2 * i].teinte, gemmes[2 * i].niveau, x, y);
            else affiche_gemme(gemmes[2 * i].teinte, gemmes[2 * i].niveau, pos_case_x + TAILLE_CASE/2, pos_case_y1 + TAILLE_CASE/2);
        }
        MLV_draw_filled_rectangle(pos_case_x, pos_case_y2, TAILLE_CASE, TAILLE_CASE, MLV_COLOR_GRAY55);
        if(2*i + 1 < nb_gemmes) {
            if(gemmes[2*i + 1].emprise == 1) affiche_gemme(gemmes[2 * i + 1].teinte, gemmes[2 * i + 1].niveau, x, y);
            else affiche_gemme(gemmes[2 * i + 1].teinte, gemmes[2 * i + 1].niveau, pos_case_x + TAILLE_CASE/2, pos_case_y2 + TAILLE_CASE/2);
        }
        MLV_draw_rectangle(pos_case_x, pos_case_y1, TAILLE_CASE, TAILLE_CASE, MLV_COLOR_BLACK);
        MLV_draw_rectangle(pos_case_x, pos_case_y2, TAILLE_CASE, TAILLE_CASE, MLV_COLOR_BLACK);
    }
    MLV_draw_text_box(TAILLE_CASE * (LARGEUR - 4) + TAILLE_CASE/4, HAUTEUR * TAILLE_CASE + TAILLE_CASE/2, TAILLE_CASE * 3, TAILLE_CASE * 3, "Create", 0,
                      MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_LIGHT_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


/**
 * Draw a tower at the given position.
*/
void tour(int x, int y){
    MLV_draw_filled_circle(x, y, 12, MLV_COLOR_WHITE);
    MLV_draw_circle(x, y, 12, MLV_COLOR_BLACK);
}


/**
 * Display all the towers of the player and their projectiles.
*/
void affiche_tour(Tour *towers, int nb_tours){
    for(int i = 0; i < nb_tours; i++){
        MLV_draw_filled_circle(towers[i].x * TAILLE_CASE + TAILLE_CASE/2, towers[i].y * TAILLE_CASE + TAILLE_CASE/2, 12, MLV_COLOR_WHITE);
        MLV_draw_circle(towers[i].x * TAILLE_CASE + TAILLE_CASE/2, towers[i].y * TAILLE_CASE + TAILLE_CASE/2, 12, MLV_COLOR_BLACK);
        if(towers[i].pret == 1){
            // Affiche la gemme posée sur un tour
            affiche_gemme(towers[i].gemme.teinte, towers[i].gemme.niveau,
            towers[i].x * TAILLE_CASE + TAILLE_CASE/2, towers[i].y * TAILLE_CASE + TAILLE_CASE/2);
            MLV_draw_circle(towers[i].x * TAILLE_CASE + TAILLE_CASE/2, towers[i].y * TAILLE_CASE + TAILLE_CASE/2, 3*TAILLE_CASE, MLV_COLOR_BLACK);
        }
        // Affiche les projectiles lancés par un tour
        affiche_proj(towers[i].proj_shooted, towers[i].nb_proj, towers[i].gemme.teinte);
    }
}


/**
 * Display all the projectiles shooted by a tower.
*/
void affiche_proj(Projectile *proj, int nb_proj, int teinte){
    for(int i = 0; i < nb_proj; i++){
        MLV_draw_filled_circle(proj[i].x , proj[i].y, 2, get_color(teinte));
        MLV_draw_circle(proj[i].x, proj[i].y, 2, MLV_COLOR_BLACK);
    }
}


/**
 * The function affiche_monstre draw all the monsters of a wave on the grid.
 * For every monster, It draw a circle corresponding to his position and a health bar above him.
 * The color of the circle depend on the tint of the monster.
 * The length of the health bar corresponds to the actual health of the monster.
 * If the monster have a state (PYRO, DENDRO, HYDRO), a corresponding rectangle is drawn above the health bar.
 * @param vague: The wave of monster to display.
*/
void affiche_monstre(Vague vague){
    for(int i = 0; i < vague.nbr_monstre; i++){
        if(vague.monstre[i].pas != 0){
            MLV_draw_filled_circle(vague.monstre[i].pos_x, vague.monstre[i].pos_y, TAILLE_MONSTRE_RAYON, get_color(vague.monstre[i].teinte));
            MLV_draw_circle(vague.monstre[i].pos_x, vague.monstre[i].pos_y, TAILLE_MONSTRE_RAYON, MLV_COLOR_BLACK);
            MLV_draw_filled_rectangle(vague.monstre[i].pos_x - (TAILLE_CASE/5)*2, vague.monstre[i].pos_y - (TAILLE_CASE/5)*2,
                                    (((TAILLE_CASE/5)*4) * vague.monstre[i].hp)/vague.base_hp, TAILLE_CASE/5, MLV_COLOR_GREEN);
            if(vague.base_hp - vague.monstre[i].hp > 0)
                MLV_draw_filled_rectangle(vague.monstre[i].pos_x - (TAILLE_CASE/5)*2 + (((TAILLE_CASE/5)*4) * vague.monstre[i].hp)/vague.base_hp, vague.monstre[i].pos_y - (TAILLE_CASE/5)*2,
                                        (((TAILLE_CASE/5)*4) * (vague.base_hp - vague.monstre[i].hp))/vague.base_hp , TAILLE_CASE/5, MLV_COLOR_RED);
            MLV_draw_filled_rectangle(vague.monstre[i].pos_x - (TAILLE_CASE/5)*0.5, vague.monstre[i].pos_y - (TAILLE_CASE/5)*4, 5, 10,
                                    (vague.monstre[i].etat == PYRO) ? MLV_COLOR_RED : (vague.monstre[i].etat == DENDRO) ? MLV_COLOR_GREEN : (vague.monstre[i].etat == HYDRO) ? MLV_COLOR_BLUE : MLV_rgba(0,0,0,0));
        }
        else break;
    }
}



/**
 * Display all the monster's wave.
*/
void affiche_ensemble(TableVague *tabVague) {
    for (int i = 0; i < tabVague->vagTotal; i++) affiche_monstre(tabVague->table[i]);

    MLV_draw_text(TAILLE_CASE * 8,  (HAUTEUR + 2) * TAILLE_CASE - 10, "Vague %d", MLV_COLOR_BLACK, tabVague->vagTotal);
}


/**
 * Initialize the game field by drawing the lines of the grid.
*/
void initialise_terrain(){
    int i, j;
    for(i = 0; i < LARGEUR; ++i){
        for(j = 0; j < HAUTEUR; ++j){
            MLV_draw_line(i * TAILLE_CASE, j, i * TAILLE_CASE,
            (j + 1) * TAILLE_CASE, MLV_COLOR_BLACK);
            MLV_draw_line(i * TAILLE_CASE, j * TAILLE_CASE, (i + 1) * TAILLE_CASE,
            j * TAILLE_CASE, MLV_COLOR_BLACK);
        }
    }
    MLV_draw_filled_rectangle(0, HAUTEUR * TAILLE_CASE, LARGEUR * TAILLE_CASE, 4 * TAILLE_CASE, MLV_COLOR_LIGHT_GREY);
    MLV_draw_filled_rectangle(0, HAUTEUR * TAILLE_CASE, 10, 4 * TAILLE_CASE, MLV_COLOR_GRAY55);
    MLV_draw_filled_rectangle(0, HAUTEUR * TAILLE_CASE, LARGEUR * TAILLE_CASE, 10, MLV_COLOR_GRAY55);
    MLV_draw_filled_rectangle(LARGEUR * TAILLE_CASE - 10, HAUTEUR * TAILLE_CASE, 10, 4 * TAILLE_CASE, MLV_COLOR_GRAY55);
    MLV_draw_filled_rectangle(0, (HAUTEUR + 2) * TAILLE_CASE + 40, LARGEUR * TAILLE_CASE, 10, MLV_COLOR_GRAY55);
}


/**
 * Display a square a the center of a given zone.
*/
void afficher_carre_au_centre(int x, int y, int color) {
    int centre_x = x * TAILLE_CASE + TAILLE_CASE / 2;
    int centre_y = y * TAILLE_CASE + TAILLE_CASE / 2;
    int demi_cote = TAILLE_CASE / 2;
    MLV_draw_filled_rectangle(centre_x - demi_cote, centre_y - demi_cote,
    TAILLE_CASE, TAILLE_CASE, color);
}


/**
 * Display the field by coloring every box depending on his type.
*/
void affiche_terrain(caseType grille[LARGEUR][HAUTEUR]){
    for(int i = 0; i < LARGEUR; i++){
        for(int j = 0; j < HAUTEUR; j++){
            switch(grille[i][j]){
                case MONSTER:
                    afficher_carre_au_centre(i, j, MLV_COLOR_RED);
                    break;
                case BUSY:
                    afficher_carre_au_centre(i, j, MLV_COLOR_WHITE);
                    break;
                case JOUEUR:
                    afficher_carre_au_centre(i, j, MLV_COLOR_GREEN);
                    break;
                case TOUR:
                    afficher_carre_au_centre(i, j, MLV_COLOR_GRAY);
                    break;
                default:
                    afficher_carre_au_centre(i, j, MLV_COLOR_GRAY);
            }
        }
    }
}


/**
 * The function affiche_all display all the element of the game, including the field,
   the waves of monsters, the mana, the towers and the gems.
 * @param grille: The grid of the game.
 * @param tabVague: The table that contains all monster's wave.
 * @param mana: The reserve of mana of the player.
 * @param game: The actual state of the game.
 * @param x: The coordinate x of the mouse's position.
 * @param x: The coordinate x of the mouse's position.
*/
void affiche_all(caseType grille[LARGEUR][HAUTEUR], TableVague *tabVague, Reserve mana, Game game, int x, int y){
    MLV_update_window();
    affiche_terrain(grille);
    initialise_terrain();
    affiche_ensemble(tabVague);
    affiche_mana(mana);
    affiche_tour(game.tours, game.nb_tours);
    affiche_gemmes(game.gemmes, game.nb_gemmes, x, y);
}


/**
 * The function get_color return the MLV color depending on the given tint.
 * @param teinte: The tint that provides the color.
 * @return: The MLV color corresponding to the tint.
*/
MLV_Color get_color(int teinte){
    if(teinte >= 330 || teinte <= 30) return MLV_COLOR_RED;
    if(teinte >= 210 && teinte <= 270) return MLV_COLOR_BLUE;
    if(teinte >= 90 && teinte <= 150) return MLV_COLOR_LIME_GREEN;
    if(teinte > 30 && teinte < 90) return MLV_COLOR_YELLOW;
    if(teinte > 150 && teinte < 210) return MLV_COLOR_SKY_BLUE;
    if(teinte > 270 && teinte < 330) return MLV_COLOR_PURPLE;
    return MLV_COLOR_BLACK;
}

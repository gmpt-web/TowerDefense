#ifndef __GEMMES__
#define __GEMMES__


/**
Enumeration for the type of the gem
*/
typedef enum{
    PURE,
    MIXTE
}Type_gem;


/**
 Enumeration for the element of the gem
*/
typedef enum{
    PYRO,
    HYDRO,
    DENDRO,
    NONE
}Element;


/**
 Structure that represent a gem
*/
typedef struct _gemmes_{
    Type_gem type; // Type de la gemme
    Element elem;  // Élément de la gemme
    int teinte;
    int niveau;
    int emprise; //enregistre si la gemme a été selectionné
}Gem;


/**
 Function that create a new gem
*/
Gem cree_gemme();

/**
 Function that permit the rearrangement of the list of gem
*/
Gem *rearrange_gemmes(Gem gemmes[20], int pos, int taille);


/**
 Function that can merge two gems (Level up)
*/
int fusion(Gem *gemme1, Gem gemme2);

#endif

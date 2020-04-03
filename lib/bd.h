#pragma once

#include "liste.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct compagnie
{
    size_t id;
    char nom[128];
    char code_postal[5];
    char mail[128];
} compagnie;

typedef struct compagnies
{
    node *tete;
} compagnies;

// Ouvre la BD pour l'écriture.
// Retourne vrai si la BD est prête pour écriture/lecture.
bool bd_ouvrir(char const* const chemin_bd);

// Ferme la BD.
void bd_fermer();

// Lit la table compagnie.
// Retourne une liste de compagnies.
compagnies* bd_lecture_compagnies();

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

// Lit la BD et rempli les structures données.
void bd_lecture(char const* const chemin_bd, compagnies** cs);

// Écrit la BD avec les structures données.
void bd_ecriture(char const* const chemin_bd, compagnie const* const c);

// Libère la mémoire allouée pour compagnies.
void free_compagnies(compagnies* cs);

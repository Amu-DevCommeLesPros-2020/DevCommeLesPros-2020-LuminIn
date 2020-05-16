#pragma once

#include "constantes.h"

#include <stddef.h>

typedef struct entreprise
{
    size_t id;
    char nom[L_NOM];
    char code_postal[L_CP];
    char mail[L_MAIL];
} entreprise;

typedef struct entreprises
{
    struct node *tete;
} entreprises;

// Initialise la liste des compagnies.
void en_init();

// Détruit la liste des compagnies.
void en_destroy();

// Crée un nouveau profil de compagnie.
size_t en_creer_profil(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL]);

// Supprimer un profil, donné un index.
void en_supprimer_profil(size_t const id);

// Rechercher une compagnie par id.
struct entreprise* en_recherche(size_t const id);

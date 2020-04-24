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
void co_init();

// Détruit la liste des compagnies.
void co_destroy();

// Crée un nouveau profil de compagnie.
size_t co_creer_profil(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL]);

// Supprimer un profil, donné un index.
void co_supprimer_profil(size_t const id);

// Modifier un profil, donné un index.
void co_modifier_profil(size_t const id, char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL]);

// Rechercher une compagnie par id.
struct entreprise* co_recherche(size_t const id);

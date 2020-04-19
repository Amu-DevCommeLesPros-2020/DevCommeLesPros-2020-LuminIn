#pragma once

#include <stddef.h>

typedef struct chercheur
{
    size_t id;
    char nom[128];
    char prenom[128];
    char mail[128];
    char code_postal[5];
    char competences[5][128];
    size_t id_collegues[5];
} chercheur;

typedef struct chercheurs
{
    struct node *tete;
} chercheurs;

// Initialise la liste des chercheurs.
void ch_init();

// Détruit la liste des chercheurs.
void ch_destroy();

// Crée un nouveau profil de chercheur.
size_t ch_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5]);

// Modifier un profil, donné un index.
void ch_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5]);

// Liste des chercheurs existants.
void ch_ids(size_t ids[10]);

// Rechercher un chercheur par id.
struct chercheur* ch_recherche(size_t const id);


#pragma once

#include "constantes.h"

#include <stddef.h>

typedef struct chercheur
{
    size_t id;
    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE];
    size_t id_collegues[N_COLLEGUES];
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
size_t ch_creer_profil(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char const competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES]);

// Supprimer un profil, donné un index.
void ch_supprimer_profil(size_t const id);

// Modifier un profil, donné un index.
void ch_modifier_profil(size_t const id, char const code_postal[L_CP], char const competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES]);

// Liste des chercheurs existants.
void ch_ids(size_t ids[N_POSTES]);

// Rechercher un chercheur par id.
struct chercheur* ch_recherche(size_t const id);


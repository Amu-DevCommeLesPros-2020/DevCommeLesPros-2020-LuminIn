#pragma once

#include <stddef.h>

struct chercheur;

// Initialise la liste des chercheurs.
void ch_init();

// Détruit la liste des chercheurs.
void ch_destroy();

// Crée un nouveau profil de chercheur.
size_t ch_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5]);

// Modifier un profil, donné un index.
void ch_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5]);

// Rechercher un chercheur par id.
struct chercheur* ch_recherche(size_t const id);


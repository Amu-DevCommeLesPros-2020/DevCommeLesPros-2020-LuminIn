#pragma once

#include <stddef.h>

struct entreprise;

// Initialise la liste des compagnies.
void co_init();

// Détruit la liste des compagnies.
void co_destroy();

// Crée un nouveau profil de compagnie.
size_t co_creer_profil(char const* const nom, char const code_postal[5], char const* const mail);

// Supprimer un profil, donné un index.
void co_supprimer_profil(size_t const id);

// Modifier un profil, donné un index.
void co_modifier_profil(size_t const id, char const* const nom, char const code_postal[5], char const* const mail);

// Rechercher une compagnie par id.
struct entreprise* co_recherche(size_t const id);

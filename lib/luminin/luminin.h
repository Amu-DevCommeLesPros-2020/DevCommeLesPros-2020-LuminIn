#pragma once

#include "recherche.h"

void lu_init(char const* const chemin_bd);

// Fonctions relative aux entreprises.
size_t lu_creer_profil_entreprise(char const* const nom, char const code_postal[5], char const* const mail);

void lu_supprimer_profil_entreprise(size_t const id);

void lu_modifier_profil_entreprise(size_t const id, char const* const nom, char const code_postal[5], char const* const mail);

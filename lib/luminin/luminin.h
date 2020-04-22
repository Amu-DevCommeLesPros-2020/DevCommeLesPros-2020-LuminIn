#pragma once

#include "recherche.h"

void lu_init(char const* const chemin_bd);

size_t lu_creer_profil_entreprise(char const* const nom, char const code_postal[5], char const* const mail);

void lu_supprimer_profil_entreprise(size_t const id);

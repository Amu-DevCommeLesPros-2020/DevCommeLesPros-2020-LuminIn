#pragma once

#include "constantes.h"

#include <stddef.h>

void lu_init(char const* const chemin_bd);

// Fonctions relatives aux entreprises.
size_t lu_creer_profil_entreprise(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL]);

void lu_supprimer_profil_entreprise(size_t const id);

void lu_modifier_profil_entreprise(size_t const id, char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL]);

// Fonctions relatives aux postes.
size_t lu_creer_poste(char const titre[L_TITRE], char const competences[N_COMPETENCES][L_COMPETENCE], size_t const id_compagnie);

void lu_supprimer_poste(size_t const id);

// Fonctions relatives aux recherches.
void lu_recherche_poste_par_competences(size_t const id_chercheur, size_t ids_poste[N_POSTES]);

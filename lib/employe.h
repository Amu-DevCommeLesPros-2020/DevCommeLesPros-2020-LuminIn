#pragma once

#include <stddef.h>

struct employe;

// Initialise la liste des employes.
void em_init();

// Détruit la liste des employes.
void em_destroy();

// Crée un nouveau profil d'employé.
size_t em_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5]);

// Modifier un profil, donné un index.
void em_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5]);

// Rechercher un employé par id.
struct employe* em_recherche(size_t const id);

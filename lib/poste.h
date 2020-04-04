#pragma once

#include "types.h"

// Initialise la liste des postes.
void po_init();

// Détruit la liste des postes.
void po_destroy();

// Créer un poste.
size_t po_creer_poste(char const* const titre, char const competences[5][128], size_t const id_compagnie);

// Supprimer un poste.
void po_supprimer_poste(size_t const id);

// Cherche un poste par id.
poste* po_recherche(size_t const id);

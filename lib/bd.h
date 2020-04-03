#pragma once

#include <stdbool.h>

// Ouvre la BD pour l'écriture.
// Retourne vrai si la BD est prête pour écriture/lecture.
bool bd_ouvrir(char const* const chemin_bd);

// Ferme la BD.
void bd_fermer();

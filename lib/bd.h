#pragma once

#include <stddef.h>

// Ouvre la BD pour l'écriture.
void bd_ouvrir(char const* const chemin_bd);

// Ferme la BD.
void bd_fermer();

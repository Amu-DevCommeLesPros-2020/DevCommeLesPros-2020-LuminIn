#pragma once

// Ouvre un fichier de journal.
void j_ouvrir(char const* const chemin);

// Ajoute un évènement au journal.
void j_ecrire(char const* const ligne);

// Ferme le fichier de journal.
void j_fermer();

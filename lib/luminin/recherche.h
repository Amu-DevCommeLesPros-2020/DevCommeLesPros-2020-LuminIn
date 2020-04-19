#pragma once

#include <stddef.h>

// Retourne la liste des postes pour lesquels le chercheur est qualifié par ordre de compétences remplies.
void recherche_poste_par_competences(size_t const id_chercheur, size_t ids_poste[10]);

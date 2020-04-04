#pragma once

#include "types.h"

// Donne le chemin de la BD.
void bd_init(char const* const chemin_bd);

// Lit la BD et rempli les structures données.
void bd_lecture(compagnies** cos);

// Lit la table compagnie dans la structure donnée.
void bd_lecture_compagnies(compagnies** cos);

// Écrit la BD avec les structures données.
void bd_ecriture(compagnies const* const cos);

// Écrit la structure compagnies dans la BD.
void bd_ecriture_compagnies(compagnies const* const cos);

// Libère la mémoire allouée pour compagnies.
void free_compagnies(compagnies* cos);

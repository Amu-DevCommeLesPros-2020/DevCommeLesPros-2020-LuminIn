#pragma once

#include "types.h"

// Lit la BD et rempli les structures données.
void bd_lecture(char const* const chemin_bd, compagnies** cos);

// Lit la table compagnie dans la structure donnée.
void bd_lecture_compagnies(char const* const chemin_bd, compagnies** cos);

// Écrit la BD avec les structures données.
void bd_ecriture(char const* const chemin_bd, compagnies const* const cos);

// Écrit la structure compagnies dans la BD.
void bd_ecriture_compagnies(char const* const chemin_bd, compagnies const* const cos);

// Libère la mémoire allouée pour compagnies.
void free_compagnies(compagnies* cos);

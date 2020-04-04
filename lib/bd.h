#pragma once

#include "types.h"

// Donne le chemin de la BD.
void bd_init(char const* const chemin_bd);

// Lit la BD et rempli les structures données.
void bd_lecture(compagnies** cos, postes** pos);

// Lit la table compagnie dans la structure donnée.
void bd_lecture_compagnies(compagnies** cos);

// Lit la table poste dans la structure donnée.
void bd_lecture_postes(postes** pos);

// Écrit la BD avec les structures données.
void bd_ecriture(compagnies const* const cos);

// Écrit la structure compagnies dans la BD.
void bd_ecriture_compagnies(compagnies const* const cos);

// Écrit la structure postes dans la BD.
void bd_ecriture_postes(postes const* const pos);

// Libère la mémoire allouée pour compagnies.
void free_compagnies(compagnies* cos);

// Libère la mémoire allouée pour postes.
void free_postes(postes* pos);

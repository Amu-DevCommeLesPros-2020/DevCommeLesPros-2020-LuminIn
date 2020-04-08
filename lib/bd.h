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

// Lit la table employe dans la structure donnée.
void bd_lecture_employes(employes** ems);

// Écrit la BD avec les structures données.
void bd_ecriture(compagnies const* const cos);

// Écrit la structure compagnies dans la BD.
void bd_ecriture_compagnies(compagnies const* const cos);

// Écrit la structure postes dans la BD.
void bd_ecriture_postes(postes const* const pos);

// Écrit la structure employes dans la BD.
void bd_ecriture_employes(employes const* const ems);

// Libère la mémoire allouée pour compagnies.
void free_compagnies(compagnies* cos);

// Libère la mémoire allouée pour postes.
void free_postes(postes* pos);

// Libère la mémoire allouée pour employes.
void free_employes(employes* ems);

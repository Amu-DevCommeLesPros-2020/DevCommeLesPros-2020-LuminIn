#pragma once

typedef struct chercheurs chercheurs;
typedef struct employes employes;
typedef struct entreprises entreprises;
typedef struct postes postes;

// Donne le chemin de la BD.
void bd_init(char const* const chemin_bd);

// Lit la BD et rempli les structures données.
void bd_lecture(chercheurs** chs, employes** ems, entreprises** ens, postes** pos);

// Lit la table entreprise dans la structure donnée.
void bd_lecture_entreprises(entreprises** ens);

// Lit la table poste dans la structure donnée.
void bd_lecture_postes(postes** pos);

// Lit la table employe dans la structure donnée.
void bd_lecture_employes(employes** ems);

// Lit la table chercheur dans la structure donnée.
void bd_lecture_chercheurs(chercheurs** chs);

// Écrit la BD avec les structures données.
void bd_ecriture(chercheurs const* const chs, employes const* const ems, entreprises const* const ens, postes const* const pos);

// Écrit la structure entreprises dans la BD.
void bd_ecriture_entreprises(entreprises const* const cos);

// Écrit la structure postes dans la BD.
void bd_ecriture_postes(postes const* const pos);

// Écrit la structure employes dans la BD.
void bd_ecriture_employes(employes const* const ems);

// Écrit la structure chercheurs dans la BD.
void bd_ecriture_chercheurs(chercheurs const* const chs);

// Libère la mémoire allouée pour entreprises.
void free_entreprises(entreprises* cos);

// Libère la mémoire allouée pour postes.
void free_postes(postes* pos);

// Libère la mémoire allouée pour employes.
void free_employes(employes* ems);

// Libère la mémoire allouée pour chercheurs.
void free_chercheurs(chercheurs* chs);

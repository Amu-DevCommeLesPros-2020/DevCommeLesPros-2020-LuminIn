#pragma once

#include "constantes.h"

#include <stddef.h>

typedef struct employe
{
    size_t id;
    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE];
    size_t id_entreprise;
    size_t id_collegues[N_COLLEGUES];
} employe;

typedef struct employes
{
    struct node *tete;
} employes;

// Initialise la liste des employes.
void em_init();

// Détruit la liste des employes.
void em_destroy();

// Crée un nouveau profil d'employé.
size_t em_creer_profil(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES]);

// Supprimer un profil, donné un index.
void em_supprimer_profil(size_t const id);

// Modifier un profil, donné un index.
void em_modifier_profil(size_t const id, char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES]);

// Liste des employés existants.
void em_ids(size_t ids[N_POSTES]);

// Rechercher un employé par id.
struct employe* em_recherche(size_t const id);

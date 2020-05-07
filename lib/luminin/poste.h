#pragma once

#include "constantes.h"

#include <stddef.h>

typedef struct poste
{
    size_t id;
    char titre[L_TITRE];
    char competences[N_COMPETENCES][L_COMPETENCE];
    size_t id_entreprise;
} poste;

typedef struct postes
{
    struct node *tete;
} postes;

// Initialise la liste des postes.
void po_init();

// Détruit la liste des postes.
void po_destroy();

// Créer un poste.
size_t po_creer_poste(char const titre[L_TITRE], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_compagnie);

// Supprimer un poste.
void po_supprimer_poste(size_t const id);

// Liste des postes existants.
void po_ids(size_t ids[N_POSTES]);

// Cherche un poste par id.
poste* po_recherche(size_t const id);

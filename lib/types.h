#pragma once

#include "liste.h"

#include <stddef.h>

typedef struct compagnie
{
    size_t id;
    char nom[128];
    char code_postal[5];
    char mail[128];
} compagnie;

typedef struct compagnies
{
    node *tete;
} compagnies;

typedef struct poste
{
    size_t id;
    char titre[128];
    char competences[5][128];
    size_t id_compagnie;
} poste;

typedef struct postes
{
    node *tete;
} postes;

typedef struct employe
{
    size_t id;
    char nom[128];
    char prenom[128];
    char mail[128];
    char code_postal[5];
    char competences[5][128];
    size_t id_entreprise;
    size_t id_collegues[5];
} employe;

typedef struct employes
{
    node *tete;
} employes;

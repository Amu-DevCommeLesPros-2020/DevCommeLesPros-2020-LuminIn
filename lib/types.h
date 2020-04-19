#pragma once

#include <stddef.h>

struct node;

typedef struct entreprise
{
    size_t id;
    char nom[128];
    char code_postal[5];
    char mail[128];
} entreprise;

typedef struct entreprises
{
    struct node *tete;
} entreprises;

typedef struct poste
{
    size_t id;
    char titre[128];
    char competences[5][128];
    size_t id_entreprise;
} poste;

typedef struct postes
{
    struct node *tete;
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
    struct node *tete;
} employes;

typedef struct chercheur
{
    size_t id;
    char nom[128];
    char prenom[128];
    char mail[128];
    char code_postal[5];
    char competences[5][128];
    size_t id_collegues[5];
} chercheur;

typedef struct chercheurs
{
    struct node *tete;
} chercheurs;

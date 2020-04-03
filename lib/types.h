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

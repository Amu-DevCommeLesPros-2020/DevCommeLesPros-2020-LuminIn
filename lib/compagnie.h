#pragma once

#include "types.h"

#include <stddef.h>

// Crée un nouveau profil de compagnie.
size_t co_creer_profil(compagnies *cos, char const* const nom, char const code_postal[5], char const* const mail);


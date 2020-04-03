#include "compagnie.h"

#include "types.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

size_t co_creer_profil(compagnies *cos, char const* const nom, char const code_postal[5], char const* const mail)
{
    compagnie *co = malloc(sizeof(compagnie));
    co->id = cos->tete ? ((compagnie*)(l_tail(cos->tete)->data))->id + 1 : 1;
    strcpy(co->nom, nom);
    strncpy(co->code_postal, code_postal, 5);
    strcpy(co->mail, mail);

    l_append(&(cos->tete), l_make_node(co));

    return co->id;
}

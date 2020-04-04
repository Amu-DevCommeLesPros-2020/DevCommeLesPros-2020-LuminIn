#include "compagnie.h"

#include "bd.h"
#include "journal.h"
#include "types.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

compagnies *cos_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &cos_->tete; (*n) && ((compagnie*)((*n)->data))->id != id && (*n)->next; (*n) = (*n)->next);
    return n;
}

void co_init()
{
    co_destroy();

    cos_ = malloc(sizeof(compagnies));
    cos_->tete = NULL;

    bd_lecture_compagnies(&cos_);
}

void co_destroy()
{
    free_compagnies(cos_);
    cos_ = NULL;
}

size_t co_creer_profil(char const* const nom, char const code_postal[5], char const* const mail)
{
    j_ecrire("Creation profil compagnie.");

    compagnie *co = malloc(sizeof(compagnie));
    co->id = cos_->tete ? ((compagnie*)(l_tail(cos_->tete)->data))->id + 1 : 1;
    strcpy(co->nom, nom);
    strncpy(co->code_postal, code_postal, 5);
    strcpy(co->mail, mail);

    l_append(&(cos_->tete), l_make_node(co));

    bd_ecriture_compagnies(cos_);

    return co->id;
}

void co_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil compagnie.");

    l_remove(recherche(id));

    bd_ecriture_compagnies(cos_);
}

void co_modifier_profil(size_t const id, char const* const nom, char const code_postal[5], char const* const mail)
{
    j_ecrire("Modification profil compagnie.");

    compagnie *co = co_recherche(id);
    if(co)
    {
        strcpy(co->nom, nom);
        strncpy(co->code_postal, code_postal, 5);
        strcpy(co->mail, mail);
    }
}

compagnie* co_recherche(size_t const id)
{
    j_ecrire("Recherche profil compagnie.");

    node **n = recherche(id);

    return *n ? (compagnie*)((*n)->data) : NULL;
}

#include "entreprise.h"

#include "bd.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

entreprises *cos_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &cos_->tete; (*n) && ((entreprise*)((*n)->data))->id != id; n = &((*n)->next));
    return n;
}

void co_init()
{
    co_destroy();

    bd_lecture_entreprises(&cos_);
}

void co_destroy()
{
    free_entreprises(cos_);
    cos_ = NULL;
}

size_t co_creer_profil(char const* const nom, char const code_postal[5], char const* const mail)
{
    j_ecrire("Creation profil compagnie.");

    entreprise *co = malloc(sizeof(entreprise));
    co->id = cos_->tete ? ((entreprise*)(l_tail(cos_->tete)->data))->id + 1 : 1;
    strcpy(co->nom, nom);
    strncpy(co->code_postal, code_postal, 5);
    strcpy(co->mail, mail);

    l_append(&(cos_->tete), l_make_node(co));

    bd_ecriture_entreprises(cos_);

    return co->id;
}

void co_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil compagnie.");

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const next = n == &cos_->tete ? (*n)->next : cos_->tete;
    l_remove(n);
    cos_->tete = next;

    bd_ecriture_entreprises(cos_);
}

void co_modifier_profil(size_t const id, char const* const nom, char const code_postal[5], char const* const mail)
{
    j_ecrire("Modification profil compagnie.");

    entreprise *co = co_recherche(id);
    if(co)
    {
        memmove(co->nom, nom, 128);
        memmove(co->code_postal, code_postal, 5);
        memmove(co->mail, mail, 128);

        bd_ecriture_entreprises(cos_);
    }
}

entreprise* co_recherche(size_t const id)
{
    j_ecrire("Recherche profil compagnie.");

    node **n = recherche(id);

    return *n ? (entreprise*)((*n)->data) : NULL;
}

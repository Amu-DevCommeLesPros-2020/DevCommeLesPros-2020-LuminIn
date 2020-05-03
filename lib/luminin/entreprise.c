#include "entreprise.h"

#include "bd.h"
#include "constantes.h"

#include "journal/journal.h"
#include "liste/liste.h"
#include "utilite/stringize.h"

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

size_t co_creer_profil(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL])
{
    j_ecrire("Creation profil compagnie. [nom=%s,code_postal=%" STRINGIZE(L_CP) "c,mail=%s]", nom, code_postal, mail);

    entreprise *co = malloc(sizeof(entreprise));
    co->id = cos_->tete ? ((entreprise*)(l_tail(cos_->tete)->data))->id + 1 : 1;
    strcpy(co->nom, nom);
    strncpy(co->code_postal, code_postal, L_CP);
    strcpy(co->mail, mail);

    l_append(&(cos_->tete), l_make_node(co));

    bd_ecriture_entreprises(cos_);

    j_ecrire("Compagnie créée. [identifiant=%zu]", co->id);

    return co->id;
}

void co_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil compagnie. [identifiant=%zu]", id);

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const next = n == &cos_->tete ? (*n)->next : cos_->tete;
    l_remove(n);
    cos_->tete = next;

    bd_ecriture_entreprises(cos_);
}

entreprise* co_recherche(size_t const id)
{
    j_ecrire("Recherche profil compagnie. [identifiant=%zu]", id);

    node **n = recherche(id);

    j_ecrire("Recherche %sfructueuse.", *n ? "" : "in");

    return *n ? (entreprise*)((*n)->data) : NULL;
}

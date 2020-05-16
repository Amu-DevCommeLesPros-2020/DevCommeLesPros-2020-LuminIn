#include "entreprise.h"

#include "bd.h"
#include "constantes.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

entreprises *ens_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &ens_->tete; (*n) && ((entreprise*)((*n)->data))->id != id; n = &((*n)->next));
    return n;
}

void en_init()
{
    en_destroy();

    bd_lecture_entreprises(&ens_);
}

void en_destroy()
{
    free_entreprises(ens_);
    ens_ = NULL;
}

size_t en_creer_profil(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL])
{
    j_ecrire("Creation profil compagnie. [nom=%s,code_postal=%s,mail=%s]", nom, code_postal, mail);

    entreprise *co = calloc(1, sizeof(entreprise));
    co->id = ens_->tete ? ((entreprise*)(l_tail(ens_->tete)->data))->id + 1 : I_ENTREPRISE;
    strcpy(co->nom, nom);
    strcpy(co->code_postal, code_postal);
    strcpy(co->mail, mail);

    l_append(&(ens_->tete), l_make_node(co));

    bd_ecriture_entreprises(ens_);

    j_ecrire("Compagnie créée. [identifiant=%zu]", co->id);

    return co->id;
}

void en_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil compagnie. [identifiant=%zu]", id);

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const next = n == &ens_->tete ? (*n)->next : ens_->tete;
    l_remove(n);
    ens_->tete = next;

    bd_ecriture_entreprises(ens_);
}

entreprise* en_recherche(size_t const id)
{
    j_ecrire("Recherche profil compagnie. [identifiant=%zu]", id);

    node **n = recherche(id);

    j_ecrire("Recherche %sfructueuse.", *n ? "" : "in");

    return *n ? (entreprise*)((*n)->data) : NULL;
}

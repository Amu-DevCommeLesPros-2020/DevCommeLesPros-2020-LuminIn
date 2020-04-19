#include "poste.h"

#include "bd.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stdlib.h>
#include <string.h>

postes *pos_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &pos_->tete; (*n) && ((poste*)((*n)->data))->id != id && (*n)->next; (*n) = (*n)->next);
    return n;
}

void po_init()
{
    po_destroy();

    pos_ = malloc(sizeof(postes));
    pos_->tete = NULL;

    bd_lecture_postes(&pos_);
}

void po_destroy()
{
    free_postes(pos_);
    pos_ = NULL;
}

size_t po_creer_poste(char const* const titre, char const competences[5][128], size_t const id_compagnie)
{
    j_ecrire("Creation poste.");

    poste *po = malloc(sizeof(poste));
    po->id = pos_->tete ? ((poste*)(l_tail(pos_->tete)->data))->id + 1 : 1;
    strcpy(po->titre, titre);
    for(int i = 0; i != 5; ++i)
    {
        strcpy(po->competences[i], competences[i]);
    }
    po->id_entreprise = id_compagnie;

    l_append(&(pos_->tete), l_make_node(po));

    bd_ecriture_postes(pos_);

    return po->id;
}

void po_supprimer_poste(size_t const id)
{
    j_ecrire("Suppression poste.");

    l_remove(recherche(id));

    bd_ecriture_postes(pos_);
}

void po_ids(size_t ids[10])
{
    int i = 0;
    for(node *n = pos_->tete; n; n = n->next)
    {
        ids[i++] = ((poste*)(n->data))->id;
    }
}

poste* po_recherche(size_t const id)
{
    j_ecrire("Recherche poste.");

    node **n = recherche(id);

    return *n ? (poste*)((*n)->data) : NULL;
}

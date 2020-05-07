#include "poste.h"

#include "bd.h"
#include "constantes.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stdlib.h>
#include <string.h>

postes *pos_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &pos_->tete; (*n) && ((poste*)((*n)->data))->id != id; n = &((*n)->next));
    return n;
}

void po_init()
{
    po_destroy();

    bd_lecture_postes(&pos_);
}

void po_destroy()
{
    free_postes(pos_);
    pos_ = NULL;
}

size_t po_creer_poste(char const titre[L_TITRE], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_compagnie)
{
    j_ecrire("Creation poste. [titre=%s,competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,id_entreprise=%zu]", titre, competences[0], competences[1], competences[2], competences[3], competences[4], id_compagnie);

    poste *po = calloc(1, sizeof(poste));
    po->id = pos_->tete ? ((poste*)(l_tail(pos_->tete)->data))->id + 1 : I_POSTE;
    strcpy(po->titre, titre);
    for(int i = 0; competences[i] && i != N_COMPETENCES; ++i)
    {
        strcpy(po->competences[i], competences[i]);
    }
    po->id_entreprise = id_compagnie;

    l_append(&(pos_->tete), l_make_node(po));

    bd_ecriture_postes(pos_);

    j_ecrire("Poste créé. [identifiant=%zu]", po->id);
    
    return po->id;
}

void po_supprimer_poste(size_t const id)
{
    j_ecrire("Suppression poste. [identifiant=%zu]", id);

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const next = n == &pos_->tete ? (*n)->next : pos_->tete;
    l_remove(n);
    pos_->tete = next;

    bd_ecriture_postes(pos_);
}

void po_ids(size_t ids[N_POSTES])
{
    memset(ids, 0, N_POSTES * sizeof(size_t));
    int i = 0;
    for(node const* n = pos_->tete; n; n = n->next)
    {
        ids[i++] = ((poste*)(n->data))->id;
    }
}

poste* po_recherche(size_t const id)
{
    j_ecrire("Recherche poste. [identifiant=%zu]", id);

    node **n = recherche(id);

    j_ecrire("Recherche %sfructueuse.", *n ? "" : "in");

    return *n ? (poste*)((*n)->data) : NULL;
}

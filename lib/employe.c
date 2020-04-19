#include "employe.h"

#include "bd.h"
#include "liste.h"
#include "journal.h"
#include "types.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

employes *ems_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &ems_->tete; (*n) && ((employe*)((*n)->data))->id != id && (*n)->next; (*n) = (*n)->next);
    return n;
}

void em_init()
{
    em_destroy();

    ems_ = malloc(sizeof(employes));
    ems_->tete = NULL;

    bd_lecture_employes(&ems_);
}

void em_destroy()
{
    free_employes(ems_);
    ems_ = NULL;
}

size_t em_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5])
{
    j_ecrire("Creation profil d'employe.");

    employe *em = malloc(sizeof(employe));
    em->id = ems_->tete ? ((poste*)(l_tail(ems_->tete)->data))->id + 1 : 1;
    strcpy(em->nom, nom);
    strcpy(em->prenom, prenom);
    strcpy(em->mail, mail);
    strncpy(em->code_postal, code_postal, 5);
    for(int i = 0; i != 5; ++i)
    {
        strcpy(em->competences[i], competences[i]);
    }
    em->id_entreprise = id_entreprise;
    for(int i = 0; i != 5; ++i)
    {
        em->id_collegues[i] = id_collegues[i];
    }

    l_append(&(ems_->tete), l_make_node(em));

    bd_ecriture_employes(ems_);

    return em->id;
}

void em_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5])
{
    j_ecrire("Modification profil d'employe.");

    employe *em = em_recherche(id);
    if(em)
    {
        strcpy(em->nom, nom);
        strcpy(em->prenom, prenom);
        strcpy(em->mail, mail);
        strncpy(em->code_postal, code_postal, 5);
        for(int i = 0; i != 5; ++i)
        {
            strcpy(em->competences[i], competences[i]);
        }
        em->id_entreprise = id_entreprise;
        for(int i = 0; i != 5; ++i)
        {
            em->id_collegues[i] = id_collegues[i];
        }

        l_append(&(ems_->tete), l_make_node(em));

        bd_ecriture_employes(ems_);
    }
}

employe* em_recherche(size_t const id)
{
    j_ecrire("Recherche profil employe.");

    node **n = recherche(id);

    return *n ? (employe*)((*n)->data) : NULL;
}

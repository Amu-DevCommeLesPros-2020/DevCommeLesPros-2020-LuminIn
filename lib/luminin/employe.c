#include "employe.h"

#include "bd.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

employes *ems_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &ems_->tete; (*n) && ((employe*)((*n)->data))->id != id; n = &((*n)->next));
    return n;
}

void em_init()
{
    em_destroy();

    bd_lecture_employes(&ems_);
}

void em_destroy()
{
    free_employes(ems_);
    ems_ = NULL;
}

size_t em_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5])
{
    j_ecrire("Creation profil d'employe. [nom=%s,prenom=%s,mail=%s,code_postal=%5s,\
              competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
              id_entreprise=%zu,collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", nom, prenom, mail, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_entreprise, id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

    employe *em = malloc(sizeof(employe));
    em->id = ems_->tete ? ((employe*)(l_tail(ems_->tete)->data))->id + 1 : 1;
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

    j_ecrire("Profil d'employé créé. [identifiant=%zu", em->id);

    return em->id;
}

void em_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_entreprise, size_t const id_collegues[5])
{
    j_ecrire("Modification profil d'employe. [identifiant=%zu,nom=%s,prenom=%s,mail=%s,code_postal=%5s,\
              competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
              id_entreprise=%zu,collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", id, nom, prenom, mail, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_entreprise, id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

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

        bd_ecriture_employes(ems_);
    }
}

employe* em_recherche(size_t const id)
{
    j_ecrire("Recherche profil employe. [identifiant=%zu]", id);

    node **n = recherche(id);

    j_ecrire("Recherche %sfructueuse.", *n ? "" : "in");

    return *n ? (employe*)((*n)->data) : NULL;
}

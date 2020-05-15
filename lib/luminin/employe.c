#include "employe.h"

#include "bd.h"
#include "constantes.h"

#include "journal/journal.h"
#include "liste/liste.h"
#include "utilite/stringize.h"

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

size_t em_creer_profil(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES])
{
    j_ecrire("Creation profil d'employe. [nom=%s,prenom=%s,mail=%s,code_postal=%s,\
              competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
              id_entreprise=%zu,collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", nom, prenom, mail, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_entreprise, id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

    employe *em = calloc(1, sizeof(employe));
    em->id = ems_->tete ? ((employe*)(l_tail(ems_->tete)->data))->id + 1 : I_EMPLOYE;
    strcpy(em->nom, nom);
    strcpy(em->prenom, prenom);
    strcpy(em->mail, mail);
    strcpy(em->code_postal, code_postal);
    for(int i = 0; competences[i] && i != N_COMPETENCES; ++i)
    {
        strcpy(em->competences[i], competences[i]);
    }
    em->id_entreprise = id_entreprise;
    for(int i = 0; id_collegues[i] && i != N_COLLEGUES; ++i)
    {
        em->id_collegues[i] = id_collegues[i];
    }

    l_append(&(ems_->tete), l_make_node(em));

    bd_ecriture_employes(ems_);

    j_ecrire("Profil d'employé créé. [identifiant=%zu", em->id);

    return em->id;
}

void em_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil employe. [identifiant=%zu]", id);

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const tete = (n == &ems_->tete ? (*n)->next : ems_->tete);
    l_remove(n);
    ems_->tete = tete;

    bd_ecriture_employes(ems_);
}

void em_modifier_profil(size_t const id, char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES])
{
    j_ecrire("Modification profil d'employe. [identifiant=%zu,code_postal=%s,\
              competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
              id_entreprise=%zu,collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", id, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_entreprise, id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

    employe *em = em_recherche(id);
    if(em)
    {
        strcpy(em->code_postal, code_postal);
        for(int i = 0; i != N_COMPETENCES; ++i)
        {
            strcpy(em->competences[i], competences[i]);
        }
        em->id_entreprise = id_entreprise;
        for(int i = 0; i != N_COLLEGUES; ++i)
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

#include "chercheur.h"

#include "bd.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

chercheurs *chs_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &chs_->tete; (*n) && ((chercheur*)((*n)->data))->id != id; n = &((*n)->next));
    return n;
}

void ch_init()
{
    ch_destroy();

    bd_lecture_chercheurs(&chs_);
}

void ch_destroy()
{
    free_chercheurs(chs_);
    chs_ = NULL;
}

size_t ch_creer_profil(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES])
{
    j_ecrire("Création profil de chercheur. [nom=%s,prenom=%s,mail=%s,code_postal=%s,\
competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", nom, prenom, mail, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

    chercheur *ch = calloc(1, sizeof(chercheur));
    ch->id = chs_->tete ? ((chercheur*)(l_tail(chs_->tete)->data))->id + 1 : I_CHERCHEUR + 1;
    strcpy(ch->nom, nom);
    strcpy(ch->prenom, prenom);
    strcpy(ch->mail, mail);
    strcpy(ch->code_postal, code_postal);
    for(int i = 0; i != N_COMPETENCES && competences[i]; ++i)
    {
        strcpy(ch->competences[i], competences[i]);
    }
    for(int i = 0; i != N_COLLEGUES && id_collegues[i]; ++i)
    {
        ch->id_collegues[i] = id_collegues[i];
    }

    l_append(&(chs_->tete), l_make_node(ch));

    bd_ecriture_chercheurs(chs_);

    j_ecrire("Profil de chercheur créé. [identifiant=%zu", ch->id);

    return ch->id;
}

void ch_supprimer_profil(size_t const id)
{
    j_ecrire("Suppression profil chercheur. [identifiant=%zu]", id);

    // Si la tête est supprimée, la tête est décalée à la deuxième node.
    node** n = recherche(id);
    node* const next = n == &chs_->tete ? (*n)->next : chs_->tete;
    l_remove(n);
    chs_->tete = next;

    bd_ecriture_chercheurs(chs_);
}

void ch_modifier_profil(size_t const id, char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES])
{
    j_ecrire("Modification profil de chercheur. [identifiant=%zu,code_postal=%s,\
competence[0]=%s,competence[1]=%s,competence[2]=%s,competence[3]=%s,competence[4]=%s,\
collegues[0]=%zu,collegues[1]=%zu,collegues[2]=%zu,collegues[3]=%zu,collegues[4]=%zu", id, code_postal, competences[0], competences[1], competences[2], competences[3], competences[4], id_collegues[0], id_collegues[1], id_collegues[2], id_collegues[3], id_collegues[4]);

    chercheur *ch = ch_recherche(id);
    if(ch)
    {
        strcpy(ch->code_postal, code_postal);
        for(int i = 0; i != N_COMPETENCES; ++i)
        {
            strcpy(ch->competences[i], competences[i]);
        }
        for(int i = 0; i != N_COLLEGUES; ++i)
        {
            ch->id_collegues[i] = id_collegues[i];
        }

        bd_ecriture_chercheurs(chs_);
    }
}

void ch_ids(size_t ids[N_CHERCHEURS])
{
    memset(ids, 0, N_CHERCHEURS * sizeof(size_t));
    int i = 0;
    for(node const* n = chs_->tete; n; n = n->next)
    {
        ids[i++] = ((chercheur*)(n->data))->id;
    }
}

chercheur* ch_recherche(size_t const id)
{
    j_ecrire("Recherche profil chercheur. [identifiant=%zu]", id);

    node **n = recherche(id);

    j_ecrire("Recherche %sfructueuse.", *n ? "" : "in");

    return *n ? (chercheur*)((*n)->data) : NULL;
}

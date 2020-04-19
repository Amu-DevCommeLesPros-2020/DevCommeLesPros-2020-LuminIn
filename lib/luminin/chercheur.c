#include "chercheur.h"

#include "bd.h"
#include "types.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

chercheurs *chs_ = NULL;

static node** recherche(size_t const id)
{
    node** n;
    for(n = &chs_->tete; (*n) && ((chercheur*)((*n)->data))->id != id && (*n)->next; (*n) = (*n)->next);
    return n;
}

void ch_init()
{
    ch_destroy();

    chs_ = malloc(sizeof(chercheur));
    chs_->tete = NULL;

    bd_lecture_chercheurs(&chs_);
}

void ch_destroy()
{
    free_chercheurs(chs_);
    chs_ = NULL;
}

size_t ch_creer_profil(char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5])
{
    j_ecrire("Creation profil de chercheur.");

    chercheur *ch = malloc(sizeof(chercheur));
    ch->id = chs_->tete ? ((poste*)(l_tail(chs_->tete)->data))->id + 1 : 1;
    strcpy(ch->nom, nom);
    strcpy(ch->prenom, prenom);
    strcpy(ch->mail, mail);
    strncpy(ch->code_postal, code_postal, 5);
    for(int i = 0; i != 5; ++i)
    {
        strcpy(ch->competences[i], competences[i]);
    }
    for(int i = 0; i != 5; ++i)
    {
        ch->id_collegues[i] = id_collegues[i];
    }

    l_append(&(chs_->tete), l_make_node(ch));

    bd_ecriture_chercheurs(chs_);

    return ch->id;
}

void ch_modifier_profil(size_t const id, char const* const nom, char const* const prenom, char const* const mail, char const code_postal[5], char const competences[5][128], size_t const id_collegues[5])
{
    j_ecrire("Modification profil de chercheur.");

    chercheur *ch = ch_recherche(id);
    if(ch)
    {
        strcpy(ch->nom, nom);
        strcpy(ch->prenom, prenom);
        strcpy(ch->mail, mail);
        strncpy(ch->code_postal, code_postal, 5);
        for(int i = 0; i != 5; ++i)
        {
            strcpy(ch->competences[i], competences[i]);
        }
        for(int i = 0; i != 5; ++i)
        {
            ch->id_collegues[i] = id_collegues[i];
        }

        l_append(&(chs_->tete), l_make_node(ch));

        bd_ecriture_chercheurs(chs_);
    }
}

chercheur* ch_recherche(size_t const id)
{
    j_ecrire("Recherche profil chercheur.");

    node **n = recherche(id);

    return *n ? (chercheur*)((*n)->data) : NULL;
}

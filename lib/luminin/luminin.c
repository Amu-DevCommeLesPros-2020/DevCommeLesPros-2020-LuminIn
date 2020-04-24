#include "luminin.h"

#include "bd.h"
#include "chercheur.h"
#include "employe.h"
#include "entreprise.h"
#include "poste.h"
#include "recherche.h"

#include <stddef.h>
#include <string.h>

void lu_init(char const* const chemin_bd)
{
    bd_init(chemin_bd);

    ch_init();
    em_init();
    co_init();
    po_init();
}

size_t lu_creer_profil_entreprise(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL])
{
    return co_creer_profil(nom, code_postal, mail);
}

void lu_supprimer_profil_entreprise(size_t const id)
{
    // Supprime tout les postes ouverts en lien avec cette entreprise.
    size_t ids_poste[N_POSTES];
    po_ids(ids_poste);
    for(size_t i = 0; i != N_POSTES && ids_poste[i] != 0; ++i)
    {
        poste *p = po_recherche(ids_poste[i]);
        if(p && p->id_entreprise == id)
        {
            po_supprimer_poste(ids_poste[i]);
        }
    }

    co_supprimer_profil(id);
}

void lu_modifier_profil_entreprise(size_t const id, char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL])
{
    entreprise *e = co_recherche(id);
    co_modifier_profil(id, strlen(nom) ? nom : e->nom, strlen(code_postal) ? code_postal : e->code_postal, strlen(mail) ? mail : e->mail);
}

void lu_recherche_poste_par_competences(size_t const id_chercheur, size_t ids_poste[N_POSTES])
{
    recherche_poste_par_competences(id_chercheur, ids_poste);
}

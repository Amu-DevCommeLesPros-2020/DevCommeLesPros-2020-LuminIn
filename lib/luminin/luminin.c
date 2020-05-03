#include "luminin.h"

#include "bd.h"
#include "chercheur.h"
#include "employe.h"
#include "entreprise.h"
#include "poste.h"

#include <assert.h>
#include <stdbool.h>
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

char const* lu_nom_entreprise(size_t const id)
{
    entreprise const* const e = co_recherche(id);
    return e ? e->nom : NULL;
}

void lu_profil_entreprise(size_t const id, char nom[L_NOM], char code_postal[L_CP], char mail[L_MAIL])
{
    entreprise const* const e = co_recherche(id);
    if(e)
    {
        strcpy(nom, e->nom);
        strncpy(code_postal, e->code_postal, L_CP);
        strcpy(mail, e->mail);
    }
    else
    {
        strcpy(nom, "");
        strcpy(code_postal, "");
        strcpy(mail, "");
    }
}

size_t lu_creer_poste(char const titre[L_TITRE], char const competences[N_COMPETENCES][L_COMPETENCE], size_t const id_compagnie)
{
    return po_creer_poste(titre, competences, id_compagnie);
}

void lu_supprimer_poste(size_t const id)
{
    po_supprimer_poste(id);
}

size_t lu_creer_profil_chercheur(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char const competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES])
{
    return ch_creer_profil(nom, prenom, mail, code_postal, competences, id_collegues);
}

void lu_supprimer_profil_chercheur(size_t const id)
{
    ch_supprimer_profil(id);
}

void lu_modifier_profil_chercheur(size_t const id, char code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t id_collegues[N_COLLEGUES])
{
    chercheur *c = ch_recherche(id);
    assert(c);

    ch_modifier_profil(id, code_postal, competences, id_collegues);
}

char const* lu_nom_chercheur(size_t const id)
{
    chercheur const* const c = ch_recherche(id);
    return c ? c->nom : NULL;
}

void lu_profil_chercheur(size_t id, char nom[L_NOM], char prenom[L_PRENOM], char mail[L_MAIL], char code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t id_collegues[N_COLLEGUES])
{
    chercheur const*  const c = ch_recherche(id);
    if(c)
    {
        strcpy(nom, c->nom);
        strcpy(prenom, c->prenom);
        strcpy(mail, c->mail);
        strncpy(code_postal, c->code_postal, L_CP);
        memcpy(competences, c->competences, NL_COMPETENCES);
        memcpy(id_collegues, c->id_collegues, N_COLLEGUES * sizeof(size_t));
    }
    else
    {
        strcpy(nom, "");
        strcpy(prenom, "");
        strcpy(mail, "");
        strcpy(code_postal, "");
        memcpy(competences, "\0", NL_COMPETENCES);
        memset(id_collegues, 0, N_COLLEGUES * sizeof(size_t));
    }
}

void lu_recherche_poste_par_competences(size_t const id_chercheur, size_t ids_poste[N_POSTES])
{
    memset(ids_poste, 0, N_POSTES * sizeof(size_t));

    chercheur const* const c = ch_recherche(id_chercheur);

    // Liste des postes pour lesquels le chercheur est qualifié et le nombre de compétences remplies.
    size_t postes_qualifies[N_POSTES][2];
    memset(postes_qualifies, 0, N_POSTES * 2 * sizeof(size_t));
    size_t q = 0;   // Nombre de postes pour lequel le chercheur se qualifie.

    // Liste des postes disponibles.
    size_t ids[N_POSTES] = {0};
    po_ids(ids);

    // Recherche parmi les postes ceux pour lesquels le chercheur se qualifie et le nombre de compétences remplies.
    poste* p = po_recherche(ids[0]);
    for(size_t i = 0; p && i != N_POSTES; p = po_recherche(ids[++i]))
    {
        bool qualifie = false;

        // Pour chaque compétence du chercheur...
        for(size_t cci = 0; strlen(c->competences[cci]) && cci != N_COMPETENCES; ++cci)
        {
            // Pour chaque compétence requise du poste...
            for(size_t pci = 0; strlen(p->competences[pci]) && pci != N_COMPETENCES; ++pci)
            {
                // Si c'est la même...
                if(strcmp(c->competences[cci], p->competences[pci]) == 0)
                {
                    // Le chercheur est qualifié et on incrémente le nombre de compétences qui correspondent.
                    qualifie = true;
                    postes_qualifies[q][0] = p->id;
                    ++postes_qualifies[q][1];
                }
            }
        }

        if(qualifie)
        {
            ++q;
        }
    }

    // Arrangement des postes pour lequel le chercheur se qualifie par ordre décroissant du nombre de qualifications correspondantes.
    for(size_t ipi = 0; q--; ++ipi)
    {
        size_t max_i = 0;
        for(size_t i = 1; i != N_POSTES; ++i)
        {
            if(postes_qualifies[i][1] > postes_qualifies[max_i][1])
            {
                max_i = i;
            }
        }

        ids_poste[ipi] = postes_qualifies[max_i][0];
        postes_qualifies[max_i][0] = 0;
        postes_qualifies[max_i][1] = 0;
    }
}

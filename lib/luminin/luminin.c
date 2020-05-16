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
    en_init();
    po_init();
}

void lu_destroy()
{
    ch_destroy();
    em_destroy();
    en_destroy();
    po_destroy();
}

size_t lu_creer_profil_entreprise(char const nom[L_NOM], char const code_postal[L_CP], char const mail[L_MAIL])
{
    return en_creer_profil(nom, code_postal, mail);
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

    en_supprimer_profil(id);
}

char const* lu_nom_entreprise(size_t const id)
{
    entreprise const* const e = en_recherche(id);
    return e ? e->nom : NULL;
}

void lu_profil_entreprise(size_t const id, char* const nom, char* const code_postal, char* const mail)
{
    entreprise const* const e = en_recherche(id);
    if(e)
    {
        if(nom) strcpy(nom, e->nom);
        if(code_postal) strcpy(code_postal, e->code_postal);
        if(mail) strcpy(mail, e->mail);
    }
    else
    {
        strcpy(nom, "");
        strcpy(code_postal, "");
        strcpy(mail, "");
    }
}

size_t lu_creer_poste(char const titre[L_TITRE], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_compagnie)
{
    return po_creer_poste(titre, competences, id_compagnie);
}

void lu_supprimer_poste(size_t const id)
{
    po_supprimer_poste(id);
}

void lu_postes_par_entreprise(size_t const id_compagnie, size_t ids_poste[N_POSTES])
{
    memset(ids_poste, 0, N_POSTES * sizeof(size_t));

    size_t ids[N_POSTES];
    po_ids(ids);
    poste const* p = po_recherche(ids[0]);
    for(size_t i = 0, j = 0; p && i != N_POSTES; p = po_recherche(ids[++i]))
    {
        if(p->id_entreprise == id_compagnie)
        {
            ids_poste[j++] = p->id;
        }
    }
}

void lu_poste(size_t const id_poste, char* const titre, char competences[][L_COMPETENCE], size_t* const id_compagnie)
{
    poste const* const p = po_recherche(id_poste);
    if(p)
    {
        if(titre) strcpy(titre, p->titre);
        if(competences) memcpy(competences, p->competences, NL_COMPETENCES);
        if(id_compagnie) *id_compagnie = p->id_entreprise;
    }
    else
    {
        if(titre) strcpy(titre, "");
        if(competences) memcpy(competences, "", NL_COMPETENCES);
        if(id_compagnie) *id_compagnie = 0;
    }
}

size_t lu_creer_profil_chercheur(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES])
{
    return ch_creer_profil(nom, prenom, mail, code_postal, competences, id_collegues);
}

void lu_supprimer_profil_chercheur(size_t const id)
{
    ch_supprimer_profil(id);
}

void lu_modifier_profil_chercheur(size_t const id, char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_collegues[N_COLLEGUES])
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

void lu_profil_chercheur(size_t id, char* const nom, char* const prenom, char* const mail, char* const code_postal, char competences[][L_COMPETENCE], size_t* const id_collegues)
{
    chercheur const*  const c = ch_recherche(id);
    if(c)
    {
        if(nom) strcpy(nom, c->nom);
        if(prenom) strcpy(prenom, c->prenom);
        if(mail) strcpy(mail, c->mail);
        if(code_postal) strcpy(code_postal, c->code_postal);
        if(competences) memcpy(competences, c->competences, NL_COMPETENCES);
        if(id_collegues) memcpy(id_collegues, c->id_collegues, N_COLLEGUES * sizeof(size_t));
    }
    else
    {
        if(nom) strcpy(nom, "");
        if(prenom) strcpy(prenom, "");
        if(mail) strcpy(mail, "");
        if(code_postal) strcpy(code_postal, "");
        if(competences) memcpy(competences, "\0", NL_COMPETENCES);
        if(id_collegues) memset(id_collegues, 0, N_COLLEGUES * sizeof(size_t));
    }
}

size_t lu_creer_profil_employe(char const nom[L_NOM], char const prenom[L_PRENOM], char const mail[L_MAIL], char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES])
{
    return em_creer_profil(nom, prenom, mail, code_postal, competences, id_entreprise, id_collegues);
}

void lu_supprimer_profil_employe(size_t const id)
{
    em_supprimer_profil(id);
}

void lu_modifier_profil_employe(size_t id, char const code_postal[L_CP], char competences[N_COMPETENCES][L_COMPETENCE], size_t const id_entreprise, size_t const id_collegues[N_COLLEGUES])
{
    employe *e = em_recherche(id);
    assert(e);

    em_modifier_profil(id, code_postal, competences, id_entreprise, id_collegues);
}

char const* lu_nom_employe(size_t const id)
{
    employe const* const e = em_recherche(id);
    return e ? e->nom : NULL;
}

void lu_profil_employe(size_t id, char* const nom, char* const prenom, char* const mail, char* const code_postal, char competences[][L_COMPETENCE], size_t* const id_entreprise, size_t* const id_collegues)
{
    employe const* const e = em_recherche(id);
    if(e)
    {
        if(nom) strcpy(nom, e->nom);
        if(prenom) strcpy(prenom, e->prenom);
        if(mail) strcpy(mail, e->mail);
        if(code_postal) strcpy(code_postal, e->code_postal);
        if(competences) memcpy(competences, e->competences, NL_COMPETENCES);
        if(id_entreprise) *id_entreprise = e->id_entreprise;
        if(id_collegues) memcpy(id_collegues, e->id_collegues, N_COLLEGUES * sizeof(size_t));
    }
    else
    {
        if(nom) strcpy(nom, "");
        if(prenom) strcpy(prenom, "");
        if(mail) strcpy(mail, "");
        if(code_postal) strcpy(code_postal, "");
        if(competences) memcpy(competences, "\0", NL_COMPETENCES);
        if(id_entreprise) *id_entreprise = 0;
        if(id_collegues) memset(id_collegues, 0, N_COLLEGUES * sizeof(size_t));
    }
}

void lu_recherche_poste_par_competences(char competences[N_COMPETENCES][L_COMPETENCE], size_t ids_poste[N_POSTES])
{
    memset(ids_poste, 0, N_POSTES * sizeof(size_t));

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
        for(size_t cci = 0; strlen(competences[cci]) && cci != N_COMPETENCES; ++cci)
        {
            // Pour chaque compétence requise du poste...
            for(size_t pci = 0; strlen(p->competences[pci]) && pci != N_COMPETENCES; ++pci)
            {
                // Si c'est la même...
                if(strcmp(competences[cci], p->competences[pci]) == 0)
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

void lu_recherche_poste_par_competences_code_postal(char competences[N_COMPETENCES][L_COMPETENCE], char const code_postal[L_CP], size_t ids_poste[N_POSTES])
{
    lu_recherche_poste_par_competences(competences, ids_poste);

    // Filtration des résultats obtenus précédement par code postal.
    size_t ids[N_POSTES];
    memcpy(ids, ids_poste, N_POSTES * sizeof(size_t));
    memset(ids_poste, 0, N_POSTES * sizeof(size_t));
    for(size_t i = 0, j = 0; ids[i] != 0 && i != N_POSTES; ++i)
    {
        if(strcmp(en_recherche(po_recherche(ids[i])->id_entreprise)->code_postal, code_postal) == 0)
        {
            ids_poste[j++] = ids[i];
        }
    }
}

void lu_recherche_chercheur_par_competences(size_t const id_poste, size_t ids_chercheur[N_CHERCHEURS])
{
    // Liste des chercheurs qualifiés pour le poste et le nombre de compétences remplies.
    size_t chercheurs_qualifies[N_POSTES][2];
    memset(chercheurs_qualifies, 0, N_POSTES * 2 * sizeof(size_t));
    size_t q = 0;   // Nombre de chercheur qualifiés.

    poste const* const p = po_recherche(id_poste);

    // Recherche parmi les chercheurs, ceux qui sont qualifiés pour le poste.
    size_t ids[N_CHERCHEURS];
    ch_ids(ids);
    chercheur* c = ch_recherche(ids[0]);
    for(size_t i = 0; c && i != N_CHERCHEURS; c = ch_recherche(ids[++i]))
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
                    chercheurs_qualifies[q][0] = c->id;
                    ++chercheurs_qualifies[q][1];
                }
            }
        }

        if(qualifie)
        {
            ++q;
        }
    }

    // Arrangement des chercheurs qualifiés pour le poste par ordre décroissant du nombre de qualifications correspondantes.
    for(size_t ipi = 0; q--; ++ipi)
    {
        size_t max_i = 0;
        for(size_t i = 1; i != N_POSTES; ++i)
        {
            if(chercheurs_qualifies[i][1] > chercheurs_qualifies[max_i][1])
            {
                max_i = i;
            }
        }

        ids_chercheur[ipi] = chercheurs_qualifies[max_i][0];
        chercheurs_qualifies[max_i][0] = 0;
        chercheurs_qualifies[max_i][1] = 0;
    }
}

void lu_recherche_chercheur_par_competences_code_postal(size_t const id_poste, size_t ids_chercheur[N_CHERCHEURS])
{
    lu_recherche_chercheur_par_competences(id_poste, ids_chercheur);

    // Code postal de l'entreprise.
    char code_postal[L_CP];
    strcpy(code_postal, en_recherche(po_recherche(id_poste)->id_entreprise)->code_postal);

    // Filtration des résultats obtenus précédement par code postal.
    size_t ids[N_CHERCHEURS];
    memcpy(ids, ids_chercheur, N_CHERCHEURS * sizeof(size_t));
    memset(ids_chercheur, 0, N_CHERCHEURS * sizeof(size_t));
    for(size_t i = 0, j = 0; ids[i] != 0 && i != N_CHERCHEURS; ++i)
    {
        if(strcmp(ch_recherche(ids[i])->code_postal, code_postal) == 0)
        {
            ids_chercheur[j++] = ids[i];
        }
    }
}

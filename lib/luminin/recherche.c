#include "recherche.h"

#include "chercheur.h"
#include "poste.h"

#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void recherche_poste_par_competences(size_t const id_chercheur, size_t ids_poste[10])
{
    memset(ids_poste, 0, 10 * sizeof(size_t));

    chercheur const* const c = ch_recherche(id_chercheur);

    // Liste des postes pour lesquels le chercheur est qualifié et le nombre de compétences remplies.
    size_t postes_qualifies[10][2];
    memset(postes_qualifies, 0, 10 * 2 * sizeof(size_t));
    size_t q = 0;   // Nombre de postes pour lequel le chercheur se qualifie.

    // Liste des postes disponibles.
    size_t ids[10] = {0};
    po_ids(ids);

    // Recherche parmi les postes ceux pour lesquels le chercheur se qualifie et le nombre de compétences remplies.
    poste* p = po_recherche(ids[0]);
    for(size_t i = 0; p && i != 10; p = po_recherche(ids[++i]))
    {
        bool qualifie = false;

        // Pour chaque compétence du chercheur...
        for(size_t cci = 0; strlen(c->competences[cci]) && cci != 5; ++cci)
        {
            // Pour chaque compétence requise du poste...
            for(size_t pci = 0; strlen(p->competences[pci]) && pci != 5; ++pci)
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
        for(size_t i = 1; i != 10; ++i)
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

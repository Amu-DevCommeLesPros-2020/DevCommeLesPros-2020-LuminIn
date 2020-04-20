#include "luminin.h"

#include "bd.h"
#include "chercheur.h"
#include "employe.h"
#include "entreprise.h"
#include "poste.h"

void lu_init(char const* const chemin_bd)
{
    bd_init(chemin_bd);
    
    ch_init();
    em_init();
    co_init();
    po_init();
}

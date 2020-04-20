#include "journal/journal.h"
#include "luminin/luminin.h"

#include <stdlib.h>
#include <stdio.h>

char const* const chemin_journal = "./luminin.log";
char const* const chemin_bd = "./luminin";

int main()
{
    j_ouvrir(chemin_journal);
    lu_init(chemin_bd);

    printf("\
*** Bienvenu sur LuminIn, le site des pros ***\n\
  * Menu principal * \n\
\n\
Vous êtes :\n\
1. Une entreprise\n\
2. Un employé\n\
3. À la recherche d'un emploi\n\
\n\
Votre choix ('q' pour quitter) : ");

    char c;
    scanf("%c", &c);

    void j_fermer();

    return 0;
}

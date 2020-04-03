#include "journal.h"

#include <stdio.h>
#include <time.h>

FILE *journal;

// Ouvre un fichier de journal.
void j_ouvrir(char const* const chemin)
{
    journal = fopen(chemin, "w");
}

// Ajoute un évènement au journal.
void j_ecrire(char const* const ligne)
{
    char time_s[100];
    time_t now = time(0);
    strftime(time_s, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
    fprintf(journal, "[%s] %s\n", time_s, ligne);
    fflush(journal);
}

// Ferme le fichier de journal.
void j_fermer()
{
    fclose(journal);
}

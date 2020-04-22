#include "journal.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

FILE *journal;

// Ouvre un fichier de journal.
void j_ouvrir(char const* const chemin)
{
    journal = fopen(chemin, "a");
}

// Ajoute un évènement au journal.
void j_ecrire(char const* const ligne, ...)
{
    char time_s[100];
    time_t now = time(0);
    strftime(time_s, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));

    va_list args_ligne;
    va_start(args_ligne, ligne);
    va_list args_args;
    va_copy(args_args, args_ligne);
    char buffer_args_ligne[1 + vsnprintf(NULL, 0, ligne, args_ligne)];
    va_end(args_ligne);
    vsnprintf(buffer_args_ligne, sizeof(buffer_args_ligne), ligne, args_args);
    va_end(args_args);

    fprintf(journal, "[%s] %s\n", time_s, buffer_args_ligne);
    fflush(journal);
}

// Ferme le fichier de journal.
void j_fermer()
{
    fclose(journal);
}

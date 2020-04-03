#include "bd.h"

#include "journal.h"

#include <sys/stat.h>

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char const* const nom_table_compagnie = "compagnie";
char const* const nom_table_poste = "poste";
char const* const nom_table_employe = "employe";
char const* const nom_table_chercheur = "chercheur";

FILE *table_poste;
FILE *table_employe;
FILE *table_chercheur;

void bd_lecture(char const* const chemin_bd, compagnies** cs)
{
    j_ecrire("Lecture de la BD.");

    char chemin_table[PATH_MAX];

    // Lecture des tables.
    *cs = NULL;
    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_compagnie);
    FILE *table_compagnie = fopen(chemin_table, "r");
    if(table_compagnie)
    {
        *cs = malloc(sizeof(compagnies));

        char *line = NULL;
        size_t length = 0;
        getline(&line, &length, table_compagnie);
        free(line);

        compagnie c;
        while(fscanf(table_compagnie, "%zu,%127[^,],%5c,%s", &c.id, c.nom, c.code_postal, c.mail) == 4)
        {
            compagnie *data = malloc(sizeof(compagnie));
            *data = c;
            l_append(&((*cs)->tete), l_make_node(data));
        }
    }
    fclose(table_compagnie);

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_poste);
    table_poste = fopen(chemin_table, "r");

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_employe);
    table_employe = fopen(chemin_table, "r");

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_chercheur);
    table_chercheur = fopen(chemin_table, "r");
}

void bd_ecriture(char const* const chemin_bd, compagnie const* const c)
{

}

void free_compagnies(compagnies* cs)
{
    if(cs)
    {
        l_free_list(cs->tete);
    }
    free(cs);
}

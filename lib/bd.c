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

FILE *table_compagnie;
FILE *table_poste;
FILE *table_employe;
FILE *table_chercheur;

bool bd_ouvrir(char const* const chemin_bd)
{
    j_ecrire("Ouverture BD.");

    // Création du dossier.
    mkdir(chemin_bd, 0700);

    char chemin_table[PATH_MAX];

    // Création des tables.
    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_compagnie);
    table_compagnie = fopen(chemin_table, "r");

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_poste);
    table_poste = fopen(chemin_table, "r");

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_employe);
    table_employe = fopen(chemin_table, "r");

    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_chercheur);
    table_chercheur = fopen(chemin_table, "r");

    return table_compagnie && table_poste && table_employe && table_chercheur;
}

void bd_fermer()
{
    j_ecrire("Fermeture BD.");

    // Fermeture des tables.
    fclose(table_compagnie);
    fclose(table_poste);
    fclose(table_employe);
    fclose(table_chercheur);
}

compagnies* bd_lecture_compagnies()
{
    char *line = NULL;
    size_t length = 0;
    getline(&line, &length, table_compagnie);
    free(line);

    compagnies *cs = malloc(sizeof(compagnies));

    compagnie c;
    while(fscanf(table_compagnie, "%zu,%127[^,],%5c,%s", &c.id, c.nom, c.code_postal, c.mail) == 4)
    {
        compagnie *data = malloc(sizeof(compagnie));
        *data = c;
        l_append(&(cs->tete), l_make_node(data));
    }

    return cs;
}

#include "bd.h"

#include "journal.h"

#include <sys/stat.h>

#include <limits.h>
#include <stdio.h>

char const* const nom_table_compagnie = "compagnie.csv";
char const* const nom_table_poste = "poste.csv";
char const* const nom_table_employe = "employe.csv";
char const* const nom_table_chercheur = "chercheur.csv";

FILE *table_compagnie;
FILE *table_poste;
FILE *table_employe;
FILE *table_chercheur;

void bd_ouvrir(char const* const chemin_bd)
{
    j_ecrire("Ouverture BD.");

    // Création du dossier.
    mkdir(chemin_bd, 0700);

    char chemin_table[PATH_MAX];

    // Création des tables.
    sprintf(chemin_table, "%s/%s", chemin_bd, nom_table_compagnie);
    table_compagnie = fopen(chemin_table, "w");

    sprintf(chemin_table, "%s/%s", chemin_bd, nom_table_poste);
    table_poste = fopen(chemin_table, "w");

    sprintf(chemin_table, "%s/%s", chemin_bd, nom_table_employe);
    table_employe = fopen(chemin_table, "w");

    sprintf(chemin_table, "%s/%s", chemin_bd, nom_table_chercheur);
    table_chercheur = fopen(chemin_table, "w");
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

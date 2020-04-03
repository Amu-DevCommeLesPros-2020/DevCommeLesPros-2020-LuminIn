#include "bd.h"

#include "journal.h"
#include "types.h"

#include <sys/stat.h>

#include <limits.h>
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

void bd_lecture(char const* const chemin_bd, compagnies** cos)
{
    j_ecrire("Lecture de la BD.");

    // Lecture des tables.
    bd_lecture_compagnies(chemin_bd, cos);
}

void bd_lecture_compagnies(char const* const chemin_bd, compagnies** cos)
{
    j_ecrire("Lecture de la table compagnie.");

    *cos = NULL; // Valeur par défault.

    // Création du chemin complet pour la table compagnie.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_compagnie);
    
    FILE *table_compagnie = fopen(chemin_table, "r");
    if(table_compagnie)
    {
        // Ignorance de la première ligne.
        char *line = NULL;
        size_t length = 0;
        getline(&line, &length, table_compagnie);
        free(line);

        // Allocation de mémoire pour compagnies.
        *cos = malloc(sizeof(compagnies));

        // Lecture des tuples.
        compagnie co;
        while(fscanf(table_compagnie, "%zu,%127[^,],%5c,%s", &co.id, co.nom, co.code_postal, co.mail) == 4)
        {
            compagnie *data = malloc(sizeof(compagnie));
            *data = co;
            l_append(&((*cos)->tete), l_make_node(data));
        }
    }

    fclose(table_compagnie);
}

void bd_ecriture(char const* const chemin_bd, compagnies const* const cos)
{
    bd_ecriture_compagnies(chemin_bd, cos);
}

void bd_ecriture_compagnies(char const* const chemin_bd, compagnies const* const cos)
{
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd, nom_table_compagnie);
    FILE *table_compagnie = fopen(chemin_table, "w");

    fprintf(table_compagnie, "id,nom,code postal,mail\n");
    if(cos)
    {
        for(node const* n = cos->tete; n; n = n->next)
        {
            compagnie *co = (compagnie*)(n->data);
            fprintf(table_compagnie, "%zu,%s,%.5s,%s\n", co->id, co->nom, co->code_postal, co->mail);
        }
    }
    
    fclose(table_compagnie);
}

void free_compagnies(compagnies* cos)
{
    if(cos)
    {
        l_free_list(cos->tete);
    }
    free(cos);
}

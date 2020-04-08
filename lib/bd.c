#include "bd.h"

#include "journal.h"
#include "types.h"

#include <sys/stat.h>

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const* const nom_table_compagnie = "compagnie";
char const* const nom_table_poste = "poste";
char const* const nom_table_employe = "employe";
char const* const nom_table_chercheur = "chercheur";

FILE *table_poste;
FILE *table_employe;
FILE *table_chercheur;

char chemin_bd_[PATH_MAX];

void bd_init(char const* const chemin_bd)
{
    assert(chemin_bd);

    strcpy(chemin_bd_, chemin_bd);
}

void bd_lecture(compagnies** cos, postes** pos)
{
    j_ecrire("Lecture de la BD.");

    // Lecture des tables.
    bd_lecture_compagnies(cos);
    bd_lecture_postes(pos);
}

void bd_lecture_compagnies(compagnies** cos)
{
    j_ecrire("Lecture de la table compagnie.");

    *cos = NULL; // Valeur par défault.

    // Création du chemin complet pour la table compagnie.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_compagnie);
    
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

void bd_lecture_postes(postes** pos)
{
    j_ecrire("Lecture de la table poste.");

    *pos = NULL; // Valeur par défault.

    // Création du chemin complet pour la table compagnie.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_poste);
    
    FILE *table_poste = fopen(chemin_table, "r");
    if(table_poste)
    {
        // Ignorance de la première ligne.
        char *line = NULL;
        size_t length = 0;
        getline(&line, &length, table_poste);
        free(line);

        // Allocation de mémoire pour compagnies.
        *pos = malloc(sizeof(postes));

        // Lecture des tuples.
        poste po;
        char competences[128 * 5];
        while(fscanf(table_poste, "%zu,%127[^,],%127[^,],%zu", &po.id, po.titre, competences, &po.id_compagnie) == 4)
        {
            poste *data = malloc(sizeof(poste));
            *data = po;
            
            char *competence = strtok(competences, ";");
            for(int i = 0; i != 5; ++i)
            {
                strcpy(data->competences[i], competence ? competence : "");
                competence = strtok(NULL, ";");
            }

            l_append(&((*pos)->tete), l_make_node(data));
        }
    }

    fclose(table_poste);
}

void bd_lecture_employes(employes** ems)
{
    j_ecrire("Lecture de la table employe.");

    *ems = NULL; // Valeur par défault.

    // Création du chemin complet pour la table employe.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_employe);
    
    FILE *table_employe = fopen(chemin_table, "r");
    if(table_employe)
    {
        // Ignorance de la première ligne.
        char *line = NULL;
        size_t length = 0;
        getline(&line, &length, table_employe);
        free(line);

        // Allocation de mémoire pour compagnies.
        *ems = malloc(sizeof(employes));

        // Lecture des tuples.
        employe em;
        memset(&em, 0, sizeof(employe));
        char competences[128 * 5];
        char id_collegues[3 * 5];
        // J'ai mis le champ id_collegues à la toute fin parce que lorsqu'il est vide, il ne peut être capturé par [^,] car ce champ de capture *doit* capturer au moins un caractère.
        while(fscanf(table_employe, "%zu,%127[^,],%127[^,],%127[^,],%5c,%639[^,],%zu,%14[^\n]", &em.id, em.nom, em.prenom, em.mail, em.code_postal, competences, &em.id_entreprise, id_collegues) >= 7)
        {
            employe *data = malloc(sizeof(employe));
            *data = em;
            
            char *competence = strtok(competences, ";");
            for(int i = 0; i != 5; ++i)
            {
                strcpy(data->competences[i], competence ? competence : "");
                competence = strtok(NULL, ";");
            }

            char *id_collegue = strtok(id_collegues, ";");
            for(int i = 0; i != 5; ++i)
            {
                data->id_collegues[i] = id_collegue ? atoi(id_collegue) : 0;
                id_collegue = strtok(NULL, ";");
            }

            l_append(&((*ems)->tete), l_make_node(data));
        }
    }

    fclose(table_employe);
}

void bd_ecriture(compagnies const* const cos)
{
    j_ecrire("Écriture de la BD.");

    bd_ecriture_compagnies(cos);
}

void bd_ecriture_compagnies(compagnies const* const cos)
{
    j_ecrire("Écriture de la table compagnie.");

    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_compagnie);
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

void bd_ecriture_postes(postes const* const pos)
{
    j_ecrire("Écriture de la table poste.");

    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_poste);
    FILE *table_poste = fopen(chemin_table, "w");

    fprintf(table_poste, "id,titre,competences,entreprise\n");
    if(pos)
    {
        for(node const* n = pos->tete; n; n = n->next)
        {
            poste *po = (poste*)(n->data);
            fprintf(table_poste, "%zu,%s,", po->id, po->titre);
            for(int i = 0; i != 5 && strlen(po->competences[i]) != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_poste, ";");
                }
                fprintf(table_poste, "%s", po->competences[i]);
            }
            fprintf(table_poste, ",%zu\n", po->id_compagnie);
        }
    }
    
    fclose(table_poste);
}

void bd_ecriture_employes(employes const* const ems)
{
    j_ecrire("Écriture de la table employe.");

    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_employe);
    FILE *table_employe = fopen(chemin_table, "w");

    fprintf(table_employe, "id,nom,prenom,mail,code postal,competences,entreprise,collegues\n");
    if(ems)
    {
        for(node const* n = ems->tete; n; n = n->next)
        {
            employe *em = (employe*)(n->data);
            fprintf(table_employe, "%zu,%s,%s,%s,%.5s,", em->id, em->nom, em->prenom, em->mail, em->code_postal);
            for(int i = 0; i != 5 && strlen(em->competences[i]) != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_employe, ";");
                }
                fprintf(table_employe, "%s", em->competences[i]);
            }
            fprintf(table_employe, ",%zu,", em->id_entreprise);
            for(int i = 0; i != 5 && em->id_collegues[i] != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_employe, ";");
                }
                fprintf(table_employe, "%zu", em->id_collegues[i]);
            }
            fprintf(table_employe, "\n");
        }
    }
    
    fclose(table_employe);
}

void free_compagnies(compagnies* cos)
{
    if(cos)
    {
        l_free_list(cos->tete);
    }
    free(cos);
}

void free_postes(postes* pos)
{
    if(pos)
    {
        l_free_list(pos->tete);
    }
    free(pos);
}

void free_employes(employes* ems)
{
    if(ems)
    {
        l_free_list(ems->tete);
    }
    free(ems);
}

#include "bd.h"

#include "chercheur.h"
#include "constantes.h"
#include "employe.h"
#include "entreprise.h"
#include "poste.h"

#include "journal/journal.h"
#include "liste/liste.h"

#include <sys/stat.h>

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const* const nom_table_chercheur = "chercheur";
char const* const nom_table_employe = "employe";
char const* const nom_table_entreprise = "entreprise";
char const* const nom_table_poste = "poste";

char chemin_bd_[PATH_MAX - NAME_MAX];

void bd_init(char const* const chemin_bd)
{
    assert(chemin_bd);

    strcpy(chemin_bd_, chemin_bd);
    mkdir(chemin_bd_, 0700);
}

void bd_lecture_entreprises(entreprises** ens)
{
    j_ecrire("Lecture de la table entreprise.");

    // Allocation de mémoire pour entreprises.
    *ens = calloc(1, sizeof(entreprises));

    // Création du chemin complet pour la table entreprise.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_entreprise);
    
    FILE *table_entreprise = fopen(chemin_table, "r");
    if(table_entreprise)
    {
        // Ignorance de la première ligne.
        fscanf(table_entreprise, "%*[^\n]\n");

        // Lecture des tuples.
        entreprise co;
        memset(&co, 0, sizeof(co));
        while(fscanf(table_entreprise, "%zu,%[^,],%[^,],%s", &co.id, co.nom, co.code_postal, co.mail) == 4)
        {
            entreprise *data = calloc(1, sizeof(entreprise));
            *data = co;
            l_append(&((*ens)->tete), l_make_node(data));
        }

        fclose(table_entreprise);
    }
}

void bd_lecture_postes(postes** pos)
{
    j_ecrire("Lecture de la table poste.");

    // Allocation de mémoire pour postes.
    *pos = calloc(1, sizeof(postes));

    // Création du chemin complet pour la table poste.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_poste);
    
    FILE *table_poste = fopen(chemin_table, "r");
    if(table_poste)
    {
        // Ignorance de la première ligne.
        fscanf(table_poste, "%*[^\n]\n");

        // Lecture des tuples.
        poste po;
        memset(&po, 0, sizeof(po));
        char competences[NL_COMPETENCES] = {'\0'};
        while(fscanf(table_poste, "%zu,%[^,],%[^,],%zu", &po.id, po.titre, competences, &po.id_entreprise) == 4)
        {
            poste *data = calloc(1, sizeof(poste));
            *data = po;
            
            char *competence = strtok(competences, ";");
            for(int i = 0; competence && i != N_COMPETENCES; ++i)
            {
                strcpy(data->competences[i], competence ? competence : "");
                competence = strtok(NULL, ";");
            }

            l_append(&((*pos)->tete), l_make_node(data));
        }

        fclose(table_poste);
    }
}

void bd_lecture_employes(employes** ems)
{
    j_ecrire("Lecture de la table employe.");

    // Allocation de mémoire pour employes.
    *ems = calloc(1, sizeof(employes));

    // Création du chemin complet pour la table employe.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_employe);
    
    FILE *table_employe = fopen(chemin_table, "r");
    if(table_employe)
    {
        // Ignorance de la première ligne.
        fscanf(table_employe, "%*[^\n]\n");

        // Lecture des tuples.
        employe em;
        memset(&em, 0, sizeof(employe));
        char competences[NL_COMPETENCES] = {'\0'};
        char id_collegues[5 * N_COLLEGUES] = {'\0'};
        // J'ai mis le champ id_collegues à la toute fin parce que lorsqu'il est vide, il ne peut être capturé par [^,] car ce champ de capture *doit* capturer au moins un caractère.
        while(fscanf(table_employe, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%zu,%250[^\n]", &em.id, em.nom, em.prenom, em.mail, em.code_postal, competences, &em.id_entreprise, id_collegues) >= 7)
        {
            employe *data = calloc(1, sizeof(employe));
            *data = em;
            
            char *competence = strtok(competences, ";");
            for(int i = 0; competence && i != N_COMPETENCES; ++i)
            {
                strcpy(data->competences[i], competence ? competence : "");
                competence = strtok(NULL, ";");
            }

            char *id_collegue = strtok(id_collegues, ";");
            for(int i = 0; id_collegue && i != N_COLLEGUES; ++i)
            {
                data->id_collegues[i] = id_collegue ? atoi(id_collegue) : 0;
                id_collegue = strtok(NULL, ";");
            }
            id_collegues[0] = '\0';

            l_append(&((*ems)->tete), l_make_node(data));
        }

        fclose(table_employe);
    }
}

void bd_lecture_chercheurs(chercheurs** chs)
{
    j_ecrire("Lecture de la table chercheur.");

    // Allocation de mémoire pour chercheurs.
    *chs = calloc(1, sizeof(chercheurs));

    // Création du chemin complet pour la table chercheur.csv.
    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_chercheur);
    
    FILE *table_chercheur = fopen(chemin_table, "r");
    if(table_chercheur)
    {
        // Ignorance de la première ligne.
        fscanf(table_chercheur, "%*[^\n]\n");

        // Lecture des tuples.
        chercheur ch;
        memset(&ch, 0, sizeof(chercheur));
        char competences[NL_COMPETENCES] = {'\0'};
        char id_collegues[5 * N_COLLEGUES] = {0};
        // J'ai mis le champ id_collegues à la toute fin parce que lorsqu'il est vide, il ne peut être capturé par [^,] car ce champ de capture *doit* capturer au moins un caractère.
        while(fscanf(table_chercheur, "%zu,%[^,],%[^,],%[^,],%[^,],%[^,],%250[^\n]", &ch.id, ch.nom, ch.prenom, ch.mail, ch.code_postal, competences, id_collegues) >= 6)
        {
            chercheur *data = calloc(1, sizeof(chercheur));
            *data = ch;
            
            char *competence = strtok(competences, ";");
            for(int i = 0; competence && i != N_COMPETENCES; ++i)
            {
                strcpy(data->competences[i], competence ? competence : "");
                competence = strtok(NULL, ";");
            }

            char *id_collegue = strtok(id_collegues, ";");
            for(int i = 0; id_collegue && i != N_COLLEGUES; ++i)
            {
                data->id_collegues[i] = id_collegue ? atoi(id_collegue) : 0;
                id_collegue = strtok(NULL, ";");
            }
            id_collegues[0] = '\0';

            l_append(&((*chs)->tete), l_make_node(data));
        }

        fclose(table_chercheur);
    }
}

void bd_ecriture(chercheurs const* const chs, employes const* const ems, entreprises const* const ens, postes const* const pos)
{
    j_ecrire("Écriture de la BD.");

    bd_ecriture_chercheurs(chs);
    bd_ecriture_employes(ems);
    bd_ecriture_entreprises(ens);
    bd_ecriture_postes(pos);
}

void bd_ecriture_entreprises(entreprises const* const ens)
{
    j_ecrire("Écriture de la table entreprise.");

    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_entreprise);
    FILE *table_entreprise = fopen(chemin_table, "w");

    fprintf(table_entreprise, "id,nom,code postal,mail\n");
    if(ens)
    {
        for(node const* n = ens->tete; n; n = n->next)
        {
            entreprise *co = (entreprise*)(n->data);
            fprintf(table_entreprise, "%zu,%s,%s,%s\n", co->id, co->nom, co->code_postal, co->mail);
        }
    }
    
    fclose(table_entreprise);
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
            for(int i = 0; i != N_COMPETENCES && strlen(po->competences[i]) != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_poste, ";");
                }
                fprintf(table_poste, "%s", po->competences[i]);
            }
            fprintf(table_poste, ",%zu\n", po->id_entreprise);
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
            fprintf(table_employe, "%zu,%s,%s,%s,%s,", em->id, em->nom, em->prenom, em->mail, em->code_postal);
            for(int i = 0; i != N_COMPETENCES && strlen(em->competences[i]) != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_employe, ";");
                }
                fprintf(table_employe, "%s", em->competences[i]);
            }
            fprintf(table_employe, ",%zu,", em->id_entreprise);
            for(int i = 0; i != N_COLLEGUES && em->id_collegues[i] != 0; ++i)
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

void bd_ecriture_chercheurs(chercheurs const* const chs)
{
    j_ecrire("Écriture de la table chercheur.");

    char chemin_table[PATH_MAX];
    sprintf(chemin_table, "%s/%s.csv", chemin_bd_, nom_table_chercheur);
    FILE *table_chercheur = fopen(chemin_table, "w");

    fprintf(table_chercheur, "id,nom,prenom,mail,code postal,competences,collegues\n");
    if(chs)
    {
        for(node const* n = chs->tete; n; n = n->next)
        {
            chercheur *ch = (chercheur*)(n->data);
            fprintf(table_chercheur, "%zu,%s,%s,%s,%s,", ch->id, ch->nom, ch->prenom, ch->mail, ch->code_postal);
            for(int i = 0; i != N_COMPETENCES && strlen(ch->competences[i]) != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_chercheur, ";");
                }
                fprintf(table_chercheur, "%s", ch->competences[i]);
            }
            fprintf(table_chercheur, ",");
            for(int i = 0; i != N_COLLEGUES && ch->id_collegues[i] != 0; ++i)
            {
                if(i != 0)
                {
                    fprintf(table_chercheur, ";");
                }
                fprintf(table_chercheur, "%zu", ch->id_collegues[i]);
            }
            fprintf(table_chercheur, "\n");
        }
    }
    
    fclose(table_chercheur);
}

void free_entreprises(entreprises* ens)
{
    if(ens)
    {
        l_free_list(ens->tete);
    }
    free(ens);
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

void free_chercheurs(chercheurs* chs)
{
    if(chs)
    {
        l_free_list(chs->tete);
    }
    free(chs);
}

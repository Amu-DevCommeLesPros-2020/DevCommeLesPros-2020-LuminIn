#include "bd.h"
#include "journal.h"

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Nombre total de tests exécutés. 
int tests_executes = 0;
// Pour chaque test qui réussi, cette variable sera incrémentée de 1.
// Le but est de la garder égale à test_executes.
int tests_reussis = 0;

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

// Incrémente le nombre de test exécutés de 1.
// Si le test réussi, incrémente le nombre de tests réussis de 1.
#define TEST(x) tests_executes += 1;    \
                if(x)                   \
                {                       \
                    tests_reussis += 1; \
                    printf("[SUCCES] ");\
                }                       \
                else                    \
                {                       \
                    printf("[ECHEC ] ");\
                }                       \
                printf(STRINGIZE(__FILE__) ", " STRINGIZE(__LINE__) ": " STRINGIZE(x) "\n");

int main()
{
    // Tests pour la bilbiothèques de journal.
    {
        char const* const nom_journal = "./luminin-test.log";

        remove(nom_journal);
        TEST(access(nom_journal, F_OK) == -1);
        
        j_ouvrir(nom_journal);
        TEST(access(nom_journal, F_OK) == 0);

        j_ecrire("test 1");
        struct stat s;
        stat(nom_journal, &s);
        TEST(s.st_size > 0);

        j_fermer();
        TEST(access(nom_journal, F_OK) == 0);
    }

    // Tests pour la lecture d'une BD non-existante.
    {
#define chemin_nonexistant_bd "./nonexistant"

        compagnies *cs;

        bd_lecture(chemin_nonexistant_bd, &cs);
        TEST(cs == NULL);
    }

    // Tests pour la lecture d'une BD existante.
    {
#define chemin_test_bd "../test"

        compagnies *cs;

        // Lecture d'une liste de compagnies d'une BD dont les valeurs sont connues.
        bd_lecture_compagnies(chemin_test_bd, &cs);
        TEST(cs != NULL);

        compagnie *disney = (compagnie*)(cs->tete->data);
        TEST(disney->id == 1);
        TEST(strcmp(disney->nom, "Disney") == 0);
        TEST(strncmp(disney->code_postal, "77700", 5) == 0);
        TEST(strcmp(disney->mail, "walt@disney.com") == 0);

        compagnie *google = (compagnie*)(l_skip(cs->tete, 1)->data);
        TEST(google->id == 2);
        TEST(strcmp(google->nom, "Google") == 0);
        TEST(strncmp(google->code_postal, "75009", 5) == 0);
        TEST(strcmp(google->mail, "emplois@google.com") == 0);

        free_compagnies(cs);
    }

    // Test pour écriture d'une structure compagnies dans la BD.
    {
#define chemin_test_ecriture_bd "./bd-test-ecriture"
        mkdir(chemin_test_ecriture_bd, 0700);

        // Creation d'une liste de compagnies fictive (avec une seule compagnie).
        compagnies *cs = malloc(sizeof(compagnies));
        cs->tete = NULL;

        compagnie *c = malloc(sizeof(compagnie));
        c->id = 100;
        strcpy(c->nom, "Fictive");
        strncpy(c->code_postal, "99000", 5);
        strcpy(c->mail, "nobody@bowhere.com");
        l_append(&(cs->tete), l_make_node(c));

        // Écriture de ces compagnies dans la BD.
        bd_ecriture_compagnies(chemin_test_ecriture_bd, cs);
        free_compagnies(cs);

        // Lecture des compagnies de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_compagnies(chemin_test_ecriture_bd, &cs);
        c = (compagnie*)(cs->tete->data);
        TEST(c->id == 100);
        TEST(strcmp(c->nom, "Fictive") == 0);
        TEST(strncmp(c->code_postal, "99000", 5) == 0);
        TEST(strcmp(c->mail, "nobody@bowhere.com") == 0);

        free_compagnies(cs);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

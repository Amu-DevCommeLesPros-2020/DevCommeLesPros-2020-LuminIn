#include "bd.h"
#include "compagnie.h"
#include "journal.h"
#include "types.h"

#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        j_ecrire("test");
        j_fermer();
        TEST(access(nom_journal, F_OK) == 0);

        struct stat s1;
        stat(nom_journal, &s1);
        TEST(s1.st_size > 0);

        j_ouvrir(nom_journal);
        j_ecrire("test");
        j_fermer();
        struct stat s2;
        stat(nom_journal, &s2);
        TEST(s2.st_size > s1.st_size);
    }

    // Tests pour la lecture d'une BD non-existante.
    {
#define chemin_nonexistant_bd "./nonexistant"

        compagnies *cos;

        bd_lecture(chemin_nonexistant_bd, &cos);
        TEST(cos == NULL);
    }

    // Tests pour la lecture d'une BD existante.
    {
#define chemin_test_bd "../test"

        compagnies *cos;

        // Lecture d'une liste de compagnies d'une BD dont les valeurs sont connues.
        bd_lecture_compagnies(chemin_test_bd, &cos);
        TEST(cos != NULL);

        compagnie *disney = (compagnie*)(cos->tete->data);
        TEST(disney->id == 1);
        TEST(strcmp(disney->nom, "Disney") == 0);
        TEST(strncmp(disney->code_postal, "77700", 5) == 0);
        TEST(strcmp(disney->mail, "walt@disney.com") == 0);

        compagnie *google = (compagnie*)(l_skip(cos->tete, 1)->data);
        TEST(google->id == 2);
        TEST(strcmp(google->nom, "Google") == 0);
        TEST(strncmp(google->code_postal, "75009", 5) == 0);
        TEST(strcmp(google->mail, "emplois@google.com") == 0);

        free_compagnies(cos);
    }

    // Test pour écriture d'une structure compagnies dans la BD.
    {
#define chemin_test_ecriture_bd "./bd-test-ecriture"
        mkdir(chemin_test_ecriture_bd, 0700);

        // Creation d'une liste de compagnies fictive (avec une seule compagnie).
        compagnies *cos = malloc(sizeof(compagnies));
        cos->tete = NULL;

        compagnie *co = malloc(sizeof(compagnie));
        co->id = 100;
        strcpy(co->nom, "Fictive");
        strncpy(co->code_postal, "99000", 5);
        strcpy(co->mail, "nobody@bowhere.com");
        l_append(&(cos->tete), l_make_node(co));

        // Écriture de ces compagnies dans la BD.
        bd_ecriture_compagnies(chemin_test_ecriture_bd, cos);
        free_compagnies(cos);

        // Lecture des compagnies de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_compagnies(chemin_test_ecriture_bd, &cos);
        co = (compagnie*)(cos->tete->data);
        TEST(co->id == 100);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strncmp(co->code_postal, "99000", 5) == 0);
        TEST(strcmp(co->mail, "nobody@bowhere.com") == 0);

        free_compagnies(cos);
    }

    // Test pour créer un profil de compagnie.
    {
        compagnies *cos = malloc(sizeof(compagnies));
        cos->tete = NULL;

        size_t const id = co_creer_profil(cos, "Fictive", "99000", "nobody@nowhere.com");
        TEST(id == 1);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

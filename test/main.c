#include "bd.h"
#include "journal.h"

#include <stdio.h>
#include <sys/stat.h>
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

    // Tests pour la création d'une BD.
    {
#define chemin_db "./db-creation-test"
#define nom_table_compagnie "compagnie.csv"
#define nom_table_poste "poste.csv"
#define nom_table_employe "employe.csv"
#define nom_table_chercheur "chercheur.csv"
        
        remove(chemin_db);

        bd_ouvrir(chemin_db);
        TEST(access(chemin_db "/" nom_table_compagnie, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_poste, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_employe, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_chercheur, F_OK) == 0);

        bd_fermer();
        TEST(access(chemin_db "/" nom_table_compagnie, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_poste, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_employe, F_OK) == 0);
        TEST(access(chemin_db "/" nom_table_chercheur, F_OK) == 0);

        remove(chemin_db);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

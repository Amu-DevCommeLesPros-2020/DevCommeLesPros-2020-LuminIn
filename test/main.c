#include "bd.h"
#include "compagnie.h"
#include "journal.h"
#include "types.h"
#include "poste.h"

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
                printf(__FILE__ ":" STRINGIZE(__LINE__) ":0"  " : " STRINGIZE(x) "\n");

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

        bd_init(chemin_nonexistant_bd);

        compagnies *cos;
        postes *pos;
        bd_lecture(&cos, &pos);

        TEST(cos == NULL);
    }

    // Tests pour la lecture d'une BD existante.
    {
#define chemin_test_bd "../test"

        bd_init(chemin_test_bd);

        // Lecture d'une liste de compagnies d'une BD dont les valeurs sont connues.
        compagnies *cos;
        bd_lecture_compagnies(&cos);

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

        // Lecture d'une liste de postes d'une BD dont les valeurs sont connues.
        postes *pos;
        bd_lecture_postes(&pos);

        TEST(pos != NULL);
        poste *acteur = (poste*)(pos->tete->data);
        TEST(acteur->id == 1);
        TEST(strcmp(acteur->titre, "acteur") == 0);
        TEST(strcmp(acteur->competences[0], "comedie") == 0);
        TEST(strcmp(acteur->competences[1], "gag") == 0);
        TEST(strcmp(acteur->competences[2], "") == 0);
        TEST(strcmp(acteur->competences[3], "") == 0);
        TEST(strcmp(acteur->competences[4], "") == 0);
        TEST(acteur->id_compagnie == 1);

        poste *developpeur = (poste*)(l_skip(pos->tete, 1)->data);
        TEST(developpeur->id == 2);
        TEST(strcmp(developpeur->titre, "developpeur") == 0);
        TEST(strcmp(developpeur->competences[0], "C") == 0);
        TEST(strcmp(developpeur->competences[1], "SQL") == 0);
        TEST(strcmp(developpeur->competences[2], "Python") == 0);
        TEST(strcmp(developpeur->competences[3], "") == 0);
        TEST(strcmp(developpeur->competences[4], "") == 0);
        TEST(developpeur->id_compagnie == 2);
    }

    // Test pour écriture d'une structure compagnies dans la BD.
    {
#define chemin_test_ecriture_bd "./bd-test-ecriture"
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

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
        bd_ecriture_compagnies(cos);
        free_compagnies(cos);

        // Lecture des compagnies de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_compagnies(&cos);
        co = (compagnie*)(cos->tete->data);
        TEST(co->id == 100);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strncmp(co->code_postal, "99000", 5) == 0);
        TEST(strcmp(co->mail, "nobody@bowhere.com") == 0);

        free_compagnies(cos);
    }

    // Test pour créer un profil de compagnie.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        TEST(id != 0);
    }

    // Test pour rechercher un profil de compagnie.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        compagnie *co = co_recherche(id);

        TEST(co != NULL);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strncmp(co->code_postal, "99000", 5) == 0);
        TEST(strcmp(co->mail, "nobody@nowhere.com") == 0);
    }

    // Test pour supprimer un profil de compagnie.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        co_supprimer_profil(id);
        
        TEST(co_recherche(id) == NULL);
    }

    // Test pour modifier un profil de compagnie.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        co_modifier_profil(id, "Reelle", "13010", "president@luminy.com");

        compagnie *co = co_recherche(id);
        TEST(co->id == id);
        TEST(strcmp(co->nom, "Reelle") == 0);
        TEST(strncmp(co->code_postal, "13010", 5) == 0);
        TEST(strcmp(co->mail, "president@luminy.com") == 0);
    }

    // Test pour écriture d'une structure postes dans la BD.
    {
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste de postes fictifs.
        postes *pos = malloc(sizeof(postes));
        pos->tete = NULL;

        poste *po = malloc(sizeof(poste));
        po->id = 100;
        strcpy(po->titre, "Professeur");
        strcpy(po->competences[0], "experience");
        strcpy(po->competences[1], "bagou");
        strcpy(po->competences[2], "patience");
        strcpy(po->competences[3], "");
        strcpy(po->competences[4], "");
        po->id_compagnie = 1;
        l_append(&(pos->tete), l_make_node(po));

        // Écriture de ces compagnies dans la BD.
        bd_ecriture_postes(pos);
        free_postes(pos);

        // Lecture des postes de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_postes(&pos);
        po = (poste*)(pos->tete->data);
        TEST(po->id == 100);
        TEST(strcmp(po->titre, "Professeur") == 0);
        TEST(strcmp(po->competences[0], "experience") == 0);
        TEST(strcmp(po->competences[1], "bagou") == 0);
        TEST(strcmp(po->competences[2], "patience") == 0);
        TEST(strcmp(po->competences[3], "") == 0);
        TEST(strcmp(po->competences[4], "") == 0);
        TEST(po->id_compagnie == 1);

        free_postes(pos);
    }

    // Test pour créer un profil de poste.
    {
        po_init();

        char const competences[5][128] = {"experience", "bagou", "patience", "", ""};
        size_t const id = po_creer_poste("Professeur", competences, 1);
        TEST(id != 0);
    }

    // Test pour supprimer un poste.
    {
        po_init();

        char const competences[5][128] = {"experience", "bagou", "patience", "", ""};
        size_t const id = po_creer_poste("Professeur", competences, 1);
        po_supprimer_poste(id);
        
        TEST(po_recherche(id) == NULL);
    }

    // Test pour rechercher un poste.
    {
        po_init();

        char const competences[5][128] = {"experience", "bagou", "patience", "", ""};
        size_t const id = po_creer_poste("Professeur", competences, 1);
        poste *po = po_recherche(id);

        TEST(po != NULL);
        TEST(strcmp(po->titre, "Professeur") == 0);
        TEST(strcmp(po->competences[0], "experience") == 0);
        TEST(strcmp(po->competences[1], "bagou") == 0);
        TEST(strcmp(po->competences[2], "patience") == 0);
        TEST(strcmp(po->competences[3], "") == 0);
        TEST(strcmp(po->competences[4], "") == 0);
        TEST(po->id_compagnie == 1);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

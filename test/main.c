#include "journal/journal.h"
#include "liste/liste.h"
#include "luminin/bd.h"
#include "luminin/chercheur.h"
#include "luminin/employe.h"
#include "luminin/entreprise.h"
#include "luminin/poste.h"

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
        char const* const nom_journal = "./lumininnin-test.log";

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

        entreprises *cos;
        postes *pos;
        bd_lecture(&cos, &pos);

        TEST(cos == NULL);
    }

    // Tests pour la lecture d'une BD existante.
    {
#define chemin_test_bd "../test"

        bd_init(chemin_test_bd);

        // Lecture d'une liste d'entreprises d'une BD dont les valeurs sont connues.
        entreprises *cos;
        bd_lecture_entreprises(&cos);

        TEST(cos != NULL);

        entreprise *disney = (entreprise*)(cos->tete->data);
        TEST(disney->id == 1);
        TEST(strcmp(disney->nom, "Disney") == 0);
        TEST(strncmp(disney->code_postal, "77700", 5) == 0);
        TEST(strcmp(disney->mail, "walt@disney.com") == 0);

        entreprise *google = (entreprise*)(l_skip(cos->tete, 1)->data);
        TEST(google->id == 2);
        TEST(strcmp(google->nom, "Google") == 0);
        TEST(strncmp(google->code_postal, "75009", 5) == 0);
        TEST(strcmp(google->mail, "emplois@google.com") == 0);

        free_entreprises(cos);

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
        TEST(acteur->id_entreprise == 1);

        poste *developpeur = (poste*)(l_skip(pos->tete, 1)->data);
        TEST(developpeur->id == 2);
        TEST(strcmp(developpeur->titre, "developpeur") == 0);
        TEST(strcmp(developpeur->competences[0], "C") == 0);
        TEST(strcmp(developpeur->competences[1], "SQL") == 0);
        TEST(strcmp(developpeur->competences[2], "Python") == 0);
        TEST(strcmp(developpeur->competences[3], "") == 0);
        TEST(strcmp(developpeur->competences[4], "") == 0);
        TEST(developpeur->id_entreprise == 2);

        // Lecture d'une liste d'employés d'une BD dont les valeurs sont connues.
        employes *ems;
        bd_lecture_employes(&ems);

        TEST(ems != NULL);
        employe *em = (employe*)(ems->tete->data);
        TEST(em->id == 1);
        TEST(strcmp(em->nom, "Untel") == 0);
        TEST(strcmp(em->prenom, "Michel") == 0);
        TEST(strcmp(em->mail, "m_untel@google.com") == 0);
        TEST(strncmp(em->code_postal, "13010", 5) == 0);
        TEST(strcmp(em->competences[0], "C++") == 0);
        TEST(strcmp(em->competences[1], "Python") == 0);
        TEST(strcmp(em->competences[2], "") == 0);
        TEST(strcmp(em->competences[3], "") == 0);
        TEST(strcmp(em->competences[4], "") == 0);
        TEST(em->id_entreprise == 2);
        TEST(em->id_collegues[0] == 0);
        TEST(em->id_collegues[1] == 0);
        TEST(em->id_collegues[2] == 0);
        TEST(em->id_collegues[3] == 0);
        TEST(em->id_collegues[4] == 0);

        em = (employe*)(l_skip(ems->tete, 1)->data);
        TEST(em->id == 2);
        TEST(strcmp(em->nom, "Mouse") == 0);
        TEST(strcmp(em->prenom, "Mickey") == 0);
        TEST(strcmp(em->mail, "mickey@mickeyville.gov") == 0);
        TEST(strncmp(em->code_postal, "77700", 5) == 0);
        TEST(strcmp(em->competences[0], "comedie") == 0);
        TEST(strcmp(em->competences[1], "") == 0);
        TEST(strcmp(em->competences[2], "") == 0);
        TEST(strcmp(em->competences[3], "") == 0);
        TEST(strcmp(em->competences[4], "") == 0);
        TEST(em->id_entreprise == 1);
        TEST(em->id_collegues[0] == 3);
        TEST(em->id_collegues[1] == 0);
        TEST(em->id_collegues[2] == 0);
        TEST(em->id_collegues[3] == 0);
        TEST(em->id_collegues[4] == 0);

        // Lecture d'une liste de chercheurs d'une BD dont les valeurs sont connues.
        chercheurs *chs;
        bd_lecture_chercheurs(&chs);

        TEST(chs != NULL);
        chercheur *ch = (chercheur*)(chs->tete->data);
        TEST(ch->id == 1);
        TEST(strcmp(ch->nom, "Duck") == 0);
        TEST(strcmp(ch->prenom, "Donald") == 0);
        TEST(strcmp(ch->mail, "donal.duck@canardville.gov") == 0);
        TEST(strncmp(ch->code_postal, "77700", 5) == 0);
        TEST(strcmp(ch->competences[0], "comedie") == 0);
        TEST(strcmp(ch->competences[1], "gag") == 0);
        TEST(strcmp(ch->competences[2], "") == 0);
        TEST(strcmp(ch->competences[3], "") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);
        TEST(ch->id_collegues[0] == 2);
        TEST(ch->id_collegues[1] == 0);
        TEST(ch->id_collegues[2] == 0);
        TEST(ch->id_collegues[3] == 0);
        TEST(ch->id_collegues[4] == 0);

        ch = (chercheur*)(l_skip(chs->tete, 1)->data);
        TEST(ch->id == 2);
        TEST(strcmp(ch->nom, "Pignon") == 0);
        TEST(strcmp(ch->prenom, "Francois") == 0);
        TEST(strcmp(ch->mail, "pignouf@gmail.com") == 0);
        TEST(strncmp(ch->code_postal, "75020", 5) == 0);
        TEST(strcmp(ch->competences[0], "C") == 0);
        TEST(strcmp(ch->competences[1], "SQL") == 0);
        TEST(strcmp(ch->competences[2], "Python") == 0);
        TEST(strcmp(ch->competences[3], "") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);
        TEST(ch->id_collegues[0] == 0);
        TEST(ch->id_collegues[1] == 0);
        TEST(ch->id_collegues[2] == 0);
        TEST(ch->id_collegues[3] == 0);
        TEST(ch->id_collegues[4] == 0);
    }

    // Test pour écriture d'une structure entreprises dans la BD.
    {
#define chemin_test_ecriture_bd "./bd-test-ecriture"
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste d'entreprises fictives (avec une seule entreprise).
        entreprises *cos = malloc(sizeof(entreprises));
        cos->tete = NULL;

        entreprise *co = malloc(sizeof(entreprise));
        co->id = 100;
        strcpy(co->nom, "Fictive");
        strncpy(co->code_postal, "99000", 5);
        strcpy(co->mail, "nobody@bowhere.com");
        l_append(&(cos->tete), l_make_node(co));

        // Écriture de ces entreprises dans la BD.
        bd_ecriture_entreprises(cos);
        free_entreprises(cos);

        // Lecture des entreprises de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_entreprises(&cos);
        co = (entreprise*)(cos->tete->data);
        TEST(co->id == 100);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strncmp(co->code_postal, "99000", 5) == 0);
        TEST(strcmp(co->mail, "nobody@bowhere.com") == 0);

        free_entreprises(cos);
    }

    // Test pour créer un profil d'entreprise.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        TEST(id != 0);
    }

    // Test pour rechercher un profil d'entreprise.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        entreprise *co = co_recherche(id);

        TEST(co != NULL);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strncmp(co->code_postal, "99000", 5) == 0);
        TEST(strcmp(co->mail, "nobody@nowhere.com") == 0);
    }

    // Test pour supprimer un profil d'entreprise.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        co_supprimer_profil(id);
        
        TEST(co_recherche(id) == NULL);
    }

    // Test pour modifier un profil d'entreprise.
    {
        co_init();

        size_t const id = co_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        co_modifier_profil(id, "Reelle", "13010", "president@lumininny.com");

        entreprise *co = co_recherche(id);
        TEST(co->id == id);
        TEST(strcmp(co->nom, "Reelle") == 0);
        TEST(strncmp(co->code_postal, "13010", 5) == 0);
        TEST(strcmp(co->mail, "president@lumininny.com") == 0);
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
        po->id_entreprise = 1;
        l_append(&(pos->tete), l_make_node(po));

        // Écriture de ces postes dans la BD.
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
        TEST(po->id_entreprise == 1);

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
        TEST(po->id_entreprise == 1);
    }

    // Test pour écriture d'une structure employes dans la BD.
    {
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste dd'employes fictifs.
        employes *ems = malloc(sizeof(employes));
        ems->tete = NULL;

        employe *em = malloc(sizeof(employe));
        em->id = 100;
        strcpy(em->nom, "Gladu");
        strcpy(em->prenom, "Gaston");
        strcpy(em->mail, "gg@gladu.name");
        strncpy(em->code_postal, "00000", 5);
        strcpy(em->competences[0], "ennuyeux");
        strcpy(em->competences[1], "maladroit");
        strcpy(em->competences[2], "empote");
        strcpy(em->competences[3], "pompeur d'air");
        strcpy(em->competences[4], "emmerdeur");
        em->id_entreprise = 100;
        em->id_collegues[0] = 66;
        em->id_collegues[1] = 67;
        em->id_collegues[2] = 68;
        em->id_collegues[3] = 69;
        em->id_collegues[4] = 70;
        l_append(&(ems->tete), l_make_node(em));

        // Écriture de ces employes dans la BD.
        bd_ecriture_employes(ems);
        free_employes(ems);

        // Lecture des employes de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_employes(&ems);
        em = (employe*)(ems->tete->data);
        TEST(em->id == 100);
        TEST(strcmp(em->nom, "Gladu") == 0);
        TEST(strcmp(em->prenom, "Gaston") == 0);
        TEST(strcmp(em->mail, "gg@gladu.name") == 0);
        TEST(strncmp(em->code_postal, "00000", 5) == 0);
        TEST(strcmp(em->competences[0], "ennuyeux") == 0);
        TEST(strcmp(em->competences[1], "maladroit") == 0);
        TEST(strcmp(em->competences[2], "empote") == 0);
        TEST(strcmp(em->competences[3], "pompeur d'air") == 0);
        TEST(strcmp(em->competences[4], "emmerdeur") == 0);
        TEST(em->id_entreprise == 100);
        TEST(em->id_collegues[0] == 66);
        TEST(em->id_collegues[1] == 67);
        TEST(em->id_collegues[2] == 68);
        TEST(em->id_collegues[3] == 69);
        TEST(em->id_collegues[4] == 70);

        free_employes(ems);
    }

    // Test pour créer un profil d'employé.
    {
        em_init();

        char const competences[5][128] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[5] = {66, 67, 68, 69, 70};

        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gladu.name", "00000", competences, 100, id_collegues);
        TEST(id != 0);
    }

    // Test pour rechercher un profil d'employe.
    {
        em_init();

        char const competences[5][128] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[5] = {66, 67, 68, 69, 70};
        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gladu.name", "00000", competences, 100, id_collegues);

        employe *em = em_recherche(id);

        TEST(em != NULL);
        TEST(strcmp(em->nom, "Gladu") == 0);
        TEST(strncmp(em->code_postal, "00000", 5) == 0);
        TEST(strcmp(em->mail, "gg@gladu.name") == 0);
    }

    // Test pour modifier un profil d'employe.
    {
        em_init();

        char incompetences[5][128] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[5] = {66, 67, 68, 69, 70};
        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gaston.name", "00000", incompetences, 100, id_collegues);

        char const competences[5][128] = {"charmant", "adroit", "gracieux", "amusant", "charmeur"};
        em_modifier_profil(id, "Gogol", "Gaston", "gg@gogol.name", "11111", competences, 100, id_collegues);

        employe *em = em_recherche(id);

        TEST(em->id == id);
        TEST(strcmp(em->nom, "Gogol") == 0);
        TEST(strcmp(em->prenom, "Gaston") == 0);
        TEST(strcmp(em->mail, "gg@gogol.name") == 0);
        TEST(strncmp(em->code_postal, "11111", 5) == 0);
        TEST(strcmp(em->competences[0], "charmant") == 0);
        TEST(strcmp(em->competences[1], "adroit") == 0);
        TEST(strcmp(em->competences[2], "gracieux") == 0);
        TEST(strcmp(em->competences[3], "amusant") == 0);
        TEST(strcmp(em->competences[4], "charmeur") == 0);
    }

    // Test pour écriture d'une structure chercheurs dans la BD.
    {
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste de chercheurs fictifs.
        chercheurs *chs = malloc(sizeof(chercheurs));
        chs->tete = NULL;

        chercheur *ch = malloc(sizeof(chercheur));
        ch->id = 100;
        strcpy(ch->nom, "Glandu");
        strcpy(ch->prenom, "Germaine");
        strcpy(ch->mail, "gg@glandu.name");
        strncpy(ch->code_postal, "00000", 5);
        strcpy(ch->competences[0], "etourdie");
        strcpy(ch->competences[1], "tete-en-l'air");
        strcpy(ch->competences[2], "paresseuse");
        strcpy(ch->competences[3], "maladroite");
        strcpy(ch->competences[4], "");
        ch->id_collegues[0] = 86;
        ch->id_collegues[1] = 87;
        ch->id_collegues[2] = 88;
        ch->id_collegues[3] = 89;
        ch->id_collegues[4] = 90;
        l_append(&(chs->tete), l_make_node(ch));

        // Écriture de ces chercheurs dans la BD.
        bd_ecriture_chercheurs(chs);
        free_chercheurs(chs);

        // Lecture des chercheurs de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_chercheurs(&chs);
        ch = (chercheur*)(chs->tete->data);
        TEST(ch->id == 100);
        TEST(strcmp(ch->nom, "Glandu") == 0);
        TEST(strcmp(ch->prenom, "Germaine") == 0);
        TEST(strcmp(ch->mail, "gg@glandu.name") == 0);
        TEST(strncmp(ch->code_postal, "00000", 5) == 0);
        TEST(strcmp(ch->competences[0], "etourdie") == 0);
        TEST(strcmp(ch->competences[1], "tete-en-l'air") == 0);
        TEST(strcmp(ch->competences[2], "paresseuse") == 0);
        TEST(strcmp(ch->competences[3], "maladroite") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);
        TEST(ch->id_collegues[0] == 86);
        TEST(ch->id_collegues[1] == 87);
        TEST(ch->id_collegues[2] == 88);
        TEST(ch->id_collegues[3] == 89);
        TEST(ch->id_collegues[4] == 90);

        free_chercheurs(chs);
    }

    // Test pour créer un profil de chercheur.
    {
        ch_init();

        char const competences[5][128] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[5] = {86, 87, 88, 89, 90};

        size_t const id = ch_creer_profil("Gladu", "Germaine", "gg@glandu.name", "00000", competences, id_collegues);
        TEST(id != 0);
    }

    // Test pour rechercher un profil de chercheur.
    {
        ch_init();

        char const competences[5][128] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[5] = {86, 87, 88, 89, 90};
        size_t const id = ch_creer_profil("Glandu", "Germaine", "gg@glandu.name", "00000", competences, id_collegues);

        chercheur *ch = ch_recherche(id);

        TEST(ch != NULL);
        TEST(strcmp(ch->nom, "Glandu") == 0);
        TEST(strncmp(ch->code_postal, "00000", 5) == 0);
        TEST(strcmp(ch->mail, "gg@glandu.name") == 0);
    }

    // Test pour modifier un profil de chercheur.
    {
        ch_init();

        char incompetences[5][128] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[5] = {86, 87, 88, 89, 90};
        size_t const id = ch_creer_profil("Glandu", "Germaine", "gg@glandu.name", "00000", incompetences, id_collegues);

        char const competences[5][128] = {"futee", "adroite", "assidue", "rigolote", ""};
        ch_modifier_profil(id, "Gogol", "Germaine", "gg@gogol.name", "11111", competences, id_collegues);

        chercheur *ch = ch_recherche(id);

        TEST(ch->id == id);
        TEST(strcmp(ch->nom, "Gogol") == 0);
        TEST(strcmp(ch->prenom, "Germaine") == 0);
        TEST(strcmp(ch->mail, "gg@gogol.name") == 0);
        TEST(strncmp(ch->code_postal, "11111", 5) == 0);
        TEST(strcmp(ch->competences[0], "futee") == 0);
        TEST(strcmp(ch->competences[1], "adroite") == 0);
        TEST(strcmp(ch->competences[2], "assidue") == 0);
        TEST(strcmp(ch->competences[3], "rigolote") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

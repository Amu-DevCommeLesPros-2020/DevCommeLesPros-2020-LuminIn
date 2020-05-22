#include "journal/journal.h"
#include "liste/liste.h"
#include "luminin/bd.h"
#include "luminin/chercheur.h"
#include "luminin/constantes.h"
#include "luminin/employe.h"
#include "luminin/entreprise.h"
#include "luminin/luminin.h"
#include "luminin/poste.h"

#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
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
    //  Quatre catégories de tests :
    //  1. Pour la bibliothèque libjournal.
    //  2. Pour l'interface bd_* de la bibliothèque libluminin.
    //  3. Pour les interfaces [ch/co/em/po]_* de la bibliothèque libluminin.
    //  4. Pour l'interface lu_* de la bibliothèque libluminin.

    // 1. Tests pour la bilbiothèque libjournal.

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

    // 2. Tests pour l'interface bd_* de la bibliothèque libluminin.

    // Tests pour la lecture d'une BD non-existante.
    {
#define chemin_nonexistant_bd "./nonexistant"

        bd_init(chemin_nonexistant_bd);

        chercheurs *chs;
        employes *ems;
        entreprises *ens;
        postes *pos;
        bd_lecture_chercheurs(&chs);
        bd_lecture_employes(&ems);
        bd_lecture_entreprises(&ens);
        bd_lecture_postes(&pos);

        TEST(chs->tete == NULL);
        TEST(ems->tete == NULL);
        TEST(ens->tete == NULL);
        TEST(pos->tete == NULL);
    }

    // Tests pour la lecture d'une BD existante.
    {
#define chemin_test_bd "../test"

        bd_init(chemin_test_bd);

        // Lecture d'une liste d'entreprises d'une BD dont les valeurs sont connues.
        entreprises *ens;
        bd_lecture_entreprises(&ens);

        TEST(ens != NULL);

        entreprise *disney = (entreprise*)(ens->tete->data);
        TEST(disney->id == I_ENTREPRISE + 1);
        TEST(strcmp(disney->nom, "Disney") == 0);
        TEST(strcmp(disney->code_postal, "77700") == 0);
        TEST(strcmp(disney->mail, "walt@disney.com") == 0);

        entreprise *google = (entreprise*)(l_skip(ens->tete, 1)->data);
        TEST(google->id == I_ENTREPRISE + 2);
        TEST(strcmp(google->nom, "Google") == 0);
        TEST(strcmp(google->code_postal, "75009") == 0);
        TEST(strcmp(google->mail, "emplois@google.com") == 0);

        free_entreprises(ens);

        // Lecture d'une liste de postes d'une BD dont les valeurs sont connues.
        postes *pos;
        bd_lecture_postes(&pos);

        TEST(pos != NULL);
        poste *acteur = (poste*)(pos->tete->data);
        TEST(acteur->id == I_POSTE + 1);
        TEST(strcmp(acteur->titre, "acteur") == 0);
        TEST(strcmp(acteur->competences[0], "comedie") == 0);
        TEST(strcmp(acteur->competences[1], "gag") == 0);
        TEST(strcmp(acteur->competences[2], "") == 0);
        TEST(strcmp(acteur->competences[3], "") == 0);
        TEST(strcmp(acteur->competences[4], "") == 0);
        TEST(acteur->id_entreprise == I_ENTREPRISE + 1);

        poste *developpeur = (poste*)(l_skip(pos->tete, 1)->data);
        TEST(developpeur->id == I_POSTE + 2);
        TEST(strcmp(developpeur->titre, "developpeur") == 0);
        TEST(strcmp(developpeur->competences[0], "C") == 0);
        TEST(strcmp(developpeur->competences[1], "SQL") == 0);
        TEST(strcmp(developpeur->competences[2], "Python") == 0);
        TEST(strcmp(developpeur->competences[3], "") == 0);
        TEST(strcmp(developpeur->competences[4], "") == 0);
        TEST(developpeur->id_entreprise == I_ENTREPRISE + 2);

        // Lecture d'une liste d'employés d'une BD dont les valeurs sont connues.
        employes *ems;
        bd_lecture_employes(&ems);

        TEST(ems != NULL);
        employe *em = (employe*)(ems->tete->data);
        TEST(em->id == I_EMPLOYE + 1);
        TEST(strcmp(em->nom, "Untel") == 0);
        TEST(strcmp(em->prenom, "Michel") == 0);
        TEST(strcmp(em->mail, "m_untel@google.com") == 0);
        TEST(strcmp(em->code_postal, "13010") == 0);
        TEST(strcmp(em->competences[0], "C++") == 0);
        TEST(strcmp(em->competences[1], "Python") == 0);
        TEST(strcmp(em->competences[2], "") == 0);
        TEST(strcmp(em->competences[3], "") == 0);
        TEST(strcmp(em->competences[4], "") == 0);
        TEST(em->id_entreprise == I_ENTREPRISE + 2);
        TEST(em->id_collegues[0] == 0);
        TEST(em->id_collegues[1] == 0);
        TEST(em->id_collegues[2] == 0);
        TEST(em->id_collegues[3] == 0);
        TEST(em->id_collegues[4] == 0);

        em = (employe*)(l_skip(ems->tete, 1)->data);
        TEST(em->id == I_EMPLOYE + 2);
        TEST(strcmp(em->nom, "Mouse") == 0);
        TEST(strcmp(em->prenom, "Mickey") == 0);
        TEST(strcmp(em->mail, "mickey@mickeyville.gov") == 0);
        TEST(strcmp(em->code_postal, "77700") == 0);
        TEST(strcmp(em->competences[0], "comedie") == 0);
        TEST(strcmp(em->competences[1], "") == 0);
        TEST(strcmp(em->competences[2], "") == 0);
        TEST(strcmp(em->competences[3], "") == 0);
        TEST(strcmp(em->competences[4], "") == 0);
        TEST(em->id_entreprise == I_ENTREPRISE + 1);
        TEST(em->id_collegues[0] == I_EMPLOYE + 3);
        TEST(em->id_collegues[1] == 0);
        TEST(em->id_collegues[2] == 0);
        TEST(em->id_collegues[3] == 0);
        TEST(em->id_collegues[4] == 0);

        // Lecture d'une liste de chercheurs d'une BD dont les valeurs sont connues.
        chercheurs *chs;
        bd_lecture_chercheurs(&chs);

        TEST(chs != NULL);
        chercheur *ch = (chercheur*)(chs->tete->data);
        TEST(ch->id == I_CHERCHEUR + 1);
        TEST(strcmp(ch->nom, "Duck") == 0);
        TEST(strcmp(ch->prenom, "Donald") == 0);
        TEST(strcmp(ch->mail, "donal.duck@canardville.gov") == 0);
        TEST(strcmp(ch->code_postal, "77700") == 0);
        TEST(strcmp(ch->competences[0], "comedie") == 0);
        TEST(strcmp(ch->competences[1], "gag") == 0);
        TEST(strcmp(ch->competences[2], "") == 0);
        TEST(strcmp(ch->competences[3], "") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);
        TEST(ch->id_collegues[0] == I_EMPLOYE + 2);
        TEST(ch->id_collegues[1] == 0);
        TEST(ch->id_collegues[2] == 0);
        TEST(ch->id_collegues[3] == 0);
        TEST(ch->id_collegues[4] == 0);

        ch = (chercheur*)(l_skip(chs->tete, 1)->data);
        TEST(ch->id == I_CHERCHEUR + 2);
        TEST(strcmp(ch->nom, "Pignon") == 0);
        TEST(strcmp(ch->prenom, "Francois") == 0);
        TEST(strcmp(ch->mail, "pignouf@gmail.com") == 0);
        TEST(strcmp(ch->code_postal, "75020") == 0);
        TEST(strcmp(ch->competences[0], "C") == 0);
        TEST(strcmp(ch->competences[1], "SQL") == 0);
        TEST(strcmp(ch->competences[2], "Python") == 0);
        TEST(strcmp(ch->competences[3], "patience") == 0);
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
        system("rm -rf " chemin_test_ecriture_bd "/*");
        mkdir(chemin_test_ecriture_bd, 0700);
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste d'entreprises fictives (avec une seule entreprise).
        entreprises *ens = calloc(1, sizeof(entreprises));

        entreprise *co = calloc(1, sizeof(entreprise));
        co->id = 100;
        strcpy(co->nom, "Fictive");
        strcpy(co->code_postal, "99000");
        strcpy(co->mail, "nobody@bowhere.com");
        l_append(&(ens->tete), l_make_node(co));

        // Écriture de ces entreprises dans la BD.
        bd_ecriture_entreprises(ens);
        free_entreprises(ens);

        // Lecture des entreprises de cette BD et test que les valeurs sont les mêmes.
        bd_lecture_entreprises(&ens);
        co = (entreprise*)(ens->tete->data);
        TEST(co->id == 100);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strcmp(co->code_postal, "99000") == 0);
        TEST(strcmp(co->mail, "nobody@bowhere.com") == 0);

        free_entreprises(ens);
    }

    //  3. Tests pour les interfaces [ch/co/em/po]_* de la bibliothèque libluminin.

    // Test pour créer un profil d'entreprise.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        en_init();

        size_t const id = en_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        TEST(id != 0);
    }

    // Test pour rechercher un profil d'entreprise.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        en_init();

        size_t const id = en_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        entreprise *co = en_recherche(id);

        TEST(co != NULL);
        TEST(strcmp(co->nom, "Fictive") == 0);
        TEST(strcmp(co->code_postal, "99000") == 0);
        TEST(strcmp(co->mail, "nobody@nowhere.com") == 0);
    }

    // Test pour supprimer un profil d'entreprise.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        en_init();

        size_t const id = en_creer_profil("Fictive", "99000", "nobody@nowhere.com");
        en_supprimer_profil(id);
        
        TEST(en_recherche(id) == NULL);

        size_t const id_1 = en_creer_profil("Fictive", "99000", "fictive@nullepart.com");
        size_t const id_2 = en_creer_profil("Irrelle", "99000", "irrelle@nullepart.com");
        en_supprimer_profil(id_1);
        TEST(en_recherche(id_1) == NULL);
        TEST(en_recherche(id_2) != NULL);

        en_supprimer_profil(id_2);
        TEST(en_recherche(id_2) == NULL);
    }

    // Test pour écriture d'une structure postes dans la BD.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste de postes fictifs.
        postes *pos = calloc(1, sizeof(postes));

        poste *po = calloc(1, sizeof(poste));
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
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        po_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"experience", "bagou", "patience", "", ""};
        size_t const id = po_creer_poste("Professeur", competences, 1);
        TEST(id != 0);

        po_destroy();
    }

    // Test pour supprimer un poste.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        po_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"experience", "bagou", "patience", "", ""};
        size_t const id = po_creer_poste("Professeur", competences, 1);
        po_supprimer_poste(id);
        
        TEST(po_recherche(id) == NULL);

        po_destroy();
    }

    // Test pour la liste des postes.
    {
        bd_init(chemin_test_bd);
        po_init();

        size_t ids[N_POSTES] = {0};
        po_ids(ids);

        TEST(ids[0] == I_POSTE + 1);
        TEST(ids[1] == I_POSTE + 2);
        TEST(ids[2] == I_POSTE + 3);
        TEST(ids[3] == I_POSTE + 4);
        TEST(ids[4] == 0);

        po_destroy();
    }

    // Test pour rechercher un poste.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        po_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"experience", "bagou", "patience", "", ""};
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
 
        po_destroy();
   }

    // Test pour écriture d'une structure employes dans la BD.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste dd'employes fictifs.
        employes *ems = calloc(1, sizeof(employes));

        employe *em = calloc(1, sizeof(employe));
        memset(em, 0, sizeof(*em));
        em->id = 100;
        strcpy(em->nom, "Gladu");
        strcpy(em->prenom, "Gaston");
        strcpy(em->mail, "gg@gladu.name");
        strcpy(em->code_postal, "00000");
        strcpy(em->competences[0], "ennuyeux");
        strcpy(em->competences[1], "maladroit");
        strcpy(em->competences[2], "empote");
        strcpy(em->competences[3], "pompeur d'air");
        strcpy(em->competences[4], "emmerdeur");
        em->id_entreprise = I_ENTREPRISE;
        em->id_collegues[0] = I_EMPLOYE + 66;
        em->id_collegues[1] = I_EMPLOYE + 67;
        em->id_collegues[2] = I_EMPLOYE + 68;
        em->id_collegues[3] = I_EMPLOYE + 69;
        em->id_collegues[4] = I_EMPLOYE + 70;
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
        TEST(strcmp(em->code_postal, "00000") == 0);
        TEST(strcmp(em->competences[0], "ennuyeux") == 0);
        TEST(strcmp(em->competences[1], "maladroit") == 0);
        TEST(strcmp(em->competences[2], "empote") == 0);
        TEST(strcmp(em->competences[3], "pompeur d'air") == 0);
        TEST(strcmp(em->competences[4], "emmerdeur") == 0);
        TEST(em->id_entreprise == I_ENTREPRISE);
        TEST(em->id_collegues[0] == I_EMPLOYE + 66);
        TEST(em->id_collegues[1] == I_EMPLOYE + 67);
        TEST(em->id_collegues[2] == I_EMPLOYE + 68);
        TEST(em->id_collegues[3] == I_EMPLOYE + 69);
        TEST(em->id_collegues[4] == I_EMPLOYE + 70);

        free_employes(ems);
    }

    // Test pour créer un profil d'employé.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        em_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[N_COLLEGUES] = {66, 67, 68, 69, 70};

        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gladu.name", "00000", competences, 100, id_collegues);
        TEST(id != 0);
    }

    // Test pour rechercher un profil d'employe.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        em_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[N_COLLEGUES] = {66, 67, 68, 69, 70};
        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gladu.name", "00000", competences, 100, id_collegues);

        employe *em = em_recherche(id);

        TEST(em != NULL);
        TEST(strcmp(em->nom, "Gladu") == 0);
        TEST(strcmp(em->code_postal, "00000") == 0);
        TEST(strcmp(em->mail, "gg@gladu.name") == 0);
    }

    // Test pour modifier un profil d'employe.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        em_init();

        char incompetences[N_COMPETENCES][L_COMPETENCE] = {"ennuyeux", "maladroit", "empote", "pompeur d'air", "emmerdeur"};
        size_t const id_collegues[N_COLLEGUES] = {66, 67, 68, 69, 70};
        size_t const id = em_creer_profil("Gladu", "Gaston", "gg@gaston.name", "00000", incompetences, 100, id_collegues);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"charmant", "adroit", "gracieux", "amusant", "charmeur"};
        em_modifier_profil(id, "11111", competences, 100, id_collegues);

        employe *em = em_recherche(id);

        TEST(em->id == id);
        TEST(strcmp(em->code_postal, "11111") == 0);
        TEST(strcmp(em->competences[0], "charmant") == 0);
        TEST(strcmp(em->competences[1], "adroit") == 0);
        TEST(strcmp(em->competences[2], "gracieux") == 0);
        TEST(strcmp(em->competences[3], "amusant") == 0);
        TEST(strcmp(em->competences[4], "charmeur") == 0);
    }

    // Test pour écriture d'une structure chercheurs dans la BD.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);

        // Creation d'une liste de chercheurs fictifs.
        chercheurs *chs = calloc(1, sizeof(chercheurs));

        chercheur *ch = calloc(1, sizeof(chercheur));
        ch->id = 100;
        strcpy(ch->nom, "Glandu");
        strcpy(ch->prenom, "Germaine");
        strcpy(ch->mail, "gg@glandu.name");
        strcpy(ch->code_postal, "00000");
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
        TEST(strcmp(ch->code_postal, "00000") == 0);
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
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        ch_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[N_COLLEGUES] = {86, 87, 88, 89, 90};

        size_t const id = ch_creer_profil("Gladu", "Germaine", "gg@glandu.name", "00000", competences, id_collegues);
        TEST(id != 0);

        ch_destroy();
    }

    // Test pour rechercher un profil de chercheur.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        ch_init();

        char competences[N_COMPETENCES][L_COMPETENCE] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[N_COLLEGUES] = {86, 87, 88, 89, 90};
        size_t const id = ch_creer_profil("Glandu", "Germaine", "gg@glandu.name", "00000", competences, id_collegues);

        chercheur *ch = ch_recherche(id);

        TEST(ch != NULL);
        TEST(strcmp(ch->nom, "Glandu") == 0);
        TEST(strcmp(ch->code_postal, "00000") == 0);
        TEST(strcmp(ch->mail, "gg@glandu.name") == 0);

        ch_destroy();
    }

    // Test pour modifier un profil de chercheur.
    {
        system("rm -rf " chemin_test_ecriture_bd "/*");
        bd_init(chemin_test_ecriture_bd);
        ch_init();

        char incompetences[N_COMPETENCES][L_COMPETENCE] = {"etourdie", "tete-en-l'air", "paresseuse", "maladroite", ""};
        size_t const id_collegues[N_COLLEGUES] = {I_EMPLOYE + 86, I_EMPLOYE + 87, I_EMPLOYE + 88, I_EMPLOYE + 89, I_EMPLOYE + 90};
        size_t const id = ch_creer_profil("Glandu", "Germaine", "gg@glandu.name", "00000", incompetences, id_collegues);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"futee", "adroite", "assidue", "rigolote", ""};
        ch_modifier_profil(id, "11111", competences, id_collegues);

        chercheur *ch = ch_recherche(id);

        TEST(ch->id == id);
        TEST(strcmp(ch->nom, "Glandu") == 0);
        TEST(strcmp(ch->prenom, "Germaine") == 0);
        TEST(strcmp(ch->mail, "gg@glandu.name") == 0);
        TEST(strcmp(ch->code_postal, "11111") == 0);
        TEST(strcmp(ch->competences[0], "futee") == 0);
        TEST(strcmp(ch->competences[1], "adroite") == 0);
        TEST(strcmp(ch->competences[2], "assidue") == 0);
        TEST(strcmp(ch->competences[3], "rigolote") == 0);
        TEST(strcmp(ch->competences[4], "") == 0);

        ch_destroy();
    }

    // Test pour la liste des chercheurs.
    {
        bd_init(chemin_test_bd);
        ch_init();

        size_t ids[N_CHERCHEURS] = {0};
        ch_ids(ids);

        TEST(ids[0] == I_CHERCHEUR + 1);
        TEST(ids[1] == I_CHERCHEUR + 2);
        TEST(ids[2] == 0);
        TEST(ids[3] == 0);
        TEST(ids[4] == 0);
    }

    // 4. Tests pour l'interface lu_*.

    // Tests pour nom d'entreprise.
    {
        lu_init(chemin_test_bd);

        TEST(strcmp(lu_nom_entreprise(I_ENTREPRISE + 1), "Disney") == 0);
        TEST(strcmp(lu_nom_entreprise(I_ENTREPRISE + 2), "Google") == 0);
        TEST(strcmp(lu_nom_entreprise(I_ENTREPRISE + 3), "Polytech") == 0);
        TEST(lu_nom_entreprise(4) == NULL);
    }

    // Tests pour créer une entreprise.
    {
        lu_init(chemin_test_ecriture_bd);

        size_t const id = lu_creer_profil_entreprise("Fictive", "99000", "rh@fictive.com");
        TEST(id != 0);
        TEST(strcmp(lu_nom_entreprise(id), "Fictive") == 0);
    }

    // Test pour suppression d'entreprise.
    {
        lu_init(chemin_test_ecriture_bd);

        size_t const id = lu_creer_profil_entreprise("Fictive", "99000", "rh@fictive.com");
        lu_supprimer_profil_entreprise(id);

        TEST(lu_nom_entreprise(id) == NULL);
    }

    // Test pour profile d'entreprise.
    {
        lu_init(chemin_test_bd);

        char nom[L_NOM];
        char code_postal[L_CP];
        char mail[L_MAIL];

        lu_profil_entreprise(I_ENTREPRISE + 1, nom, code_postal, mail);
        TEST(strcmp(nom, "Disney") == 0);
        TEST(strcmp(code_postal, "77700") == 0);
        TEST(strcmp(mail, "walt@disney.com") == 0);

        lu_profil_entreprise(I_ENTREPRISE + 5, nom, code_postal, mail);
        TEST(strcmp(nom, "") == 0);
        TEST(strcmp(code_postal, "") == 0);
        TEST(strcmp(mail, "") == 0);
    }

    // Test de création de poste.
    {
        lu_init(chemin_test_ecriture_bd);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"esprit", "humour", "", "", ""};
        size_t const id = lu_creer_poste("improvisateur", competences, 3);
        TEST(id != 0);
    }

    // Test de suppression de poste.
    {
        lu_init(chemin_test_ecriture_bd);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"esprit", "humour", "", "", ""};
        size_t const id = lu_creer_poste("improvisateur", competences, 3);
        lu_supprimer_poste(id);
    }

    // Test pour liste des postes par entreprise.
    {
        lu_init(chemin_test_bd);

        size_t ids_poste[N_POSTES];
        lu_postes_par_entreprise(I_ENTREPRISE + 1, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 1);
        TEST(ids_poste[1] == 0);

        lu_postes_par_entreprise(I_ENTREPRISE + 2, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 2);
        TEST(ids_poste[1] == 0);

        lu_postes_par_entreprise(I_ENTREPRISE + 3, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 3);
        TEST(ids_poste[1] == 0);
    }

    // Test pour nom de chercheur.
    {
        lu_init(chemin_test_bd);

        TEST(strcmp(lu_nom_chercheur(I_CHERCHEUR + 1), "Duck") == 0);
        TEST(strcmp(lu_nom_chercheur(I_CHERCHEUR + 2), "Pignon") == 0);
        TEST(lu_nom_chercheur(3) == NULL);
        TEST(lu_nom_chercheur(4) == NULL);
    }

    // Test pour création de profil de chercheur.
    {
        lu_init(chemin_test_ecriture_bd);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"esprit", "humour", "", "", ""};
        size_t const id = lu_creer_profil_chercheur("Rigolo", "Robert", "robert@rigolo.fr", "13001", competences, (const size_t []){0});
        TEST(id != 0);
        TEST(strcmp(lu_nom_chercheur(id), "Rigolo") == 0);
    }

    // Test pour suppression de profil de chercheur.
    {
        lu_init(chemin_test_ecriture_bd);

        char competences[N_COMPETENCES][L_COMPETENCE] = {"esprit", "humour", "", "", ""};
        size_t const id = lu_creer_profil_chercheur("Rigolo", "Robert", "robert@rigolo.fr", "13001", competences, (const size_t []){0});
        lu_supprimer_profil_chercheur(id);
        TEST(lu_nom_chercheur(id) == NULL);
    }

    // Test pour profil de chercheur.
    {
        lu_init(chemin_test_bd);

        char nom[L_NOM];
        char prenom[L_PRENOM];
        char code_postal[L_CP];
        char mail[L_MAIL];
        char competences[N_COMPETENCES][L_COMPETENCE];
        size_t id_collegues[N_COLLEGUES];

        lu_profil_chercheur(I_CHERCHEUR + 1, nom, prenom, mail, code_postal, competences, id_collegues);
        TEST(strcmp(nom, "Duck") == 0);
        TEST(strcmp(prenom, "Donald") == 0);
        TEST(strcmp(mail, "donal.duck@canardville.gov") == 0);
        TEST(strcmp(code_postal, "77700") == 0);
        TEST(strcmp(competences[0], "comedie") == 0);
        TEST(strcmp(competences[1], "gag") == 0);
        TEST(strcmp(competences[2], "") == 0);
        TEST(id_collegues[0] == I_EMPLOYE + 2);
        TEST(id_collegues[1] == 0);

        lu_profil_chercheur(I_CHERCHEUR + 3, nom, prenom, mail, code_postal, competences, id_collegues);
        TEST(strcmp(nom, "") == 0);
        TEST(strcmp(prenom, "") == 0);
        TEST(strcmp(mail, "") == 0);
        TEST(strcmp(code_postal, "") == 0);
        TEST(strcmp(competences[0], "") == 0);
        TEST(id_collegues[0] == 0);
    }

    // Test pour modification de profil de chercheur.
    {
        lu_init(chemin_test_ecriture_bd);

        char nom[L_NOM];
        char prenom[L_PRENOM];
        char code_postal[L_CP];
        char mail[L_MAIL];
        char competences[N_COMPETENCES][L_COMPETENCE];
        size_t id_collegues[N_COLLEGUES];
        lu_profil_chercheur(I_CHERCHEUR, nom, prenom, mail, code_postal, competences, id_collegues);

        strcpy(code_postal, "22222");
        strcpy(competences[4], "ponctualite");
        lu_modifier_profil_chercheur(I_CHERCHEUR, code_postal, competences, id_collegues);
        lu_profil_chercheur(I_CHERCHEUR, nom, prenom, mail, code_postal, competences, id_collegues);
        TEST(strcmp(code_postal, "22222") == 0);
        TEST(strcmp(competences[4], "ponctualite") == 0);
    }

    // Test pour recherche de postes par compétences.
    {
        lu_init(chemin_test_bd);

        size_t ids_poste[N_POSTES];
        
        char competences[N_COMPETENCES][L_COMPETENCE];
        lu_profil_chercheur(I_CHERCHEUR + 1, NULL, NULL, NULL, NULL, competences, NULL);

        lu_recherche_poste_par_competences(competences, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 1);
        TEST(ids_poste[1] == I_POSTE + 4);
        TEST(ids_poste[2] == 0);

        lu_profil_chercheur(I_CHERCHEUR + 2, NULL, NULL, NULL, NULL, competences, NULL);

        lu_recherche_poste_par_competences(competences, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 2);
        TEST(ids_poste[1] == I_POSTE + 3);
        TEST(ids_poste[2] == 0);
    }

    // Test pour recherche de postes par compétences et code postal.
    {
        lu_init(chemin_test_bd);

        size_t ids_poste[N_POSTES];
        
        char code_postal[L_CP];
        char competences[N_COMPETENCES][L_COMPETENCE];
        lu_profil_chercheur(I_CHERCHEUR + 1, NULL, NULL, NULL, code_postal, competences, NULL);

        lu_recherche_poste_par_competences_code_postal(competences, code_postal, ids_poste);
        TEST(ids_poste[0] == I_POSTE + 1);
        TEST(ids_poste[1] == 0);
        TEST(ids_poste[2] == 0);

        lu_profil_chercheur(I_CHERCHEUR + 2, NULL, NULL, NULL, code_postal, competences, NULL);

        lu_recherche_poste_par_competences_code_postal(competences, code_postal, ids_poste);
        TEST(ids_poste[0] == 0);
        TEST(ids_poste[1] == 0);
    }

    // Test pour recherche de collegues par compétences.
    {
        lu_init(chemin_test_bd);

        char competences_1[N_COMPETENCES][L_COMPETENCE] = {{"comedie"}, {"gag"}};
        size_t ids_collegue_1[N_EMPLOYES] = {I_EMPLOYE + 1, I_EMPLOYE + 2, I_EMPLOYE + 3};
        lu_recherche_collegue_par_competences(competences_1, ids_collegue_1);
        TEST(ids_collegue_1[0] == I_EMPLOYE + 2);
        TEST(ids_collegue_1[1] == I_EMPLOYE + 3);
        TEST(ids_collegue_1[2] == 0);

        char competences_2[N_COMPETENCES][L_COMPETENCE] = {{"C"}, {"SQL"}, {"Python"}, {"Java"}, {"Javascript"}};
        size_t ids_collegue_2[N_EMPLOYES] = {I_EMPLOYE + 1, I_EMPLOYE + 2, I_EMPLOYE + 3};
        lu_recherche_collegue_par_competences(competences_2, ids_collegue_2);
        TEST(ids_collegue_2[0] == I_EMPLOYE + 1);
        TEST(ids_collegue_2[1] == 0);
        TEST(ids_collegue_2[2] == 0);
    }

    // Test pour recherche de collegues par entreprise.
    {
        lu_init(chemin_test_bd);
        size_t ids_collegue_1[N_EMPLOYES] = {I_EMPLOYE + 1, I_EMPLOYE + 2, I_EMPLOYE + 3};
        lu_recherche_collegue_par_entreprise(I_ENTREPRISE + 1, ids_collegue_1);
        TEST(ids_collegue_1[0] == I_EMPLOYE + 2);
        TEST(ids_collegue_1[1] == I_EMPLOYE + 3);
        TEST(ids_collegue_1[2] == 0);

        size_t ids_collegue_2[N_EMPLOYES] = {I_EMPLOYE + 1, I_EMPLOYE + 2, I_EMPLOYE + 3};
        lu_recherche_collegue_par_entreprise(I_ENTREPRISE + 2, ids_collegue_2);
        TEST(ids_collegue_2[0] == I_EMPLOYE + 1);
        TEST(ids_collegue_2[1] == 0);
        TEST(ids_collegue_2[2] == 0);

        size_t ids_collegue_3[N_EMPLOYES] = {I_EMPLOYE + 1, I_EMPLOYE + 2, I_EMPLOYE + 3};
        lu_recherche_collegue_par_entreprise(I_ENTREPRISE + 3, ids_collegue_3);
        TEST(ids_collegue_3[0] == 0);
        TEST(ids_collegue_3[1] == 0);
        TEST(ids_collegue_3[2] == 0);
    }

    // Test pour recherche de chercheur par poste.
    {
        lu_init(chemin_test_bd);

        size_t ids_chercheur[N_CHERCHEURS];
        char competences[N_COMPETENCES][L_COMPETENCE];
        
        lu_poste(I_POSTE + 1, NULL, competences, NULL);
        lu_recherche_chercheur_par_competences(competences, ids_chercheur);
        TEST(ids_chercheur[0] == I_CHERCHEUR + 1);
        TEST(ids_chercheur[1] == 0);

        lu_poste(I_POSTE + 2, NULL, competences, NULL);
        lu_recherche_chercheur_par_competences(competences, ids_chercheur);
        TEST(ids_chercheur[0] == I_CHERCHEUR + 2);
        TEST(ids_chercheur[1] == 0);
    }

    // Test pour recherche de chercheur par poste et code_postal.
    {
        lu_init(chemin_test_bd);

        size_t ids_chercheur[N_CHERCHEURS];
        char competences[N_COMPETENCES][L_COMPETENCE];
        size_t id_entreprise;
        char code_postal[L_CP];

        lu_poste(I_POSTE + 1, NULL, competences, &id_entreprise);
        lu_profil_entreprise(id_entreprise, NULL, code_postal, NULL);
        lu_recherche_chercheur_par_competences_code_postal(competences, code_postal, ids_chercheur);
        TEST(ids_chercheur[0] == I_CHERCHEUR + 1);
        TEST(ids_chercheur[1] == 0);

        lu_poste(I_POSTE + 2, NULL, competences, &id_entreprise);
        lu_profil_entreprise(id_entreprise, NULL, code_postal, NULL);
        lu_recherche_chercheur_par_competences_code_postal(competences, code_postal, ids_chercheur);
        TEST(ids_chercheur[0] == 0);
        TEST(ids_chercheur[1] == 0);
    }

    printf("%d/%d\n", tests_reussis, tests_executes);

    return tests_executes - tests_reussis;
}

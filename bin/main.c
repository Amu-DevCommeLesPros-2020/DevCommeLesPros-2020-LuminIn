#include "journal/journal.h"
#include "luminin/luminin.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

char const* const chemin_journal = "./luminin.log"; // Chemin où trouver le journal.
char const* const chemin_bd = "./bd";   // Chemin où trouver la base de données.

// Identifiant et nom de l'utilisateur.
size_t id_utilisateur = 0;
char nom_utilisateur[L_NOM];

// Actions possibles.
typedef enum action
{
    INVALIDE,
    NAVIGUER_MENU_PRINCIPAL = 'p',
    QUITTER = 'q',
    NAVIGUER_MENU_PRECEDENT = 'r',
    NAVIGUER_MENU_ENTREPRISE,
    NAVIGUER_MENU_EMPLOYE,
    NAVIGUER_MENU_CHERCHEUR,
    IDENTIFICATION_ENTREPRISE,
    CREER_ENTREPRISE,
    SUPPRIMER_ENTREPRISE,
    CREER_POSTE,
    SUPPRIMER_POSTE,
    RECHERCHE_CHERCHEUR,
    IDENTIFICATION_CHERCHEUR,
    CREER_CHERCHEUR,
    MODIFIER_CHERCHEUR,
    TRANSITION_EMPLOYE,
    SUPPRIMER_CHERCHEUR,
    CHERCHEUR_RECHERCHE_POSTE,
    IDENTIFICATION_EMPLOYE,
    CREER_EMPLOYE,
    MODIFIER_EMPLOYE,
    TRANSITION_CHERCHEUR,
    SUPPRIMER_EMPLOYE,
    EMPLOYE_RECHERCHE_POSTE
} action;

// actions doit être de taille n + 1 et actions[n] doit décrire l'action à appliquer pour 'r' (menu précédent).
action choix(size_t const n, action const actions[])
{
    action a = INVALIDE;
    while(a == INVALIDE)
    {    
        printf("Votre choix [1-%zu],menu [p]rincipal,menu p[r]écédent,[q]uitter : ", n);
        char const c = getchar();
        if(c != '\n') while(getchar() != '\n');

        switch(c)
        {
            case '1':
                a = actions[0];
                break;
            case '2':
                a = n < 2 ? INVALIDE : actions[1];
                break;
            case '3':
                a = n < 3 ? INVALIDE : actions[2];
                break;
            case '4':
                a = n < 4 ? INVALIDE : actions[3];
                break;
            case '5':
                a = n < 5 ? INVALIDE : actions[4];
                break;
            case '6':
                a = n < 6 ? INVALIDE : actions[5];
                break;
            case NAVIGUER_MENU_PRINCIPAL:
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case NAVIGUER_MENU_PRECEDENT:
                a = actions[n];
                break;
            case QUITTER:
                return QUITTER;
                break;
            default:
                break;
        }
    }

    return a;
}

void en_tete()
{
    printf("\n*** Bienvenu sur LuminIn, le site des pros ***\n");
}

action menu_principal()
{
    printf("  * Menu principal *\n\n\
Vous êtes :\n\
1. Une entreprise\n\
2. Un employé\n\
3. À la recherche d'un emploi\n\n");

    return choix(3, (action[]){IDENTIFICATION_ENTREPRISE, IDENTIFICATION_EMPLOYE, IDENTIFICATION_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

action identification_entreprise()
{
    printf("  * Menu entreprise *\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
    int essais = 5;

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil d'entreprise, le nom de celle-ci.\n");
    char identifiant_entreprise[L_NOM] = {'\0'};
    while(!id_utilisateur && essais--)
    {
        printf("Identifiant : ");
        scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", identifiant_entreprise);

        long long const identifiant_numerique = atoll(identifiant_entreprise);
        if(identifiant_numerique)
        {
            char const* const n = lu_nom_entreprise(identifiant_numerique);
            if(n)
            {
                id_utilisateur = identifiant_numerique;
                strcpy(nom_utilisateur, n);
                return NAVIGUER_MENU_ENTREPRISE;
            }
        }
        else
        {
            strcpy(nom_utilisateur, identifiant_entreprise);
            return CREER_ENTREPRISE;
        }
    }

    return NAVIGUER_MENU_PRINCIPAL;
}

void creer_entreprise()
{
    printf("  * Menu entreprise *\n\n");
    
    printf("Nom : %s\n", nom_utilisateur);

    printf("Code postal : ");
    char code_postal[L_CP];
    scanf(" %" STRINGIZE(L_CP) "[^\n]%*c", code_postal);

    printf("Mail : ");
    char mail[L_MAIL];
    scanf(" %" STRINGIZE(L_MAIL) "[^\n]%*c", mail);

    id_utilisateur = lu_creer_profil_entreprise(nom_utilisateur, code_postal, mail);
    printf("Votre identifiant : %zu\n\n", id_utilisateur);
}

void supprimer_entreprise()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu entreprise [%s] *\n\n", nom_utilisateur);

    lu_supprimer_profil_entreprise(id_utilisateur);

    printf("Entreprise supprimée.\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
}

action menu_entreprise()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu entreprise [%s] *\n\n\
Vous voulez :\n\
1. Supprimer le profil de votre entreprise\n\
2. Créer le profil d'un poste à pourvoir\n\
3. Supprimer le profil d'un poste maintenant pourvu\n\
4. Faire une recherche parmi les chercheurs d'emploi\n\n", nom_utilisateur);

    return choix(4, (action []){SUPPRIMER_ENTREPRISE, CREER_POSTE, SUPPRIMER_POSTE, RECHERCHE_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

void creer_poste()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu entreprise [%s] *\n\n", nom_utilisateur);
    
    printf("Titre : ");
    char titre[L_TITRE];
    fgets(titre, L_TITRE, stdin);
    titre[strcspn(titre, "\n")] = '\0';

    char competences[N_COMPETENCES][L_COMPETENCE];
    for(int i = 0; i != N_COMPETENCES; ++i)
    {
        competences[i][0] = '\0';
    }

    bool derniere = false;
    for(int i = 0; i != N_COMPETENCES && !derniere; ++i)
    {
        printf("Compétence %d : ", i + 1);
        fgets(competences[i], L_COMPETENCE, stdin);

        competences[i][strcspn(competences[i], "\n")] = '\0';
        if(competences[i][0] == '\0')
        {
            derniere = true;
        }
    }

    size_t const identifiant = lu_creer_poste(titre, competences, id_utilisateur);
    printf("Identifiant de ce poste : %zu\n\n", identifiant);
}

void supprimer_poste()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu entreprise [%s]* \n\n", nom_utilisateur);

    printf("Identifiant : ");
    size_t identifiant;
    scanf(" %zu%*c", &identifiant);

    lu_supprimer_poste(identifiant);
}

void recherche_chercheur()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu entreprise [%s] *\n\n", nom_utilisateur);

    printf("Pour le poste [");
    size_t ids_poste[N_POSTES];
    lu_postes_par_entreprise(id_utilisateur, ids_poste);
    for(size_t i = 0; ids_poste[i] != 0 && i != N_POSTES; ++i)
    {
        if(i != 0)
        {
            printf(", ");
        }
        printf("%zu", ids_poste[i]);
    }
    printf("] : ");
    
    size_t id_poste;
    scanf(" %zu", &id_poste);
    char competences[N_COMPETENCES][L_COMPETENCE];
    lu_poste(id_poste, NULL, competences, NULL);

    printf("Recerche restreinte au code postal [o/n] : ");
    char cp;
    scanf(" %c", &cp);
    getchar();

    size_t ids_chercheur[N_CHERCHEURS] = {0};
    if(cp != 'o')
    {
        lu_recherche_chercheur_par_competences(competences, ids_chercheur);
    }
    else
    {
        char code_postal[L_CP];
        lu_profil_entreprise(id_utilisateur, NULL, code_postal, NULL);
        lu_recherche_chercheur_par_competences_code_postal(competences, code_postal, ids_chercheur);
    }

    if(ids_chercheur[0] == 0)
    {
        printf("Aucun chercheur qualifie pour ce poste.\n");
    }
    else
    {
        printf("\nChercheurs qualifies par ordre de competences :\n");
        for(size_t i = 0; ids_chercheur[i] != 0 && i != N_CHERCHEURS; ++i)
        {
            char nom[L_NOM];
            char prenom[L_PRENOM];
            char mail[L_MAIL];

            lu_profil_chercheur(ids_chercheur[i], nom, prenom, mail, NULL, NULL, NULL);

            printf("%zu. Nom : %s, Prenom : %s, Mail : %s", i + 1, nom, prenom, mail);
        }
        printf("\n");
    }
}

action identification_chercheur()
{
    printf("  * Menu chercheur *\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
    int essais = 5;

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil de chercheur d'emploi, écrivez votre nom.\n");
    char identifiant_chercheur[L_NOM] = {'\0'};
    while(!id_utilisateur && essais--)
    {
        printf("Identifiant : ");
        scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", identifiant_chercheur);

        long long const identifiant_numerique = atoll(identifiant_chercheur);
        if(identifiant_numerique)
        {
            char const* const n = lu_nom_chercheur(identifiant_numerique);
            if(n)
            {
                id_utilisateur = identifiant_numerique;
                strcpy(nom_utilisateur, n);
                return NAVIGUER_MENU_CHERCHEUR;
            }
        }
        else
        {
            strcpy(nom_utilisateur, identifiant_chercheur);
            return CREER_CHERCHEUR;
        }
    }

    return NAVIGUER_MENU_PRINCIPAL;
}

void creer_chercheur()
{
    printf("  * Menu chercheur *\n\n");

    printf("Nom : %s\n", nom_utilisateur);

    printf("Prénom : ");
    char prenom[L_NOM + 1];
    scanf(" %" STRINGIZE(L_NOM) "s%*c", prenom);

    printf("Code postal : ");
    char code_postal[L_CP];
    scanf(" %" STRINGIZE(L_CP) "[^\n]%*c", code_postal);

    printf("Mail : ");
    char mail[L_MAIL];
    scanf(" %" STRINGIZE(L_MAIL) "[^\n]%*c", mail);

    char competences[N_COMPETENCES][L_COMPETENCE];
    for(int i = 0; i != N_COMPETENCES; ++i)
    {
        competences[i][0] = '\0';
    }

    bool derniere = false;
    for(int i = 0; i != N_COMPETENCES && !derniere; ++i)
    {
        printf("Compétence %d : ", i + 1);
        fgets(competences[i], L_COMPETENCE, stdin);

        competences[i][strcspn(competences[i], "\n")] = '\0';
        if(competences[i][0] == '\0')
        {
            derniere = true;
        }
    }

    size_t collegues[N_COLLEGUES] = {0};

    bool dernier = false;
    for(int i = 0; i != N_COLLEGUES && !dernier; ++i)
    {
        printf("Identifiant de collègues %d ('0' pour terminer) : ", i + 1);
        scanf("%zu", &(collegues[i]));

        if(collegues[i] == 0)
        {
            dernier = true;
        }
    }

    id_utilisateur = lu_creer_profil_chercheur(nom_utilisateur, prenom, mail, code_postal, competences, collegues);
    printf("Votre identifiant : %zu\n\n", id_utilisateur);
}

void modifier_chercheur()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu chercheur [%s] *\n\n", nom_utilisateur);

    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
    size_t id_collegues[N_COLLEGUES] = {0};

    lu_profil_chercheur(id_utilisateur, nom, prenom, mail, code_postal, competences, id_collegues);

    printf("Code postal [%s] : ", code_postal);
    char cp[L_CP + 1];
    fgets(cp, L_CP + 1, stdin);
    cp[strcspn(cp, "\n")] = '\0';
    if(strlen(cp)) strcpy(code_postal, cp);

    bool derniere = false;
    for(int i = 0; i != N_COMPETENCES && !derniere; ++i)
    {
        printf("Compétence %d [%s] : ", i + 1, competences[i]);
        char c[L_COMPETENCE];
        fgets(c, L_COMPETENCE, stdin);
        c[strcspn(c, "\n")] = '\0';
        if(strlen(c)) strcpy(competences[i], c);
        
        if(competences[i][0] == '\0')
        {
            derniere = true;
        }
    }

    bool dernier = false;
    for(int i = 0; i != N_COLLEGUES && !dernier; ++i)
    {
        printf("Identifiant de collègues %d [%zu] : ", i + 1, id_collegues[i]);
        char c[10];
        fgets(c, sizeof(c), stdin);
        c[strcspn(c, "\n")] = '\0';
        size_t id = 0;
        if((id = atoll(c))) id_collegues[i] = id;

        if(id_collegues[i] == 0)
        {
            dernier = true;
        }
    }

    lu_modifier_profil_chercheur(id_utilisateur, code_postal, competences, id_collegues);
}

void transition_employe()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu chercheur [%s] *\n\n", nom_utilisateur);

    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE] = {{'\0'}};
    size_t id_collegues[N_COLLEGUES] = {0};
    lu_profil_chercheur(id_utilisateur, nom, prenom, mail, code_postal, competences, id_collegues);

    lu_supprimer_profil_chercheur(id_utilisateur);

    size_t id_entreprise;
    printf("Identifiant de l'entreprise : ");
    scanf("%zu", &id_entreprise);
    getchar();

    id_utilisateur = lu_creer_profil_employe(nom, prenom, mail, code_postal, competences, id_entreprise, id_collegues);

    printf("Votre nouvel identifiant : %zu\n\n", id_utilisateur);
}

void supprimer_chercheur()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu chercheur [%s] *\n\n", nom_utilisateur);

    lu_supprimer_profil_chercheur(id_utilisateur);

    printf("Chercheur suppmimé.\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
}

void chercheur_recherche_poste()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu chercheur [%s] *\n\n", nom_utilisateur);

    char competences[N_COMPETENCES][L_COMPETENCE];
    char code_postal[L_CP];
    lu_profil_chercheur(id_utilisateur, NULL, NULL, NULL, code_postal, competences, NULL);

    size_t ids_poste[N_POSTES];

    printf("Recerche restreinte au code postal [o/n] : ");
    char cp;
    scanf(" %c", &cp);
    getchar();

    if(cp != 'o')
    {
        lu_recherche_poste_par_competences(competences, ids_poste);
    }
    else
    {
        lu_recherche_poste_par_competences_code_postal(competences, code_postal, ids_poste);
    }

    if(ids_poste[0] == 0)
    {
        printf("Aucun des postes ne correspond a vos competences.\n");
    }
    else
    {
        printf("\nPoste(s) pour lesquels vous vous qualifiez par ordre de competences :\n");
        for(size_t i = 0; ids_poste[i] != 0 && i != N_POSTES; ++i)
        {
            size_t id_utilisateur;
            char titre[L_TITRE];
            lu_poste(ids_poste[i], titre, NULL, &id_utilisateur);

            char nom[L_NOM];
            char mail[L_MAIL];
            char code_postal[L_CP];
            lu_profil_entreprise(id_utilisateur, nom, code_postal, mail);

            printf("%zu. Titre : %s, Entreprise : %s, Mail : %s, Code postal : %s\n", i + 1, titre, nom, mail, code_postal);
        }
    }
}

action menu_chercheur()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu chercheur [%s] *\n\n", nom_utilisateur);
    printf("Vous voulez :\n\
1. Modifier votre profil\n\
2. Transitionner vers un profil d'employé\n\
3. Supprimer votre profil\n\
4. Faire une recherche parmi les postes offerts\n\n");

    return choix(4, (action []){MODIFIER_CHERCHEUR, TRANSITION_EMPLOYE, SUPPRIMER_CHERCHEUR, CHERCHEUR_RECHERCHE_POSTE, NAVIGUER_MENU_PRINCIPAL});
}

action identification_employe()
{
    printf("  * Menu employe *\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
    int essais = 5;

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil d'employé, écrivez votre nom.\n");
    char identifiant_employe[L_NOM] = {'\0'};
    while(!id_utilisateur && essais--)
    {
        printf("Identifiant : ");
        scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", identifiant_employe);

        long long const identifiant_numerique = atoll(identifiant_employe);
        if(identifiant_numerique)
        {
            char const* const n = lu_nom_employe(identifiant_numerique);
            if(n)
            {
                id_utilisateur = identifiant_numerique;
                strcpy(nom_utilisateur, n);
                return NAVIGUER_MENU_EMPLOYE;
            }
        }
        else
        {
            strcpy(nom_utilisateur, identifiant_employe);
            return CREER_EMPLOYE;
        }
    }

    return NAVIGUER_MENU_PRINCIPAL;
}

void creer_employe()
{
    printf("  * Menu employé *\n\n");

    printf("Nom : %s\n", nom_utilisateur);

    printf("Prénom : ");
    char prenom[L_NOM + 1];
    scanf(" %" STRINGIZE(L_NOM) "s%*c", prenom);

    printf("Code postal : ");
    char code_postal[L_CP];
    scanf(" %" STRINGIZE(L_CP) "[^\n]%*c", code_postal);

    printf("Mail : ");
    char mail[L_MAIL];
    scanf(" %" STRINGIZE(L_MAIL) "[^\n]%*c", mail);

    char competences[N_COMPETENCES][L_COMPETENCE];
    for(int i = 0; i != N_COMPETENCES; ++i)
    {
        competences[i][0] = '\0';
    }

    bool derniere = false;
    for(int i = 0; i != N_COMPETENCES && !derniere; ++i)
    {
        printf("Compétence %d : ", i + 1);
        fgets(competences[i], L_COMPETENCE, stdin);

        competences[i][strcspn(competences[i], "\n")] = '\0';
        if(competences[i][0] == '\0')
        {
            derniere = true;
        }
    }

    size_t id_entreprise;
    printf("Identifiant d'entreprise : ");
    scanf("%zu", &id_entreprise);

    size_t collegues[N_COLLEGUES] = {0};

    bool dernier = false;
    for(int i = 0; i != N_COLLEGUES && !dernier; ++i)
    {
        printf("Identifiant de collègues %d ('0' pour terminer) : ", i + 1);
        scanf("%zu", &(collegues[i]));

        if(collegues[i] == 0)
        {
            dernier = true;
        }
    }

    id_utilisateur = lu_creer_profil_employe(nom_utilisateur, prenom, mail, code_postal, competences, id_entreprise, collegues);
    printf("Votre identifiant : %zu\n\n", id_utilisateur);
}

void modifier_employe()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu employé [%s] *\n\n", nom_utilisateur);

    char code_postal[L_CP];
    size_t id_entreprise;
    char competences[N_COMPETENCES][L_COMPETENCE] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
    size_t id_collegues[N_COLLEGUES] = {0};

    lu_profil_employe(id_utilisateur, NULL, NULL, NULL, code_postal, competences, &id_entreprise, id_collegues);

    printf("Code postal [%s] : ", code_postal);
    char cp[L_CP + 1];
    fgets(cp, L_CP + 1, stdin);
    cp[strcspn(cp, "\n")] = '\0';
    if(strlen(cp)) strcpy(code_postal, cp);

    bool derniere = false;
    for(int i = 0; i != N_COMPETENCES && !derniere; ++i)
    {
        printf("Compétence %d [%s] : ", i + 1, competences[i]);
        char c[L_COMPETENCE];
        fgets(c, L_COMPETENCE, stdin);
        c[strcspn(c, "\n")] = '\0';
        if(strlen(c)) strcpy(competences[i], c);
        
        if(competences[i][0] == '\0')
        {
            derniere = true;
        }
    }

    printf("Identifiant d'entreprise [%zu]: ", id_entreprise);
    char c[10];
    fgets(c, sizeof(c), stdin);
    c[strcspn(c, "\n")] = '\0';
    size_t id = 0;
    if((id = atoll(c))) id_entreprise = id;

    bool dernier = false;
    for(int i = 0; i != N_COLLEGUES && !dernier; ++i)
    {
        printf("Identifiant de collègues %d [%zu] : ", i + 1, id_collegues[i]);
        char c[10];
        fgets(c, sizeof(c), stdin);
        c[strcspn(c, "\n")] = '\0';
        size_t id = 0;
        if((id = atoll(c))) id_collegues[i] = id;

        if(id_collegues[i] == 0)
        {
            dernier = true;
        }
    }

    lu_modifier_profil_employe(id_utilisateur, code_postal, competences, id_entreprise, id_collegues);
}

void transition_chercheur()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu employé [%s] *\n\n", nom_utilisateur);

    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
    size_t id_collegues[N_COLLEGUES] = {0};
    lu_profil_employe(id_utilisateur, nom, prenom, mail, code_postal, competences, NULL, id_collegues);

    lu_supprimer_profil_employe(id_utilisateur);

    id_utilisateur = lu_creer_profil_chercheur(nom, prenom, mail, code_postal, competences, id_collegues);

    printf("Votre nouvel identifiant : %zu\n\n", id_utilisateur);
}

void supprimer_employe()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu employé [%s] *\n\n", nom_utilisateur);

    lu_supprimer_profil_employe(id_utilisateur);

    printf("Employé suppmimé.\n\n");

    id_utilisateur = 0;
    nom_utilisateur[0] = '\0';
}

void employe_recherche_poste()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu employé [%s] *\n\n", nom_utilisateur);

    char competences[N_COMPETENCES][L_COMPETENCE];
    char code_postal[L_CP];
    size_t employe_id_entreprise;
    lu_profil_employe(id_utilisateur, NULL, NULL, NULL, code_postal, competences, &employe_id_entreprise, NULL);

    size_t ids_poste[N_POSTES];

    printf("Recerche restreinte au code postal [o/n] : ");
    char cp;
    scanf(" %c", &cp);
    getchar();

    if(cp != 'o')
    {
        lu_recherche_poste_par_competences(competences, ids_poste);
    }
    else
    {
        lu_recherche_poste_par_competences_code_postal(competences, code_postal, ids_poste);
    }

    // Filtration des résultats obtenus précédement pour éliminer les postes de l'entreprise pour laquelle l'employé travaille présentement.
    size_t ids[N_POSTES];
    memcpy(ids, ids_poste, N_POSTES * sizeof(size_t));
    memset(ids_poste, 0, N_POSTES * sizeof(size_t));
    for(size_t i = 0, j = 0; ids[i] != 0 && i != N_POSTES; ++i)
    {
        size_t poste_id_entreprise;
        lu_poste(ids[i], NULL, NULL, &poste_id_entreprise);

        if(poste_id_entreprise != employe_id_entreprise)
        {
            ids_poste[j++] = ids[i];
        }
    }

    if(ids_poste[0] == 0)
    {
        printf("Aucun des postes ne correspond a vos competences.\n");
    }
    else
    {
        printf("\nPoste(s) pour lesquels vous vous qualifiez par ordre de competences :\n");
        for(size_t i = 0; ids_poste[i] != 0 && i != N_POSTES; ++i)
        {
            size_t id_entreprise;
            char titre[L_TITRE];
            lu_poste(ids_poste[i], titre, NULL, &id_entreprise);

            char nom[L_NOM];
            char mail[L_MAIL];
            char code_postal[L_CP];
            lu_profil_entreprise(id_entreprise, nom, code_postal, mail);

            printf("%zu. Titre : %s, Entreprise : %s, Mail : %s, Code postal : %s\n", i + 1, titre, nom, mail, code_postal);
        }
    }
}

action menu_employe()
{
    assert(id_utilisateur);
    assert(strlen(nom_utilisateur));

    printf("  * Menu employe [%s] *\n\n", nom_utilisateur);
    printf("Vous voulez :\n\
1. Modifier votre profil\n\
2. Transitionner vers un profil de chercheur d'emploi\n\
3. Supprimer votre profil\n\
4. Faire une recherche parmi les postes offerts\n\n");

    return choix(4, (action []){MODIFIER_EMPLOYE, TRANSITION_CHERCHEUR, SUPPRIMER_EMPLOYE, EMPLOYE_RECHERCHE_POSTE, NAVIGUER_MENU_PRINCIPAL});
}

int main()
{
    j_ouvrir(chemin_journal);
    lu_init(chemin_bd);

    action a = NAVIGUER_MENU_PRINCIPAL;
    while(a != QUITTER)
    {
        en_tete();

        switch(a)
        {
            case NAVIGUER_MENU_PRINCIPAL:
                a = menu_principal();
                break;
            case IDENTIFICATION_ENTREPRISE:
                a = identification_entreprise();
                break;
            case NAVIGUER_MENU_ENTREPRISE:
                a = menu_entreprise();
                break;
            case CREER_ENTREPRISE:
                creer_entreprise();
                a = NAVIGUER_MENU_ENTREPRISE;
                break;
            case SUPPRIMER_ENTREPRISE:
                supprimer_entreprise();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case CREER_POSTE:
                creer_poste();
                a = NAVIGUER_MENU_ENTREPRISE;
                break;
            case SUPPRIMER_POSTE:
                supprimer_poste();
                a = NAVIGUER_MENU_ENTREPRISE;
                break;
            case RECHERCHE_CHERCHEUR:
                recherche_chercheur();
                a = NAVIGUER_MENU_ENTREPRISE;
                break;
            case IDENTIFICATION_CHERCHEUR:
                a = identification_chercheur();
                break;
            case CREER_CHERCHEUR:
                creer_chercheur();
                a = NAVIGUER_MENU_CHERCHEUR;
                break;
            case NAVIGUER_MENU_CHERCHEUR:
                a = menu_chercheur();
                break;
            case MODIFIER_CHERCHEUR:
                modifier_chercheur();
                a = NAVIGUER_MENU_CHERCHEUR;
                break;
            case TRANSITION_EMPLOYE:
                transition_employe();
                a = NAVIGUER_MENU_EMPLOYE;
                break;
            case SUPPRIMER_CHERCHEUR:
                supprimer_chercheur();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case CHERCHEUR_RECHERCHE_POSTE:
                chercheur_recherche_poste();
                a = NAVIGUER_MENU_CHERCHEUR;
                break;
            case IDENTIFICATION_EMPLOYE:
                a = identification_employe();
                break;
            case CREER_EMPLOYE:
                creer_employe();
                a = NAVIGUER_MENU_EMPLOYE;
                break;
            case NAVIGUER_MENU_EMPLOYE:
                a = menu_employe();
                break;
            case MODIFIER_EMPLOYE:
                modifier_employe();
                a = NAVIGUER_MENU_EMPLOYE;
                break;
            case TRANSITION_CHERCHEUR:
                transition_chercheur();
                a = NAVIGUER_MENU_CHERCHEUR;
                break;
            case SUPPRIMER_EMPLOYE:
                supprimer_employe();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case EMPLOYE_RECHERCHE_POSTE:
                employe_recherche_poste();
                a = NAVIGUER_MENU_EMPLOYE;
                break;
            case QUITTER:
            default:
                break;
        }
    }

    lu_destroy();
    j_fermer();

    return 0;
}

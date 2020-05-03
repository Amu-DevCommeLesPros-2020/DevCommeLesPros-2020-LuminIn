#include "journal/journal.h"
#include "luminin/luminin.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

char const* const chemin_journal = "./luminin.log";
char const* const chemin_bd = "./bd";

size_t id_entreprise = 0;
char nom_entreprise[L_NOM];

size_t id_chercheur = 0;
char nom_chercheur[L_NOM];

typedef enum action
{
    INVALIDE,
    NAVIGUER_MENU_PRINCIPAL = 'p',
    QUITTER = 'q',
    NAVIGUER_MENU_PRECEDENT = 'r',
    NAVIGUER_MENU_ENTREPRISE,
    NAVIGUER_MENU_EMPLOYE,
    NAVIGUER_MENU_CHERCHEUR,
    S_IDENTIFIER_ENTREPRISE,
    CREER_ENTREPRISE,
    SUPPRIMER_ENTREPRISE,
    CREER_POSTE,
    SUPPRIMER_POSTE,
    RECHERCHE_CHERCHEUR,
    S_IDENTIFIER_CHERCHEUR,
    CREER_CHERCHEUR,
    MODIFIER_CHERCHEUR,
    TRANSITION_EMPLOYE,
    SUPPRIMER_CHERCHEUR
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

    return choix(3, (action[]){S_IDENTIFIER_ENTREPRISE, NAVIGUER_MENU_EMPLOYE, S_IDENTIFIER_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

action identification_entreprise()
{
    printf("  * Menu entreprise *\n\n");

    id_entreprise = 0;
    nom_entreprise[0] = '\0';
    int essais = 5;

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil d'entreprise, le nom de celle-ci.\n");
    char identifiant_entreprise[L_NOM] = {'\0'};
    while(!id_entreprise && essais--)
    {
        printf("Identifiant : ");
        scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", identifiant_entreprise);

        long long const identifiant_numerique = atoll(identifiant_entreprise);
        if(identifiant_numerique)
        {
            char const* const n = lu_nom_entreprise(identifiant_numerique);
            if(n)
            {
                id_entreprise = identifiant_numerique;
                strcpy(nom_entreprise, n);
                return NAVIGUER_MENU_ENTREPRISE;
            }
        }
        else
        {
            strcpy(nom_entreprise, identifiant_entreprise);
            return CREER_ENTREPRISE;
        }
    }

    return NAVIGUER_MENU_PRINCIPAL;
}

void creer_entreprise()
{
    printf("  * Menu entreprise *\n\n");
    
    printf("Nom : %s\n", nom_entreprise);

    printf("Code postal : ");
    char code_postal[L_CP + 1];
    scanf("%" STRINGIZE(L_CP) "c%*c", code_postal);

    printf("Mail : ");
    char mail[L_MAIL];
    scanf(" %" STRINGIZE(L_MAIL) "[^\n]%*c", mail);

    id_entreprise = lu_creer_profil_entreprise(nom_entreprise, code_postal, mail);
    printf("Votre identifiant : %zu\n\n", id_entreprise);
}

void supprimer_entreprise()
{
    assert(id_entreprise);
    assert(nom_entreprise);

    printf("  * Menu entreprise [%s] *\n\n", nom_entreprise);

    lu_supprimer_profil_entreprise(id_entreprise);

    printf("Entreprise supprimée.\n\n");

    id_entreprise = 0;
    nom_entreprise[0] = '\0';
}

action menu_entreprise()
{
    assert(id_entreprise);
    assert(nom_entreprise);

    printf("  * Menu entreprise [%s] *\n\n\
Vous voulez :\n\
2. Supprimer le profil de votre entreprise\n\
3. Créer le profil d'un poste à pourvoir\n\
4. Supprimer le profil d'un poste maintenant pourvu\n\
5. Faire une recherche parmi les chercheurs d'emploi\n\n", nom_entreprise);

    return choix(4, (action []){SUPPRIMER_ENTREPRISE, CREER_POSTE, SUPPRIMER_POSTE, RECHERCHE_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

void creer_poste()
{
    assert(id_entreprise);
    assert(nom_entreprise);

    printf("  * Menu entreprise [%s] *\n\n", nom_entreprise);
    
    printf("Titre : ");
    char titre[L_TITRE];
    scanf(" %" STRINGIZE(L_TITRE) "[^\n]%*c", titre);

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

    size_t const identifiant = lu_creer_poste(titre, competences, id_entreprise);
    printf("Identifiant de ce poste : %zu\n\n", identifiant);
}

void supprimer_poste()
{
    assert(id_entreprise);
    assert(nom_entreprise);

    printf("  * Menu entreprise [%s]* \n\n", nom_entreprise);

    printf("Identifiant : ");
    size_t identifiant;
    scanf(" %zu%*c", &identifiant);

    lu_supprimer_poste(identifiant);
}

action identification_chercheur()
{
    printf("  * Menu chercheur *\n\n");

    id_chercheur = 0;
    nom_chercheur[0] = '\0';
    int essais = 5;

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil de chercheur d'emploi, écrivez votre nom.\n");
    char identifiant_chercheur[L_NOM] = {'\0'};
    while(!id_chercheur && essais--)
    {
        printf("Identifiant : ");
        scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", identifiant_chercheur);

        long long const identifiant_numerique = atoll(identifiant_chercheur);
        if(identifiant_numerique)
        {
            char const* const n = lu_nom_chercheur(identifiant_numerique);
            if(n)
            {
                id_chercheur = identifiant_numerique;
                strcpy(nom_chercheur, n);
                return NAVIGUER_MENU_CHERCHEUR;
            }
        }
        else
        {
            strcpy(nom_chercheur, identifiant_chercheur);
            return CREER_CHERCHEUR;
        }
    }

    return NAVIGUER_MENU_PRINCIPAL;
}

void creer_chercheur()
{
    printf("  * Menu chercheur *\n\n");
    
    printf("Nom : %s\n", nom_chercheur);

    printf("Prénom : ");
    char prenom[L_NOM + 1];
    scanf("%" STRINGIZE(L_NOM) "s%*c", prenom);

    printf("Code postal : ");
    char code_postal[L_CP + 1];
    scanf("%" STRINGIZE(L_CP) "c%*c", code_postal);

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

    id_chercheur = lu_creer_profil_chercheur(nom_chercheur, prenom, mail, code_postal, competences, collegues);
    printf("Votre identifiant : %zu\n\n", id_chercheur);
}


void modifier_chercheur()
{
    assert(id_chercheur);
    assert(nom_chercheur);

    printf("  * Menu chercheur [%s] *\n\n", nom_chercheur);

    char nom[L_NOM];
    char prenom[L_PRENOM];
    char mail[L_MAIL];
    char code_postal[L_CP];
    char competences[N_COMPETENCES][L_COMPETENCE] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
    size_t id_collegues[N_COLLEGUES] = {0};

    lu_profil_chercheur(id_chercheur, nom, prenom, mail, code_postal, competences, id_collegues);

    printf("Code postal [%.5s] : ", code_postal);
    char cp[L_CP + 1 + 1];
    fgets(cp, L_CP + 1 + 1, stdin);
    cp[strcspn(cp, "\n")] = '\0';
    if(strlen(cp)) strncpy(code_postal, cp, 5);

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

    lu_modifier_profil_chercheur(id_chercheur, code_postal, competences, id_collegues);
}

void supprimer_chercheur()
{
    assert(id_chercheur);
    assert(nom_chercheur);

    printf("  * Menu chercheur [%s] *\n\n", nom_chercheur);

    lu_supprimer_profil_chercheur(id_chercheur);

    printf("Chercheur suppmimé.\n\n");

    id_chercheur = 0;
    nom_chercheur[0] = '\0';
}

action menu_chercheur()
{
    assert(id_chercheur);
    assert(nom_chercheur);

    printf("  * Menu chercheur [%s] *\n\n\
Vous voulez :\n\
1. Modifier votre profil\n\
2. Transitionner vers un profil d'employé\n\
3. Supprimer votre profil\n\n", nom_chercheur);

    return choix(3, (action []){MODIFIER_CHERCHEUR, TRANSITION_EMPLOYE, SUPPRIMER_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
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
            case S_IDENTIFIER_ENTREPRISE:
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
            case S_IDENTIFIER_CHERCHEUR:
                a = identification_chercheur();
                break;
            case CREER_CHERCHEUR:
                creer_chercheur();
                a = NAVIGUER_MENU_CHERCHEUR;
            case NAVIGUER_MENU_CHERCHEUR:
                a = menu_chercheur();
                break;
            case MODIFIER_CHERCHEUR:
                modifier_chercheur();
                a = NAVIGUER_MENU_CHERCHEUR;
                break;
            case SUPPRIMER_CHERCHEUR:
                supprimer_chercheur();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case QUITTER:
            default:
                break;
        }
    }

    void j_fermer();

    return 0;
}

#include "journal/journal.h"
#include "luminin/luminin.h"

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
    MODIFIER_ENTREPRISE,
    SUPPRIMER_ENTREPRISE,
    CREER_POSTE,
    SUPPRIMER_POSTE,
    RECHERCHE_CHERCHEUR
} action;

// actions doit être de taille n + 1 et actions[n] doit décrire l'action à appliquer pour 'r' (menu précédent).
action choix(size_t const n, action const actions[])
{
    char c;
    action a = INVALIDE;
    while(a == INVALIDE)
    {    
        printf("Votre choix [1-%zu],menu [p]rincipal,menu p[r]écédent,[q]uitter : ", n);
        scanf(" %c", &c);

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
    printf("*** Bienvenu sur LuminIn, le site des pros ***\n");
}

action menu_principal()
{
    printf("  * Menu principal *\n\n\
Vous êtes :\n\
1. Une entreprise\n\
2. Un employé\n\
3. À la recherche d'un emploi\n\n");

    return choix(3, (action[]){S_IDENTIFIER_ENTREPRISE, NAVIGUER_MENU_EMPLOYE, NAVIGUER_MENU_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

action identification_entreprise()
{
    printf("  * Menu entreprise *\n\n");

    id_entreprise = 0;
    nom_entreprise[0] = '\0';

    printf("Spécifiez votre identifiant ou, pour créer un nouveau profil d'entreprise, le nom de celle-ci.\n");
    char identifiant_entreprise[L_NOM] = {'\0'};
    while(!id_entreprise)
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

    return INVALIDE;
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

    size_t const identifiant = lu_creer_profil_entreprise(nom_entreprise, code_postal, mail);
    printf("Votre identifiant : %zu\n\n", identifiant);
}

action menu_entreprise()
{
    printf("  * Menu entreprise [%s] *\n\n\
Vous voulez :\n\
1. Modifier le profil de votre entreprise\n\
2. Supprimer le profil de votre entreprise\n\
3. Créer le profil d'un poste à pourvoir\n\
4. Supprimer le profil d'un poste maintenant pourvu\n\
5. Faire une recherche parmi les chercheurs d'emploi\n\n", nom_entreprise);

    return choix(5, (action []){MODIFIER_ENTREPRISE, SUPPRIMER_ENTREPRISE, CREER_POSTE, SUPPRIMER_POSTE, RECHERCHE_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

void modifier_entreprise()
{
    printf("  * Menu entreprise [%s] *\n\n", nom_entreprise);

    printf("Identifiant : ");
    size_t identifiant;
    scanf(" %zu%*c", &identifiant);

    printf("Nom : ");
    char nom[L_NOM];
    fgets(nom, L_NOM, stdin);
    nom[strcspn(nom, "\n")] = '\0';

    printf("Code postal : ");
    char code_postal[L_CP + 1 + 1];
    fgets(code_postal, L_CP + 1 + 1, stdin);
    code_postal[strcspn(code_postal, "\n")] = '\0';

    printf("Mail : ");
    char mail[L_MAIL];
    fgets(mail, L_MAIL, stdin);
    mail[strcspn(mail, "\n")] = '\0';

    lu_modifier_profil_entreprise(identifiant, nom, code_postal, mail);

}

void supprimer_entreprise()
{
    printf("  * Menu entreprise [%s] *\n\n", nom_entreprise);

    lu_supprimer_profil_entreprise(id_entreprise);

    printf("Entreprise supprmiée.\n\n");
}

void creer_poste()
{
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
    printf("  * Menu entreprise [%s]* \n\n", nom_entreprise);

    printf("Identifiant : ");
    size_t identifiant;
    scanf(" %zu%*c", &identifiant);

    lu_supprimer_poste(identifiant);
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
            case MODIFIER_ENTREPRISE:
                modifier_entreprise();
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
            case QUITTER:
            default:
                break;
        }
    }

    void j_fermer();

    return 0;
}

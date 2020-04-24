#include "journal/journal.h"
#include "luminin/luminin.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

char const* const chemin_journal = "./luminin.log";
char const* const chemin_bd = "./bd";

typedef enum action
{
    INVALIDE,
    NAVIGUER_MENU_PRINCIPAL = 'p',
    QUITTER = 'q',
    NAVIGUER_MENU_PRECEDENT = 'r',
    NAVIGUER_MENU_ENTREPRISE,
    NAVIGUER_MENU_EMPLOYE,
    NAVIGUER_MENU_CHERCHEUR,
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

    return choix(3, (action[]){NAVIGUER_MENU_ENTREPRISE, NAVIGUER_MENU_EMPLOYE, NAVIGUER_MENU_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

action menu_entreprise()
{
    printf("  * Menu entreprise *\n\n\
Vous voulez :\n\
1. Créer le profil de votre entreprise\n\
2. Modifier le profil de votre entreprise\n\
3. Supprimer le profil de votre entreprise\n\
4. Créer le profil d'un poste à pourvoir\n\
5. Supprimer le profil d'un poste maintenant pourvu\n\
6. Faire une recherche parmi les chercheurs d'emploi\n\n");

    return choix(6, (action []){CREER_ENTREPRISE, MODIFIER_ENTREPRISE, SUPPRIMER_ENTREPRISE, CREER_POSTE, SUPPRIMER_POSTE, RECHERCHE_CHERCHEUR, NAVIGUER_MENU_PRINCIPAL});
}

void creer_entreprise()
{
    printf("  * Menu entreprise *\n\n");
    
    printf("Nom : ");
    char nom[L_NOM];
    scanf(" %" STRINGIZE(L_NOM) "[^\n]%*c", nom);

    printf("Code postal : ");
    char code_postal[L_CP + 1];
    scanf("%" STRINGIZE(L_CP) "c%*c", code_postal);

    printf("Mail : ");
    char mail[L_MAIL];
    scanf(" %" STRINGIZE(L_MAIL) "[^\n]%*c", mail);

    size_t const identifiant = lu_creer_profil_entreprise(nom, code_postal, mail);
    printf("Votre identifiant : %zu\n\n", identifiant);
}

void modifier_entreprise()
{
    printf("  * Menu entreprise *\n\n");

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
    printf("  * Menu entreprise *\n\n");

    printf("Identifiant : ");
    size_t identifiant;
    scanf(" %zu%*c", &identifiant);

    lu_supprimer_profil_entreprise(identifiant);
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
            case NAVIGUER_MENU_ENTREPRISE:
                a = menu_entreprise();
                break;
            case CREER_ENTREPRISE:
                creer_entreprise();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case MODIFIER_ENTREPRISE:
                modifier_entreprise();
                a = NAVIGUER_MENU_PRINCIPAL;
                break;
            case SUPPRIMER_ENTREPRISE:
                supprimer_entreprise();
                a = NAVIGUER_MENU_PRINCIPAL;
            case QUITTER:
            default:
                break;
        }
    }

    void j_fermer();

    return 0;
}

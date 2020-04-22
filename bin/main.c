#include "journal/journal.h"
#include "luminin/luminin.h"

#include <stdlib.h>
#include <stdio.h>

char const* const chemin_journal = "./luminin.log";
char const* const chemin_bd = "./bd";

typedef enum action
{
    QUITTER = 'q',
    MENU_PRINCIPAL,
    MENU_ENTREPRISE,
    MENU_EMPLOYE,
    MENU_CHERCHEUR,
    CREER_ENTREPRISE,
    SUPPRIMER_ENTREPRISE,
    CREER_POSTE,
    SUPPRIMER_POSTE,
    RECHERCHE_CHERCHEUR
} action;

void en_tete()
{
    printf("*** Bienvenu sur LuminIn, le site des pros ***\n");
}

char choix()
{
    printf("Votre choix ('q' pour quitter) : ");
    char c;
    scanf(" %c", &c);
    return c;
}

action menu_principal()
{
    printf("  * Menu principal *\n\n\
Vous êtes :\n\
1. Une entreprise\n\
2. Un employé\n\
3. À la recherche d'un emploi\n\n");

    switch(choix())
    {
        case '1':
            return MENU_ENTREPRISE;
            break;
        case '2':
            return MENU_EMPLOYE;
            break;
        case '3':
            return MENU_CHERCHEUR;
            break;
        case QUITTER:
            return QUITTER;
            break;
        default:
            break;
    }

    return MENU_PRINCIPAL;
}

action menu_entreprise()
{
    printf("  *Menu entreprise *\n\n\
Vous voulez :\n\
1. Créer le profil de votre entreprise\n\
2. Créer le profil d'un poste à pourvoir\n\
3. Supprimer le profil d'un poste maintenant pourvu\n\
4. Faire une recherche parmi les chercheurs d'emploi\n\
5. Supprimer le profil de votre entreprise\n\n");

    switch(choix())
    {
        case '1':
            return CREER_ENTREPRISE;
            break;
        case '2':
            return CREER_POSTE;
            break;
        case '3':
            return SUPPRIMER_POSTE;
            break;
        case '4':
            return RECHERCHE_CHERCHEUR;
            break;
        case '5':
            return SUPPRIMER_ENTREPRISE;
            break;
        case QUITTER:
            return QUITTER;
            break;
    }

    return MENU_ENTREPRISE;
}

void creer_entreprise()
{
    printf("  * Menu entreprise *\n\n");
    
    printf("Nom : ");
    char nom[128];
    scanf(" %127[^\n]%*c", nom);

    printf("Code postal : ");
    char code_postal[6];
    scanf("%5c%*c", code_postal);

    printf("Mail : ");
    char mail[128];
    scanf(" %127[^\n]%*c", mail);

    size_t const identifiant = lu_creer_profil_entreprise(nom, code_postal, mail);
    printf("Votre identifiant : %zu\n\n", identifiant);
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

    action a = MENU_PRINCIPAL;
    while(a != QUITTER)
    {
        en_tete();

        switch(a)
        {
            case MENU_PRINCIPAL:
                a = menu_principal();
                break;
            case MENU_ENTREPRISE:
                a = menu_entreprise();
                break;
            case CREER_ENTREPRISE:
                creer_entreprise();
                a = MENU_PRINCIPAL;
                break;
            case SUPPRIMER_ENTREPRISE:
                supprimer_entreprise();
                a = MENU_PRINCIPAL;
            case QUITTER:
            default:
                break;
        }
    }

    void j_fermer();

    return 0;
}

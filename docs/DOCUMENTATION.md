# Description des entités

- [Relations entre les entités](#relations-entre-les-entit%C3%A9s)
- [Bibliothèques](#biblioth%C3%A8ques)
    - [libjournal](#libjournal)
    - [libliste](#libliste)
    - [libluminin](#libluminin)
        - [luminin](#luminin)
        - [chercheur, employé, entreprise et poste](#chercheur-employ%C3%A9-entreprise-et-poste)
        - [bd](#bd)
- [Programme de test](#programme-de-test)
- [Application](#application)

## Relations entre les entités

<img src="./Relation entites - diagramme UML.svg">

## Bibliothèques

Il y a trois bibliothèques.
Deux bibliothèques, `libjournal` et `libliste`, offrent des fonctionalités génériques.
Une troisième bibliothèque, `libluminin`, implémente les fonctionalités propres au projet.

- Le nom de toutes les fonctions sont préfixées de deux lettres représentant leurs entité ou couches respective.

### libjournal

Permet de tenir à jour un journal des événements pertinents sous forme de fichier.

- Les notes sont ajoutées à la fin du fichier.
- Les notes sont préfixées de la date et l'heure.
- La signature de la fonction `j_ecrire` calque la signature de la fonction `printf`.

### libliste

Une définition de liste doublement chaînée avec un pointeur `data` de type `void*` et ses fonctions connexes.

### libluminin

Contient les entités définissant les acteurs du programme.

#### luminin

Collection de fonctions constituant l'interface publique de la bibliothèque. L'application ne fait pas appel aux fonctions des autres fichiers.

- Les fonctions `lu_init` et `lu_destroy` doivent être appellées en premier et dernier lieu respectivement.

#### chercheur, employé, entreprise et poste

Collection de fonctions permettant de manipuler les données en lien avec les acteurs respectifs.

- Les fonctions `[ch/em/en/po]_init` et `[ch/em/en/po]_destroy` doivent être appellées en premier et dernier lieu respectivement.
- Les fonctions `[ch/em/en/po]_init` extraient les données de la base de données et les gardent en mémoire dans une liste.
- La base de données est mise à jour à chaque appel de fonction qui modifient le contenu de la liste.
- Les fonctions `[ch/em/en/po]_destroy` libère la mémoire allouée à la liste.

#### bd

Responsable de la lecture et de l'écriture des données persistantes entre les utilisations.

- La base de données est organisée en table.
- À chaque table correspond un fichier.
- Dans une table les données sont séparées par des virgules.
- La première ligne d'une table est une en-tête qui décrit les champs.

## Programme de test

Teste les entités séparément et dans leur ensemble.

- Teste la bibliothèque `libjournal`.
- Teste les fonctions `bd_*` de la bibliothèque `libluminin`.
- Teste les fonctions `[ch/co/em/po]_*` de la bibliothèque `libluminin`.
- Teste les fonctions `lu_*` de la bibliothèque `libluminin`.

## Application

Présente une interface textuelle qui permet aux utilisateurs de gérer leurs profils et effectuer des recherches.

- N'appelle que les fonctions `lu_*`.


# Logiciel-de-gestion-de-versions

./myGit init : initialise le répertoire de références.
./myGit list-refs : affiche toutes les références existantes.
./myGit create-ref <name> <hash> : crée la référence <name> qui pointe vers le commitcorrespondant au hash donné.
./myGit delete-ref <name> : supprime la référence name.
./myGit add <elem> [<elem2> <elem3> ...] : ajoute un ou plusieurs fichiers/répertoires à la zone de préparation (pour faire partie du prochain commit).
./myGit list-add : affiche le contenu de la zone de préparation.
./myGit clear-add : vide la zone de préparation.
.myGit commit <branch name> [-m <message>] : effectue un commit sur une branche, avec ou sans message descriptif.
./myGit get-current-branch : affiche le nom de la branche courante.
./myGit branch <branch-name> : crée une branche qui s’appelle <branch-name> si elle n’existe pas déjà. Si la branche existe déjà, la commande doit afficher un message d’erreur.
./myGit branch-print <branch-name> : affiche le hash de tous les commits de la branche, accompagné de leur message descriptif éventuel. Si la branche n’existe pas, un message d’erreur est affiché.
./myGit checkout-branch <branch-name> : réalise un déplacement sur la branche <branch-name>. Si cette branche n’existe pas, un message d’erreur est affiché.
./myGit checkout-commit <pattern> : réalise un déplacement sur le commit qui commence par <pattern>. Des messages d’erreur sont affichés quand <pattern> ne correspond pas à un seul commit.

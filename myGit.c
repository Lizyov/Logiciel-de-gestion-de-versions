#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include "bib_liste.h"


int main(int argc, char *argv[]){

	if(strcmp(argv[1], "init") == 0){
		if(file_exists(".refs")){
			system("rm -r .refs");
		}
		initRefs();
		initBranch();
	}

	if(strcmp(argv[1], "list-refs") == 0){
		printf ("REFS : \n");
		if(file_exists(".refs")){

			List *L = listdir(".refs");

			for(Cell* ptr = *L;ptr != NULL; ptr = ptr->next){
				if(ptr->data[0] == '.'){
                    continue;
                }
                else{
                    char *content = getRef(ptr->data);
                    printf("- %s \t %s \n", ptr->data,content);
                }
				
			}
		}
	}

	if(strcmp(argv[1], "create-ref") == 0){
		createUpdateRef(argv[2], argv[3]);
	}

	if(strcmp(argv[1], "delete-ref") == 0){
		deleteRef(argv[2]);
	}

	if(strcmp(argv[1], "add") == 0){
		for(int i=2; i < argc; i++){
			myGitAdd(argv[i]);
		}
	}

	if(strcmp(argv[1], "clear-add") == 0){
		system("rm .add");
	}

	if(strcmp(argv[1], "list-add") ==0){
		printf("Zone de preparation : \n");
		if(file_exists(".add")){
			WorkTree *wt = ftwt(".add");
			printf("%s", wtts(wt));
		}
	}

	if(strcmp(argv[1], "commit") == 0){
		if(strcmp(argv[3], "-m") == 0){
			myGitCommit(argv[2], argv[4]);
		}
		else{
			myGitCommit(argv[2], NULL);
		}
	}

	if ( strcmp(argv[1],"get-current-branch") == 0) {
   		printf ( "%s \n" , getCurrentBranch());
    }

    if ( strcmp ( argv [1] , "branch" ) == 0) {

        if(!branchExists(argv[2]))
            createBranch(argv[2]);
        else
            printf ("The branch already exists.");
    }

    if ( strcmp ( argv [1] , "branch-print") == 0){
    
        if (!branchExists(argv[2]))
            printf ( "The branch does not exist.");
        else
            printBranch(argv[2]);
    }

    if (strcmp(argv[1],"checkout-branch") == 0){

        if (!branchExists(argv[2]))
            printf ( "The branch does not exist.");
        else
            myGitCheckoutBranch(argv[2]);
    }

    if ( strcmp(argv[1],"checkout-commit") == 0) {
        myGitCheckoutCommit((char*)argv[2]);
    }

	if (strcmp(argv[1], "merge") == 0) {
		
		List *l = merge(argv[2], argv[3]);
		
		if (l == NULL) {
			printf("La fusion est réalisée avec succes !\n");
			return 0;
		} 
		else {

			
			printf("Des collisions ont été trouvées, veuillez choisir une des options suivantes:\n\n");
			int choix;

			printf("1-Garder les fichiers de la branche courante\n");
			printf("2-Garder les fichiers de la branche %s\n", argv[2]);
			printf("3-Choisir manuellement selon le conflit la branche sur laquelle il souhaite garder le fichier/répertoire qui est en conflit\n");
			scanf("%d", &choix);

			char *current_branch = getCurrentBranch();
			
			if (choix == 1) {
				createDeletionCommit(argv[2], l, "suppression");
				merge(argv[2], argv[3]);
			}
			
			if (choix == 2) {
				createDeletionCommit(current_branch, l, "suppression");
				merge(argv[2], argv[3]);
			}
			
			if (choix == 3) {

				List *l_courant = initList();
				List *l_remote = initList();

				manconflicts(&l_courant, &l_remote, current_branch, argv[2], l);

				createDeletionCommit(current_branch, l_courant, "suppression");
				createDeletionCommit(argv[2], l_remote, "suppression");

				merge(argv[2], argv[3]);
				
			}
		}
			
	}
  
	return 1;
}

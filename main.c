#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "bib_liste.h"
#include <assert.h>

int main(){

    List* ma_liste1 = initList();
    //printf("initialisation OK \n");

    Cell * c1 = buildCell("Hello");
    Cell * c2 = buildCell("From");
    Cell * c3 = buildCell("The");
    Cell * c4 = buildCell("Other");
    Cell * c5 = buildCell("Side");
    //printf("Cellules OK \n");

    insertFirst(ma_liste1,c5);
    insertFirst(ma_liste1,c4);
    insertFirst(ma_liste1,c3);
    insertFirst(ma_liste1,c2);
    insertFirst(ma_liste1,c1);
    //printf("Insertions OK \n");

    char * resultat = ltos(ma_liste1);
    //printf("Resultat OK\n");
    
    printf("---------------------------------\n");

    printf("%s\n",resultat);

    printf("---------------------------------\n");
    
    // Tester la fonction ListGet

    Cell * element2 = listGet(ma_liste1,1);
    assert(element2 == c2);

    // Tester la fonction searchList 

    char * chaine1 = "From";
    char * chaine2 = "Yellow";
    assert(searchList(ma_liste1,chaine1) == c2);
    assert(searchList(ma_liste1,chaine2) == NULL);

    // Tester la fonction stol

    char * phrase = "At|Least|I|Can|Say|That|I|Have|Tried";
    List* ma_liste2 = initList();
    ma_liste2 = stol(phrase);

    affichList(ma_liste2);
    printf("-----------------------------\n");

    // Tester les fonctions ltof et ftol

    ltof(ma_liste1, "HELLO.txt");

    List * ma_liste1_bis = ftol("HELLO.txt");

    affichList(ma_liste1_bis);
    printf("-----------------------------\n");

    // Tester opendir et readdir

    DIR * dp = opendir("test_exo3");

    struct dirent * ep ;
    if ( dp != NULL ){
        
        while ((ep = readdir(dp)) != NULL){
           
            printf("\x5c%s \n",ep->d_name) ;
        }

    }
    printf("---------------------------------\n");

    // Tester listdir 

    affichList(listdir("test_exo3"));
    printf("---------------------------------\n");
    
    // Tester file_exists

    assert(file_exists("HELLO.txt") == 1);

    // Tester cp

    cp("fichier_destination.txt","fichier_source.txt");

    // Tester hashToPath

    char * hachage = sha256file("HELLO.txt");
    //printf("hachage = %s \n", hachage);
    printf("%s \n",hashToPath(hachage));

    blobFile("HELLO.txt");

    // tester getChmod 

    printf("%d \n",getChmod("HELLO.txt"));
    setMode(777, "HELLO.txt");
    printf("nouveaux droits : %d \n",getChmod("HELLO.txt"));

    // tester createWorkfile

    WorkFile * wf1 = createWorkFile("Work_File1");
    printf("Affichage : %s \n", wfts(wf1));

    // Tester wfts et stwf

    WorkFile * wf1_bis = stwf(wfts(wf1));

    // tester initWorkTree

    WorkTree * work_tree = initWorkTree();
    
    // Tester inWorkTree + appendWorkTree 

    assert(appendWorkTree(work_tree, "Work_File1",NULL, 0) == 0); // inserer le work_file1
    assert(inWorkTree(work_tree, "Work_File1") == 0); // vérifier que work_file1 a bien été inséré
    assert(appendWorkTree(work_tree, "Work_File1",NULL, 0) == -4); // on ne peut pas inserer work_file1 car il existe déjà 
    appendWorkTree(work_tree, "Work_File2",NULL, 0);
    appendWorkTree(work_tree, "Work_File3",NULL, 0);
    printf("work_tree : \n%s", wtts(work_tree));

    // Tester stwt

    WorkTree * work_tree_bis = stwt(wtts(work_tree));
    printf("work_tree_bis : \n%s", wtts(work_tree_bis));
    
    // Tester wttf

    wttf(work_tree, "Work_Tree");

    // Tester ftwt

    WorkTree * work_tree_abc = ftwt("Work_Tree_ABC");
    printf("work_tree_abc : \n%s", wtts(work_tree_abc));

    // blobWorkTree

    printf("blobWorkTree : %s\n",blobWorkTree(work_tree));

    // Tester saveWorkTree

    //printf("saveWorkTree : %s\n",saveWorkTree(work_tree_abc,"~/L2-S4/SDD/projet_scv/EXO2/Work_File_ABC"));

    // Tester createKeyVal, freeKeyVal, kvts, stkv

    kvp * paire1 = createKeyVal("message","test_createKeyVal");
    
    printf("resultat de kvts = %s\n",kvts(paire1));

    kvp * paire1_bis = stkv(kvts(paire1));

    printf("resultat de stkv = %s\n",kvts(paire1_bis));

    freeKeyVal(paire1);
    freeKeyVal(paire1_bis);

    // Tester initCommit, commitSet, commitGte, stc, cts, ctf, ftc

    Commit * commit1 = createCommit(blobWorkTree(work_tree));
    commitSet(commit1, "message","test_commitSet");

    printf("resultat commitSet : \n%s",cts(commit1));

    printf("resultat commitGet : \n%s\n",commitGet(commit1,"message"));

    Commit * commit1_bis = stc(cts(commit1));

    printf("resultat stc : \n%s",cts(commit1_bis));

    ctf(commit1,"Commit_1");

    Commit * commit1_ter = ftc("Commit_1");
    
    printf("resultat ftc : \n%s",cts(commit1_ter));

    printf("resultat blobCommit : \n%s\n",blobCommit(commit1));

    // Tester initRefs 

    initRefs(); // on retrouve le repertoire .refs avec la commande : ls -a dans le terminal

    // Tester createUpdateRef et deleteRef

    char * hash_commit1 = blobCommit(commit1);
    //createUpdateRef("test",hash_commit1);

    //deleteRef("test");

    //printf("resultat de getRef : %s \n",getRef("test"));

    // Tester myGitAdd

    createFile("test_myGitAdd");
    createFile("test_myGitAdd_bis");

    myGitAdd("test_myGitAdd");

    // Tester myGitCommit

    createUpdateRef("HEAD",hash_commit1);

    createBranch("test");
    createUpdateRef("test",hash_commit1);

    //myGitCommit("HEAD","test_myGitCommit"); //-------------- ça supprime le .add

    // Tester initBranch + branchExits + createBranch

    initBranch(); // on retrouve le fichier .current_branch dans le repertoire de l'execution
    assert(branchExists("master") == 1);
    
    createBranch("Feature1");

    WorkFile * workcom = createWorkFile("Work_com");

    WorkTree * wtcom  = initWorkTree();
    appendWorkTree(wtcom, "Work_com",NULL, 0);

    Commit * com = createCommit(blobWorkTree(wtcom));

    createUpdateRef("Feature1", blobCommit(com));
    
    printf("current branch : %s\n",getCurrentBranch());
   
    printBranch("HEAD");

    return 1;
}
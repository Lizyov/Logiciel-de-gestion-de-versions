#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "bib_liste.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


List* initList(){
    List* list = (List*)malloc(sizeof(List));
    *list = NULL;
    return list;
}

Cell* buildCell(char* ch){
    Cell* list = (Cell*)malloc(sizeof(Cell)); 
        if (list){
            list->data = strdup(ch);
            list->next = NULL;
        }
    return list;
}

Cell* searchList(List* L, char* str){

    List tmp = *L;
    while(tmp){
        if (strcmp(tmp->data,str) == 0){
            return tmp;
        }
        tmp = tmp->next;
    }
    
    return NULL;
}

void insertFirst(List *L, Cell* C){
    if(L && searchList(L, C->data) == NULL){
        C->next = *L;
        *L = C;
    }
 
}

int listSize(List * L){
    int res = 0;
    List tmp = *L;
    while(tmp){
        res++;
        tmp = tmp->next;
    } 

    return res;
}

char* ctos(Cell* c){
    return c->data;
}

char* ltos(List* L){

    char * res = malloc(sizeof(char)*N*MAX_FILES);
    List tmp = *L;
    while(tmp){
        strcat(res,ctos(tmp));
        tmp = tmp->next;
        if (tmp){
            strcat(res,"|");
        }
    }
    return res;
}

Cell * listGet ( List * L , int i ) {

    Cell * ptr = * L ;
    int k = 0;
    while ( ptr != NULL ) {
        if ( k == i ) break ;
        ptr = ptr->next;
        k++;
        }
    if ( ptr == NULL ) {
        printf ( "INDEX OUT OF RANGE ! \n" ) ;
    }
    return ptr ;
}


List* stol(char* s){
    
    int pos = 0;
    int n_pos = 0;
    int sep = '|' ;
    int size = strlen(s);
    char * tmp ;
    char * result = malloc(sizeof(char)*1000) ;
    List * L = initList () ;

    while ( pos < size ) {
        tmp = strchr(s + pos , sep ) ;
        if ( tmp == NULL )
            n_pos = size + 1;
        else {
        n_pos = tmp - s + 1;
    }
    
    memcpy(result,s+pos,n_pos-pos-1);
    result[n_pos-pos-1]='\0';
    pos = n_pos ;
    insertFirst (L , buildCell ( result ) ) ;
    }
    return L ;
}
    
void ltof(List*L,char*path) {

    FILE * fp = fopen(path,"w") ;
    if ( fp != NULL )
    {
        fputs(ltos(L),fp);
        fclose(fp);
    }
    
}

List * ftol (char*path) {

    char buff[N*MAX_FILES];
    FILE * f = fopen(path,"r") ;

    fgets(buff,N*MAX_FILES,f);

    return stol(buff);
}

void affichList(List *L){

    List tmp = *L;

    while(tmp){
        printf("%s\n",tmp->data);
        tmp = tmp->next;
    }

}

List* listdir(char* root_dir){

    DIR * dp = opendir(root_dir);
    List * res = initList();

    struct dirent * ep ;
    if ( dp != NULL ){
        
        while ((ep = readdir(dp)) != NULL){
            insertFirst(res,buildCell(ep->d_name)) ;
        }

        closedir(dp);

    }
    else{

        perror("Impossible d'ouvrir le dossier !\n");
        return NULL;
    }

    return res;
}

struct stat st = {0};
int file_exists ( char * file ) {
    struct stat buffer ;
    return (stat(file,&buffer) == 0) ;
}

void cp(char *to, char *from){

    if(file_exists(from)){

        FILE * fs = fopen(from,"r");
        FILE * fd = fopen(to,"w");

        char buff[360]; 

        while(fgets(buff,360,fs) != NULL){
            
            fputs(buff,fd);

        }
        
        fclose(fs);
        fclose(fd);

    }
}

int hashFile(char* source, char* dest){
    
    char commande[50]; 
    
    sprintf(commande, "cat %s | sha256sum > %s",source, dest);

	system(commande);

	return 0;
}

char* sha256file(char* file){
    
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[1000];
    strcpy(fname,template);
    int fd = mkstemp(fname);

    char cmd [20000];
    sprintf(cmd,"cat %s|sha256sum>%s",file,fname);
    system(cmd);

    char * hash = malloc(65*sizeof(char));
    FILE * f = fopen(fname,"r") ;
    fgets(hash,65,f);
    hash[64] = '\0';

    char command [10000];
    sprintf ( command , "rm %s " , fname ) ;
    system ( command ) ;

    return hash ;
}

char* hashToPath(char* hash){


    char * path = malloc((strlen(hash)+1)*sizeof(char));
    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    int j = 4;

    for(int i=3; i <strlen(hash);i++, j++){
        path[i] = hash[j];
    }

    return path;
 
}

void blobFile ( char * file ) {
    char * hash = sha256file ( file ) ;
    char * ch2 = strdup ( hash ) ;
    ch2 [2] = '\0';
    if (!file_exists(ch2)){
        char buff[100];
        sprintf(buff,"mkdir%s",ch2);
        system(buff);
    }
    char * ch = hashToPath(hash);
    cp(ch,file);
}

// Exo 4 ==========================================================

int getChmod(const char *path){
	struct stat ret;
	if (stat(path, &ret) == -1){
		return -1;
	}
	return (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|/*owner*/(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|/*group*/(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);/*other*/
}

void setMode(int mode, char *path){
	char buff[100];
	sprintf (buff, "chmod %d %s ", mode, path);
	system (buff);
}

WorkFile *createWorkFile(char *name){
	WorkFile *wf = malloc(sizeof(WorkFile));
	wf->name = strdup(name);
	wf->hash = NULL;
	wf->mode = 0;
	return wf;
}

char *wfts(WorkFile *wf){
	char *ch = malloc(1000*sizeof(char));
	sprintf(ch, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);
	return ch;
}

WorkFile *stwf(char *ch){
	char *name = malloc(sizeof(char)*1000);
	char *hash = malloc(sizeof(char)*1000);
	int mode;
	sscanf(ch, "%s\t%s\t%d", name, hash, &mode);
	WorkFile *wf = createWorkFile(name);
	wf->mode = mode;
	wf->hash = hash;
	return wf;
}

WorkTree *initWorkTree(){
	WorkTree *wt = malloc(sizeof(WorkTree));
	wt->tab = malloc(N*sizeof(WorkFile));
	wt->size = N;
	wt->n = 0;
	return wt;
}

int inWorkTree(WorkTree *wt, char *name){
	for(int i=0; i<wt->n; i++){
		if(strcmp(wt->tab[i].name, name)==0) return i;
	}
	return -1;
}

int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode){
	if(inWorkTree(wt, name) >= 0){
		printf("Name : %s is already in the WT \n", name);
		return -4;
	}
	if(wt->size > wt->n){
		wt->tab[wt->n].mode = mode;
		wt->tab[wt->n].name = strdup(name);
		if(hash != NULL)
			wt->tab[wt->n++].hash = strdup(hash);
		else
			wt->tab[wt->n++].hash = NULL;
		return 0;
	}
    return -1;
}

char *wtts(WorkTree *wt){

	char *ch = malloc(wt->size*1000*sizeof(char));
    ch[0] = '\0';

    for(int i=0; i<wt->n; i++){
      
		strcat(ch, wfts(wt->tab+i));
		strcat(ch, "\n");
	
    }
	
    return ch;
}

WorkTree *stwt(char *ch){
	int pos = 0;
	int n_pos=0;
	int sep = '\n';
	char *ptr;
	char *result = malloc(sizeof(char)*10000);
	WorkTree *wt = initWorkTree();
		while(pos < strlen(ch)){
		ptr = strchr(ch + pos, sep);
		if(ptr == NULL)
			n_pos = strlen(ch)+1;
		else{
			n_pos = ptr - ch + 1;
		}
		memcpy(result, ch+pos, n_pos - pos - 1);
		result[n_pos - pos-1] = '\0';
		pos = n_pos;
		WorkFile *wf = stwf(result);
		appendWorkTree(wt, wf->name, wf->hash, wf->mode);
		}
		return wt;
}

int wttf(WorkTree *wt, char *path){
	FILE *fp = fopen(path, "w");
	if(fp != NULL){
		fputs(wtts(wt), fp);
		fclose(fp);
		return 0;
	}
	return -1;
}

WorkTree * ftwt(char *file){

    char * buff=malloc(sizeof(char)*N);
    FILE * f = fopen(file,"r");
    char * all_wf = malloc(sizeof(char)*N*MAX_FILES);
    all_wf[0] = '\0';
    while(fgets(buff,N,f)!=NULL){
        strcat(all_wf,buff);
    }
    return stwt(all_wf);
}

// Exo 5 ==========================================================

char *hashToFile(char *hash){
	char *ch2 = strdup(hash);
	ch2[2] = '\0';
	if(stat(ch2, &stat) == -1){
		mkdir(ch2, 0700);
	}
	return hashToPath(hash);
}

char * blobWorkTree(WorkTree *wt){

	char fname[100] = "/tmp/myfileXXXXXX";
	int fd = mkstemp(fname);
	wttf(wt, fname);
	char *hash = sha256file(fname);

	char *ch = hashToFile(hash);
   
    strcat(ch, ".t");
 
	cp(ch, fname);
    
	return hash;
}

char *concat_paths(char *path1, char *path2){
	char *result = malloc(strlen(path1) + strlen(path2) + 1);
	if(result == NULL){
		printf("Error: unable to allocate memory\n");
		return NULL;
	}
	strcpy(result, path1);// Copy path1 to result
	strcat(result,"/");// Append a slash to result
	strcat(result ,path2); // Append path2 to result
	return result;
}

int isFile(const char* name){

    DIR* directory = opendir(name);

    if(directory != NULL){
        closedir(directory);
        return 0;
    }

    if(errno == ENOTDIR){
        return 1;
    }

    return -1;
}

char *saveWorkTree(WorkTree *wt, char *path){
	for(int i=0; i < wt->n; i++){
		char *absPath = concat_paths(path, wt->tab[i].name);
		if(isFile(absPath) == 1){
			blobFile(absPath);
			wt->tab[i].hash = sha256file(absPath);
			wt->tab[i].mode = getChmod(absPath);
		}
		else{
			WorkTree *wt2 = initWorkTree();
			List *L = listdir(absPath);
			for(Cell *ptr = *L; ptr != NULL; ptr = ptr->next){
				if(ptr->data[0] == '.')
					continue;
				appendWorkTree(wt2, ptr->data, NULL, 0);
			}
			wt->tab[i].hash = saveWorkTree(wt2, absPath);
			wt->tab[i].mode = getChmod(absPath);
		}
	}
	return blobWorkTree(wt);
}

int isWorkTree(char *hash){
	if(file_exists(strcat(hashToPath(hash), ".t"))){
	return 1;
	}
	if(file_exists(hashToPath(hash))){
	return 0;
	}
	return -1;
}

void restoreWorkTree(WorkTree *wt, char *path){
	for(int i=0; i < wt->n; i++){
		char *absPath = concat_paths(path,wt->tab[i].name);
		char *copyPath = hashToPath(wt->tab[i].hash);
		char *hash = wt->tab[i].hash;
		if(isWorkTree(hash) == 0){//si c’est un fichier
            cp(absPath, copyPath);
            setMode(getChmod(copyPath), absPath);
		}
		else{
			if(isWorkTree(hash) == 1){//si c’est un repertoire
				strcat(copyPath, ".t") ;
				WorkTree *nwt = ftwt(copyPath);
				restoreWorkTree(nwt, absPath);
				setMode(getChmod(copyPath), absPath);
			}
		}
	}
}

// Exo 6 ==========================================================

kvp *createKeyVal(char *key, char *val){
	kvp *k = malloc(sizeof(kvp));
	k->key = strdup(key);
	k->value = strdup(val);
	return k;
}

void freeKeyVal(kvp *kv){
	free(kv->key);
	free(kv->value);
	free(kv);
}

char *kvts(kvp *k){
	char *buff = malloc(sizeof(char)*100);
	sprintf(buff,"%s : %s", k->key, k->value);
	return buff;
}

kvp *stkv(char *str){
	char key[100], val[100];
    //printf("str = %s\n",str);
	sscanf(str, "%s : %s", key, val);
    //printf("key = %s\n",key);
    //printf("val = %s\n",val);
	return createKeyVal(key, val);
}

Commit *initCommit(){
	Commit *c = malloc(sizeof(Commit));
	c->T = malloc(Ncommit*sizeof(kvp*));
	c->size = Ncommit ;
	for(int i=0; i < c->size; i++){
		c->T[i] = NULL;
	}
	c->n = 0;
	return c;
}

unsigned long hashF(unsigned char *str){

    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;

}

void commitSet(Commit *c, char *key, char *value){
	int p = (hashF(key)) % (c->size);
	while(c->T[p] != NULL){
		p = (p+1) % (c->size);//probing lineaire
	}
	c->T[p] = createKeyVal(key, value);
	c->n++;
}

Commit *createCommit(char *hash){
	Commit *c = initCommit();
	commitSet(c,"tree", hash);
	return c;
}

char *commitGet(Commit *c, char *key) {
  int i = 0;
  int cle;
  while (i < c->size) {
    cle = (hashF(key) + i) % c->size;
    if (c->T[cle] != NULL) {
      if (strcmp((c->T[cle])->key, key) == 0) {
        return (c->T[cle])->value;
      }
    }
    i++;
  }
  return NULL;
}

char *cts(Commit *c){
	char *str = malloc(sizeof(char)*100*(c->n));
    str[0]='\0';
	for(int i=0; i < c->size; i++){
		if(c->T[i] != NULL){
            strcat(str, kvts(c->T[i]));
            strcat(str, "\n");
		}
	}

	return str;
}

Commit *stc(char *ch){
	int pos = 0;
	int n_pos = 0;
	int sep = '\n';
	char *ptr;
	char *result = malloc(sizeof(char)*10000);
	Commit *c = initCommit(10);
	while(pos < strlen(ch)){
		ptr = strchr(ch + pos, sep);
		if(ptr == NULL)
			n_pos = strlen(ch)+1;
		else
			n_pos = ptr - ch + 1;
		memcpy(result, ch + pos, n_pos - pos - 1);
		result[n_pos-pos-1] = '\0';
		pos = n_pos;
		kvp *elem = stkv(result);
		commitSet(c ,elem->key, elem->value);
	}
	return c;
}

Commit * ftc (char * file) {
    char buff [MESSAGE_SIZE];
    char * all = malloc(sizeof(char) * MESSAGE_SIZE);
    all[0] = '\0';
    FILE * f = fopen(file, "r");
    printf("%s\n",file);
    if (f == NULL) {
        printf("le fichier %s n'existe pas\n", file);
        return NULL;
    }
    while(fgets(buff, N, f) != NULL) {
        strcat(all, buff);
    }
    Commit * c = stc(all);
    return c;
}

void ctf(Commit *c, char *file){
	FILE *fp = fopen(file, "w");
	if(fp != NULL){
		fputs(cts(c), fp);
		fclose(fp);
	}
}

char *blobCommit(Commit *c){
	char fname[100] = "/tmp/myfileXXXXXX";
	int fd = mkstemp(fname);
	ctf(c, fname);
	char *hash = sha256file(fname);
	char *ch = hashToFile(hash);
	strcat(ch, ".c");
	cp(ch, fname);
	return hash;
}


// Exo 7 =============================================

void initRefs(){
	if(!file_exists(".refs")){
		system("mkdir .refs");
		system("touch .refs/master");
		system("touch .refs/HEAD");
	}
}

void createUpdateRef(char *ref_name, char *hash) {
  char file[256];
  sprintf(file, ".refs/%s", ref_name);
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    printf("Could not open the file %s\n", file);
  }
  fprintf(f, "%s", hash);
  fclose(f);
}

void deleteRef(char *ref_name){
	char buff[256];
    sprintf(buff,"rm .refs/%s", ref_name);
    system(buff);
	
}

char *getRef(char *ref_name) {
  
    char file[256];
    sprintf(file, ".refs/%s", ref_name);

    FILE *fp;
    fp = fopen(file, "r");

    if (fp == NULL){
        printf("Could not open the file %s\n", ref_name);
        return NULL;
    }
    else{

        char buff[256];
        char *lecture = fgets(buff, 256, fp);

        if (lecture == NULL) {
            char *ref = (char *)malloc(sizeof(char));
            ref[0] = '\0';
            fclose(fp);
            return ref;
        }
        else{

            char *ref = (char *)malloc(sizeof(char) * 256);
            strcpy(ref, buff);
            fclose(fp);
            return ref;
            }
    }

  
  
}

void createFile (char *file){
	char buff[100];
	sprintf(buff, "touch %s", file);
	system(buff);
}
	
void myGitAdd(char *file_or_folder){
	WorkTree *wt;
	if(!file_exists(".add")){
		createFile(".add");
		wt = initWorkTree();
	}
	else{
		wt = ftwt(".add");
	}
	if(file_exists(file_or_folder)){
		appendWorkTree(wt, file_or_folder, NULL, 0);
		wttf(wt, ".add");
	}
	else{
		printf("file or folder %s does not exist\n", file_or_folder);
	}
}

void myGitCommit (char *branch_name, char *message){

	if(!file_exists(".refs")){
		printf("Il faut d’abord initialiser les references du projets");
		return;
	}
  
	char buff[256];
	sprintf(buff,".refs/%s", branch_name);
	if(!file_exists(buff)){
		printf("La branche n’existe pas");
		return;
	}
	char *last_hash = getRef(branch_name);
	char *head_hash = getRef("HEAD");
	if(strcmp(last_hash, head_hash) !=0){
		printf("HEAD doit pointer sur le dernier commit de la branche");
		return;
	}
	WorkTree *wt = ftwt(".add");
	char *hashwt = saveWorkTree(wt, ".");
	Commit *c = createCommit(hashwt);
	if(strlen(last_hash) !=0){
		commitSet(c, "predecessor", last_hash);
	}
	if(message != NULL){
		commitSet(c, "message", message);
	}
	char *hashc = blobCommit(c);
	createUpdateRef(branch_name, hashc);
	createUpdateRef("HEAD", hashc);
	system("rm .add");
}



// Exo 8 ==========================================================

void initBranch(){

    FILE * f = fopen(".current_branch","w") ;
    fputs("master",f);
    fclose(f) ;
}

int branchExists(char* branch){

    List * refs = listdir(".refs");
    return searchList(refs,branch) != NULL ;

}

void createBranch(char *branch) {

  char *hash = getRef("HEAD");
  
  if (hash == NULL) { 
    printf("HEAD does not exist\n");
    return;
  }
  else{

    if (hash[0] == '\0') {
        free(hash);
        printf("HEAD is empty !\n");
        return;
    }
    else{
        createUpdateRef(branch, hash);
    }
  }

}

char * getCurrentBranch(){

    FILE * f = fopen(".current_branch","r");
    char * buff = malloc ( sizeof ( char ) *100) ;
    fscanf(f,"%s",buff);
    return buff ;
}

char * hashToPathCommit ( char * hash ) {
    char * buff = (char*)malloc(sizeof(char)*100);
    buff = strcpy(buff,hashToPath(hash));
    buff = strcat(buff,".c");
    return buff ;

}

void printBranch(char* branch) {
    char* commit_hash = getRef(branch);
    Commit *c = ftc(hashToPathCommit(commit_hash));

    while (c != NULL) {
        if (commitGet(c, "message") != NULL) {
            printf("%s -> %s\n", commit_hash, commitGet(c, "message"));
        } else {
            printf("%s\n", commit_hash);
        }

        
        if (commitGet(c, "predecessor") != NULL) {
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash));
        } else {
            c = NULL;
        }
    }
}

List * branchList ( char * branch ) {

    List * L = initList () ;
    char * commit_hash = getRef ( branch ) ;
    char * hash_to_path_commit = hashToPathCommit(commit_hash);
    Commit * c = ftc (hash_to_path_commit) ;
    while ( c != NULL ) {
        insertFirst (L , buildCell ( commit_hash ) ) ;
        if (commitGet(c,"predecessor") != NULL){
            commit_hash = commitGet(c,"predecessor");
            c = ftc(hashToPathCommit(commit_hash));
        }else{
            c = NULL ;
        }
    }

    return L;
}

List * getAllCommits(){

    List * L = initList () ;
    List * content = listdir(".refs");

    for ( Cell * ptr = * content ; ptr != NULL ; ptr = ptr->next ){
        
        if ( ptr->data[0] == '.' ) continue ;
        List * list = branchList(ptr->data);
        Cell * cell = * list ;
        while ( cell != NULL ){
            if (searchList(L,cell->data) == NULL){
                insertFirst(L,buildCell(cell->data));
            }
            cell = cell->next;
        }
    }
    
    return L ;
}


// Exo 9 ======================================================

void restoreCommit(char * hash_commit){

    char * commit_path = hashToPathCommit(hash_commit) ;
    Commit * c = ftc(commit_path);
    char * tree_hash = strcat(hashToPath(commitGet(c,"tree")),".t");
    WorkTree * wt = ftwt(tree_hash);
    restoreWorkTree(wt,".");

}

void myGitCheckoutBranch(char * branch ){
    //Change current_branch :

    FILE * f = fopen(".current_branch","w");
    fprintf(f,"%s",branch);
    fclose(f);

    char * hash_commit = getRef(branch);
    createUpdateRef("HEAD",hash_commit);
    restoreCommit(hash_commit);
}

List * filterList(List* L,char* pattern){

    List * filtered = initList() ;
    int taille_pattern = strlen(pattern);
    for(Cell* ptr = *L; ptr != NULL; ptr = ptr->next ){

        char * c = strdup(ptr->data);
        c[strlen(pattern)]='\0';
        if(strncmp(c,pattern,taille_pattern) == 0){
            insertFirst(filtered, buildCell(ptr->data));
        }
        free(c);
    }

    return filtered ;
}

void myGitCheckoutCommit(char * pattern){
    List * L = getAllCommits();
    List * filtred_list = filterList(L,pattern);
    if ( listSize ( filtred_list ) == 1) {
        char * commit_hash = (listGet(filtred_list,0))->data;
        createUpdateRef ("HEAD",commit_hash);
        restoreCommit( commit_hash ) ;
    }else{
        if (listSize(filtred_list) == 0){
        printf("No pattern matching. \n");
        }else{
            printf("Multiple matchings found: \n");
            for(Cell* ptr = *filtred_list; ptr != NULL; ptr = ptr->next){
                printf("-> %s \n",ptr->data);
            }
        }
    }
}

// Exo 11 ======================================================

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts){

    WorkFile * tmp1 = wt1->tab;
    WorkFile * tmp2 = wt2->tab;
    WorkTree * wt_res = initWorkTree();
    int conflit = 0;

    // On parcourt d'abord wt1 pour récupérer les fichiers/répértoires qui ne sont pas dans wt2
    for (int i = 0; i < wt1->n; i++) {
        conflit = 0;
        for (int j = 0; j < wt2->n; j++) {

            if ((strcmp(wt2->tab[j].name, wt1->tab[i].name) == 0)) {
                insertFirst(*conflicts, buildCell(wt1->tab[i].name));
                conflit = 1;
                break;
            }
        }
        if (conflit == 1) {
            appendWorkTree(wt_res, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
        }
    }
    // On parcourt ensuite wt2 pour récupérer les fichiers/répértoires qui ne sont pas dans wt1

    for (int i = 0; i < wt2->n; i++) {
        conflit = 0;
        for (int j = 0; j < wt1->n; j++) {

            if ((strcmp(wt1->tab[j].name, wt2->tab[i].name) == 0)) {
                insertFirst(*conflicts, buildCell(wt2->tab[i].name));
                conflit = 1;
                break;
            }
        }
        if (conflit == 1) {
            appendWorkTree(wt_res, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);
        }
    }

    return wt_res;

}

WorkTree *btwt(char *branch) {

  char buff[256];
  char *hash_commit = getRef(branch);

  if (hash_commit == NULL) {
    return NULL;
  }
 
  Commit *c = ftc(hashToPathCommit(hash_commit));
  // On récupère le hash du WorkTree et construit le chemin associé
  char *hash_worktree = commitGet(c, "tree");
  char *path_worktree = hashToPath(hash_worktree);
  strcpy(buff, path_worktree);
  strcat(buff, ".t");
  
  return ftwt(buff);
}

List* merge(char* remote_branch, char* message){

    List * tmp = initList();
    WorkTree * wt_courant = btwt(getCurrentBranch());
    WorkTree * wt_remote = btwt(remote_branch);
    WorkTree * wt_merge = mergeWorkTrees(wt_courant,wt_remote,&tmp);

    if(*tmp != NULL){
        return tmp;
    }
    char *hash_courant = getRef(getCurrentBranch());
    char *hash_remote = getRef(remote_branch);

    Commit * c = createCommit(blobWorkTree(wt_merge));
    commitSet(c, "predecessor", hash_courant);
    commitSet(c, "merged_predecessor", hash_remote);
    commitSet(c,"message", message);

    char * hash_c = blobCommit(c);
    createUpdateRef(getCurrentBranch(),hash_c );
    createUpdateRef("HEAD", hash_c);
    deleteRef(remote_branch);
    restoreWorkTree(wt_merge, ".");

    return NULL;

}

void createDeletionCommit(char* branch, List* conflicts, char* message){

    char *current_branch = getCurrentBranch();

    // On se déplace sur la branche en question
    myGitCheckoutBranch(branch);
    // On récupère le dernier commit de cette branche, et le worktree associé
    WorkTree *wt_branch = btwt(branch);
    if (wt_branch == NULL) return;

    // On vide la zone de préparation .add
    
    FILE *fadd = fopen(".add", "w");

    fclose(fadd);

    // On ajoute les fichiers/répertoires du worktree qui ne font pas partie de la liste des conflits

    for (int i = 0; i < wt_branch->n; i++) {
        if (searchList(conflicts, wt_branch->tab[i].name) == NULL) {
            myGitAdd(wt_branch->tab[i].name);
        }
    }
  
    // On crée le commit de suppression
    myGitCommit(branch, message);
  
    // On revient sur la branche de départ
    myGitCheckoutBranch(getCurrentBranch());
    
}

void manconflicts(List **l_courant, List **l_remote, char *courant, char *remote, List *l) {

    Cell *tmp = *l;
    
    while (tmp) {

            printf("Veuillez choisir le numéro de la branche où vous souhaitez que %s soit supprimé\n",tmp->data);
            int choix;

            printf("%s ----------- Choisir 1\n", courant);
            printf("%s ----------- Choisir 2\n", remote);
            scanf("%d", &choix);

            if (choix == 1) {
            insertFirst(*l_courant, buildCell(tmp->data));
            tmp = tmp->next;
            } 
            else{
            insertFirst(*l_remote, buildCell(tmp->data));
            tmp = tmp->next;
        
            }
    }

}

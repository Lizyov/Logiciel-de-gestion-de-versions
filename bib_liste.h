#define N 100 //taille_work_tree
#define MAX_FILES 1000
#define Ncommit 10 // nombre_informations_dans_un_commit
#define MESSAGE_SIZE 1000


typedef struct cell{
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

List* initList();
Cell* buildCell(char* ch);
void insertFirst(List *L, Cell* C);
int listSize(List * L);
char* ctos(Cell* c);
char* ltos(List* L);
Cell* listGet(List* L, int i);
Cell* searchList(List* L, char* str);
List* stol(char* s);
void ltof(List* L, char* path);
List* ftol(char* path);
void affichList(List *L);
List* listdir(char* root_dir);
int file_exists(char *file);
void cp(char *to, char *from);
char* hashToPath(char* hash);
int hashFile(char* source, char* dest);
char* sha256file(char* file);
void blobFile ( char * file );

// Exercice 4 ==========================================

typedef struct{
	char *name;
	char *hash;
	int mode;
} WorkFile;

typedef struct{
	WorkFile *tab;
	int size; // nombre maximum de fichiers et repertoires que peut contenir le WorkTree
	int n; // nombre de fichiers et repertoires présents dans le WorkTree
} WorkTree;

int getChmod(const char *path);

void setMode(int mode, char *path);

WorkFile *createWorkFile(char *name);
char *wfts(WorkFile *wf);
WorkFile *stwf(char *ch);
WorkTree *initWorkTree();
int inWorkTree(WorkTree *wt, char *name);
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode);
char *wtts(WorkTree *wt);
WorkTree *stwt(char *ch);
int wttf(WorkTree *wt, char *path);
WorkTree *ftwt(char *file);

// Exercice 5 ==========================================

char *hashToFile(char *hash);
char *blobWorkTree(WorkTree *wt);
char *concat_paths(char *path1, char *path2);
int isFile(const char* name);
char *saveWorkTree(WorkTree *wt, char *path);
int isWorkTree(char *hash);
void restoreWorkTree(WorkTree *wt, char *path);

// Exercice 6 ==========================================

typedef struct key_value_pair{
char *key;
char *value;
} kvp;

typedef struct hash_table{
kvp **T;
int n;
int size;
} HashTable;

typedef HashTable Commit;

kvp *createKeyVal(char *key, char *val);
void freeKeyVal(kvp *kv);
char *kvts(kvp *k);
kvp *stkv(char *str);
Commit *initCommit();
void commitSet(Commit *c, char *key, char *value);
unsigned long hashF(unsigned char *str);
Commit *createCommit(char *hash);
char *commitGet(Commit *c, char *key);
char *cts(Commit *c);
Commit *stc(char *ch);
Commit *ftc(char *file);
void ctf(Commit *c, char *file);
char *blobCommit(Commit *c);

// Exercice 7 =================================================

void initRefs();
void createUpdateRef(char *ref_name, char *hash);
void deleteRef(char *ref_name);
char *getRef(char *ref_name);
void createFile (char *file);
void myGitAdd(char *file_or_folder);
void myGitCommit (char *branch_name, char *message);

// Exercice 8 =================================================

void initBranch(); 
int branchExists ( char * branch );
void createBranch(char * branch);
char * getCurrentBranch();
void printBranch (char * branch);
List * branchList ( char * branch );
List * getAllCommits();


// Exercice 9 =================================================

void restoreCommit(char * hash_commit);
void myGitCheckoutBranch(char * branch);
List * filterList(List* L,char* pattern);
void myGitCheckoutCommit(char * pattern);

// Exercice 11 ================================================

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);
WorkTree *btwt(char *branch);
List* merge(char* remote_branch, char* message);
void createDeletionCommit(char* branch, List* conflicts, char* message);
void manconflicts(List **l_courant, List **l_remote, char *courant, char *remote, List *l);


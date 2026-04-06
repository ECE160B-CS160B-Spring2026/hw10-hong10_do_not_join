#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define HASHSIZE 101

struct nlist {              // table entry:
    struct nlist *next;     // next entry in chain
    char *name;             // defined name
    int  defn;              // replacement text
};

struct tnode {           // the tree node:
    int count;           // number of occurrences
    char *word;          // points to the text
    struct tnode *left;  // left child
    struct tnode *right; // right child
};

struct tnode *talloc(void);
char *mystrdup(char *);
struct tnode *addtree(struct tnode *, int, char *);
void treeprint(struct tnode *);
int getword(char *, int);

static struct nlist *hashtab[HASHSIZE]; // pointer table
struct nlist *lookup(char *);

// hash: form hash value for string
unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

// lookup:look for s in hashtab
struct nlist *lookup(char *s)
{
    struct nlist *np;

    // loop through linked list
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; // found

    return NULL;       // not found
}

// install:put (name, defn) in hashtab
struct nlist *install(char *name, int defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { // not found
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } 

    np->defn = defn;

    return np;
}

// K&R pg. 140-143
// addtree: add a node with w, at or below p
struct tnode *addtree(struct tnode *p, int count, char *w)
{
    int cond;
    if (p == NULL) {    // a new word has arrived
        p = talloc();   // make a new node
        p->word = mystrdup(w);
        p->count = count;
        p->left = p->right = NULL;
    } else {
        cond = count < p->count ? -1 : 1;
        if (cond < 0) {
            p->left = addtree(p->left, count, w);
        }
        else if (cond > 0) {
            p->right = addtree(p->right, count, w);
        }
    }
    /*
    }if ((cond = strcmp(w, p->word)) == 0)
        p->count++;     // repeated word
    else if (cond < 0)  // less than into left subtree
        p->left = addtree(p->left, w);
    else                // greater than into right subtree
        p->right = addtree(p->right, w);
    */
    return p;
}
// treeprint: in-order print of tree p
void treeprint(struct tnode *p)
{
    if (p != NULL) {
        treeprint(p->right);
        printf("%d %s\n", p->count, p->word);
        //printf("%s\n", p->word);
        treeprint(p->left);
    }
}

// talloc: make a tnode
struct tnode *talloc(void)
{
    return (struct tnode *) malloc(sizeof(struct tnode));
}

char *mystrdup(char *s) // make a duplicate of s
{
    char *p;
    p = (char *) malloc(strlen(s)+1); // +1 for '\0'
    if (p != NULL)
        strcpy(p, s);
    return p;
}

int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;
    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}
// K&R pg. 79
#define BUFSIZE 100
char buf[BUFSIZE]; // buffer for ungetch
int bufp = 0; // next free position in buf
int getch(void) // get a (possibly pushed back) character
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}
void ungetch(int c) // push character back on input
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many caracters\n");
    else
        buf[bufp++] = c;
}


// word frequency count
int main()
{
    //printf("MAIN\n");
    struct tnode *root;
    char word[MAXWORD];
    root = NULL;
    // add word to hash map

    while (getword(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            // root = addtree(root, word);
            //printf("%s\n", word);
            //printf("%d\n", lookup(word)==NULL);
            if (lookup(word) == NULL) {
                install(word, 1);
            } else {
                struct nlist *n = lookup(word);
                int count = n->defn;
                install(word, count+1);
            }
            //printf("word: %s %d\n", word, lookup(word)->defn);
        }
    }

    //printf("READING MAP\n");

    // add count->word to tree by looping thru all possible keys
    for (int i=0; i<HASHSIZE; i++) {
        struct nlist *node = hashtab[i];
        if(node != NULL) {
            char *word = node->name;
            //printf("word: %s %d\n", word, lookup(word)->defn);
        }
    }

    for (int i=0; i<HASHSIZE; i++) {
        struct nlist *node = hashtab[i];
        if(node != NULL) {
            char *word = node->name;
            int count = lookup(word)->defn;
            //printf("word: %s %d\n", word, lookup(word)->defn);
            root = addtree(root, count, word);
        }
    }
    
    treeprint(root);
    return 0;
}

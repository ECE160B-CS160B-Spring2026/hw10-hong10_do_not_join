#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// K&R Pg. 144-145

struct nlist {              // table entry:
    struct nlist *next;     // next entry in chain
    int value;              // value in the entry
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; // pointer table

// hash: form hash value for string
unsigned hash(int val)
{
    return abs(val % HASHSIZE);
}

// lookup:look for s in hashtab
struct nlist *lookup(int val)
{
    struct nlist *np;

    // loop through linked list
    for (np = hashtab[hash(val)]; np != NULL; np = np->next)
        if (val == np->value)
            return np; // found

    return NULL;       // not found
}

// install:put (name, defn) in hashtab
struct nlist *install(int val)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(val)) == NULL) { // not found
        np = (struct nlist *) malloc(sizeof(*np));
	np->value = val;
        if (np == NULL)
            return NULL;
        hashval = hash(val);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }

    return np;
}

int intersection(int output[], int array1[], int lenArr1, int array2[], int lenArr2) {
	int ii = 0;
	for (int i = 0; i < lenArr1; i++)
		install(array1[i]);
	for (int j = 0; j < lenArr2; j++) {
		if (lookup(array2[j]) != NULL){
			output[ii] = array2[j];
			ii++;
		}
	}
	return ii;
}

#define MAXSIZE 255



int main()
{
    int array1[MAXSIZE];
    int array2[MAXSIZE];
    int len1, len2;
    int array3[MAXSIZE];
    scanf("%d", &len1);
    for (int i=0; i<len1; i++) {
        scanf("%d", &array1[i]);
    }
    scanf("%d", &len2);
    for (int i=0; i<len2; i++) {
        scanf("%d", &array2[i]);
    }
    int len3 = intersection(array3, array1, len1, array2, len2);

    for (int i = 0; i < len3; i++) {
	    printf("%d ", array3[i]);
    }
    printf("\n");

    return 0;
}

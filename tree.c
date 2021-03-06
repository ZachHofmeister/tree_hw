//
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "utils.h"
#include "queue.h"

//-------------------------------------------------
tnode* tnode_create(const char* word) {
    tnode* p = (tnode*)malloc(sizeof(tnode));
    p->word = strdup(word);    // copy of word allocated on heap
    p->count = 1;
    p->left = NULL;
    p->right = NULL;
    
    return p;
}

void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

tree* tree_create(void) {
    tree* p = (tree*)malloc(sizeof(tree));
    p->root = NULL;
    p->size = 0;
    
    return p;
}

static void tree_deletenodes(tree* t, tnode* p) {
    if (p == NULL) { return; }
    
    tree_deletenodes(t, p->left);
    tree_deletenodes(t, p->right);
    tnode_delete(p);
    t->size--;
}

void tree_clear(tree* t) {
    tree_delete(t);
    t->root = NULL;
    t->size = 0;
}

void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

bool tree_empty(tree* t) { return t->size == 0; }
size_t tree_size(tree* t) { return t->size; }

static tnode* tree_addnode(tree* t, tnode** p, const char* word) {
    int compare;
    
    if (*p == NULL) {
        *p = tnode_create(word);
    } else if ((compare = strcmp(word, (*p)->word)) == 0) {
        (*p)->count++;
    } else if (compare < 0) { tree_addnode(t, &(*p)->left, word);
    } else {
        tree_addnode(t, &(*p)->right, word);
    }

    return *p;
}

tnode* tree_add(tree* t, const char* word) {
    tnode* p = tree_addnode(t, &(t->root), word);
    t->size++;

    return p;
}

static void tree_printme(tree* t, tnode* p) {
    printf("%s", p->word);
    if (p->count > 1) { printf(" -- %d", p->count); }
    printf("\n");
}

static void tree_printnodes(tree* t, tnode* p) {
    if (p == NULL) { return; }
    
    tree_printnodes(t, p->left);
    tree_printme(t, p);
    tree_printnodes(t, p->right);
}

void tree_print(tree* t) {    // INORDER-printing
    tree_printnodes(t, t->root);
    printf("\n");
}

static void tree_printnodes_preorder(tree* t, tnode* p) {
    if (p == NULL) { return; }
    
    tree_printme(t, p);
    tree_printnodes(t, p->left);
    tree_printnodes(t, p->right);
}

void tree_print_preorder(tree* t) {
    tree_printnodes_preorder(t, t->root);
    printf("\n");
}

static void tree_printnodes_postorder(tree* t, tnode* p) {
    if (p == NULL) { return; }
    
    tree_printnodes(t, p->left);
    tree_printnodes(t, p->right);
    tree_printme(t, p);
}

void tree_print_postorder(tree* t) {
    tree_printnodes_postorder(t, t->root);
    printf("\n");
}

static void tree_printnodes_levelorder(tree* t, queue * q) {
    tree_printme(t, queue_front(q));
    if (queue_front(q)->left != NULL) queue_push(q, queue_front(q)->left);
    if (queue_front(q)->right != NULL) queue_push(q, queue_front(q)->right);
    queue_pop(q);
    if (!queue_empty(q)) tree_printnodes_levelorder(t, q);
}

void tree_print_levelorder(tree* t) {
    if (t->root == NULL) return;
    queue * queue = queue_create();
    queue_push(queue, t->root);
    tree_printnodes_levelorder(t, queue);
    printf("\n");
}

void tree_test() {
    tree* t = tree_create();
    tree_add(t, "now");
    tree_add(t, "is");
    tree_add(t, "the");
    tree_add(t, "time");
    tree_add(t, "for");
    tree_add(t, "all");
    tree_add(t, "good");
    tree_add(t, "men");
    tree_add(t, "and");
    tree_add(t, "women");
    tree_add(t, "to");
    tree_add(t, "come");
    tree_add(t, "to");
    tree_add(t, "the");
    tree_add(t, "aid");
    tree_add(t, "of");
    tree_add(t, "their");
    tree_add(t, "country");

    tree_print(t);
    printf("is my tree empty? %s\n", yesorno(tree_empty(t)));
    printf("size of tree: %zu\n\n", tree_size(t));
    
    tree_clear(t);
    printf("is my tree empty now? %s\n", yesorno(tree_empty(t)));
    printf("size of tree: %zu\n\n", tree_size(t));
}

tree * inputToTree(FILE * fin) {
    int c;
    char word[128];
    memset(word, 0, sizeof(word));
    char * wordp = word;
    tree * t = tree_create();

    while ((c = (fin == NULL? getchar() : fgetc(fin))) != EOF) {
        if (isalpha(c)) *wordp++ = c;
        else if (wordp != word) {
            tree_add(t, word);
            memset(word, 0, sizeof(word));
            wordp = word;
        }
    }

    return t;
}

int main(int argc, const char* argv[]) {
    // tree_test();

    tree * t = tree_create();
    if (argc > 2) { //Too many arguments
        fprintf(stderr, "Usage: %s (file).", *argv);
        exit(1);
    } else if (argc == 2) { //File input
        FILE * fin = fopen(argv[1], "r");
        if (fin == NULL) {
            fprintf(stderr, "Error opening file \"%s\".", argv[1]);
            exit(1);
        }
        t = inputToTree(fin);
    } else { //Console input
        t = inputToTree(NULL);
        printf("\n");
    }
    printf("Tree inorder:\n");
    tree_print(t);
    printf("Tree levelorder:\n");
    tree_print_levelorder(t);
    
    return 0;
}


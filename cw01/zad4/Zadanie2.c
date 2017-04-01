#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Tree.h"

Tree * initTree(void) {
	Tree * result = malloc(sizeof(Tree));
	result->root = NULL;
	return result;
}

void deleteSubTree(PersonT * node) {
	if (node == NULL) return;
	deleteSubTree(node->left);
	deleteSubTree(node->right);
	free(node);
}

void deleteTree(Tree * Tree) {

	PersonT* p = Tree->root;
	if (p->right != NULL) {
		deleteSubTree(p->right);
	}
	if (p->left != NULL) {
		deleteSubTree(p->left);
	}
	free(Tree->root);
	free(Tree);
}

//returns 1 when the list is empty
//otherwise returns 0
int isEmptyT(Tree* tree) {
	if (tree->root==NULL) return 1;
	else return 0;
}

void AddPersonT(Tree* t, PersonT* start, int a, char name[30]) {
	
	//if Tree is empty
	if (t->root == NULL) {
		PersonT *p;
		p = (PersonT*)calloc(1, sizeof(PersonT));
		strcpy(p->name, name);
		p->a = a;
		p->left = NULL;
		p->right = NULL;
		p->parent = NULL;
		t->root = p;
	}
	//should be in left subTree
	else if (a<(start->a)) {
			
		if (start->left != NULL)
			AddPersonT(t,(start->left), a, name);
			
			//we are already in leaf - add new node
		else {
			PersonT *l;
			l = (PersonT*)calloc(1, sizeof(PersonT));
			strcpy(l->name, name);
			l->a = a;
			l->left = NULL;
			l->right = NULL;
			l->parent =start;
			start->left = l;
		}
	}
	else {
		if (start->right != NULL)
			AddPersonT(t,start->right, a, name);
		else {
			PersonT* r = (PersonT*)calloc(1, sizeof(PersonT));
			strcpy(r->name, name);
			r->a = a;
			r->left = NULL;
			r->right = NULL;
			r->parent = start;
			start->right = r;
		}
	}
}

struct PersonT* findPersonT(Tree* t,PersonT* start, int a, char name[30]){
	if (t->root == NULL) return NULL;
	if (comparePeopleT(start,a,name)==1) return start;
	
	if (a < start->a && start->left != NULL) return findPersonT(t,start->left, a,name);  
	if (a > start->a && start->right != NULL) return findPersonT(t,start->right, a,name);
	return NULL;
}

int comparePeopleT(PersonT* p, int a, char name[30]) {
	if (p == NULL) return 0;
	if (p->a == a && strcmp(p->name, name) == 0) return 1;
	return 0;
}

PersonT* findMax(PersonT* p) {
	if (p == NULL) return NULL;
	if (p->right == NULL) return p;
	else findMax(p->right);
	return NULL;
}

PersonT* findMin(PersonT* p) {
	if (p == NULL) return NULL;
	if (p->left == NULL) return p;
	else findMin(p->left);
	return NULL;
}

void copyPersonTValues(PersonT* d, PersonT* s)
{
	d->a = s->a;
	strcpy(d->name, s->name);
}

void deletePersonT(Tree* t,PersonT* p) {
	if (p->left == NULL && p->right == NULL) { // this is leaf
		if (p->parent == NULL) // this was the only element in tree. We remove the tree.
			t->root = NULL;
		else {
			if (p->parent->left == p) p->parent->left = NULL;
			else p->parent->right = NULL;
		}
		free(p);
	}
	else if (p->left != NULL) {
		PersonT* l = findMax(p->left);
		copyPersonTValues(p, l);
		deletePersonT(t, l);
	}
	else if (p->right != NULL) {
		PersonT* r = findMin(p->right);
		copyPersonTValues(p, r);
		deletePersonT(t, r);
	}
}

ListT* sortedTree(Tree* t) {
	//make tree copy
	if (t->root == NULL) return NULL;
	PersonT* min = findMin(t->root);
	ListT* list = malloc(sizeof(ListT));
	PersonL* prev = malloc(sizeof(PersonL));
	prev->a = min->a;
	strcpy(prev->name, min->name);
	list->head = prev;
	prev->next = NULL;
	deletePersonT(t, min);
	
	while (findMin(t->root)!=NULL) {
		min = findMin(t->root);
		PersonL* p = malloc(sizeof(PersonL));
		p->a = min->a;
		strcpy(p->name, min->name);
		p->next = NULL;
		prev->next = p;
		deletePersonT(t, min);
		prev = prev->next;
	}
	return list;
}


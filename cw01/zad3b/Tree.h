#pragma once
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct PersonT {
	int a;
	char name[30];
	struct PersonT* left;
	struct PersonT* right;
	struct PersonT* parent;
} PersonT;


typedef struct PersonL {
	int a;
	char name[30];
	struct PersonL* next;
} PersonL;

typedef struct ListT {
	PersonL* head;
}ListT;

typedef struct Tree {
	PersonT* root;
}Tree;

Tree * initTree(void);
void deleteSubTree(PersonT * node);
void deleteTree(Tree * Tree);
int isEmptyT(Tree* tree);
void AddPersonT(Tree* t, PersonT* start, int a, char name[30]);
struct PersonT* findPersonT(Tree* t, PersonT* start, int a, char name[30]);
int comparePeopleT(PersonT* p, int a, char name[30]);
PersonT* findMax(PersonT* p);
PersonT* findMin(PersonT* p);
void copyPersonTValues(PersonT* d, PersonT* s);
void deletePersonT(Tree* t, PersonT* p);
ListT* sortedTree(Tree* t);

#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Person {
	int a;
	char name[30];
	struct Person* prev;
	struct Person* next;
} Person;

typedef struct List {
	struct Person* head;
	struct Person* tail;
}List;
//BEZ WARTOWNIKA
 List* initList(void);


void printTest(void);
void deleteList(List* l);
int isEmpty(List* l);
void AddPerson(List* l,int a, char name[30]);

Person* findPerson(List* l, int a, char name[30]);

int comparePeople(Person* p, int a, char name[30]);

Person* cutPerson(struct List* l, int a, char name[30]);

void deletePerson(struct List* l, int a, char name[30]);

List* sortListByNumber(List* l, int a);

void printPerson(List* l,int a,char name[30]);

#endif // LIST_INCLUDED

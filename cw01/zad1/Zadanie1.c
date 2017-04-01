#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <conio.h>
#include <string.h>
#include "List.h"

//Bez wartownika
 List* initList() {
	List *ptr;
	ptr = (List*)calloc(1, sizeof(List));
	ptr->head = NULL;
	ptr->tail = NULL;
	return (ptr);
}

void deleteList(List* l) {
	Person* walker = l->head;
	if (walker != NULL) {

		while (walker->next != NULL)
		{
			walker = walker->next;
			free (walker->prev);
		}
		free (walker);
	}
}

int isEmpty(List* l) {
	if (l->head == NULL && l->tail == NULL) return 1;
	else return 0;
}
void AddPerson(List* l,int a, char name[30]) {

	Person *p;
	p= (Person*)calloc(1, sizeof(Person));
	strcpy(p->name, name);
	p->a = a;
	//if list is empty
	if (l->head == NULL) {
		l->head = p;
		l->head->prev = NULL;
		l->head->next = NULL;
		l->tail = l->head;
	}
	else {
		l->tail->next = p;
		p->prev = l->tail;
		p->next = NULL;
		l->tail = p;
	}
}

Person* findPerson(List* l, int a, char name[30]) {
	if (l == NULL) return NULL;
	Person* walker = l->head;
	while (walker != NULL) {
		if (walker->a == a && strcmp(name, walker->name)==0)
			return walker;
		walker = walker->next;
	}
	return NULL;
}

int comparePeople(Person* p, int a, char name[30]) {
	if (p == NULL) return 0;
	if (p->a ==a && strcmp(p->name,name)==0) return 1;
	return 0;
}
Person* cutPerson(struct List* l, int a, char name[30]) {

	if (l == NULL) return NULL;
	if (l->head == NULL) return NULL;
	Person* walker1 = l->head;
	Person* walker2 = walker1->next;
	//delete sole element
	if (comparePeople(walker1, a, name) == 1 && (l->head->next == NULL)) {
		l->head = NULL;
		l->tail = NULL;
		return walker1;
	}
	//delete first element, move head
	if (comparePeople(walker1, a, name) == 1) {
		l->head = l->head->next;
		l->head->prev = NULL;
		return walker1;
	}
	while (walker2 != NULL && comparePeople(walker2, a, name) == 0) {
		walker1 = walker1->next;
		walker2 = walker2->next;
	}
	if (comparePeople(walker2, a, name) == 1) {
		//walker2 is the last element
		if (walker2->next == NULL) {
			walker1->next = NULL;
			l->tail = walker1;
			return walker2;
		}
		else {
			walker1->next = walker2->next;
			walker2->next->prev = walker1;
			return walker2;
		}
	}
	return NULL;
}
void deletePerson(struct List* l, int a, char name[30]) {

	if (l == NULL) return;
	if (l->head == NULL) return;
	Person* walker1 = l->head;
	Person* walker2 = walker1->next;
	//delete sole element
	if (comparePeople(walker1, a, name) == 1 && (l->head->next == NULL)) {
		l->head = NULL;
		l->tail = NULL;
		free(walker1);
		return;
	}
	//delete first element, move head
	if (comparePeople(walker1, a, name) == 1) {
		l->head = l->head->next;
		l->head->prev = NULL;
		free(walker1);
		return;
	}
	while (walker2 != NULL && comparePeople(walker2,a, name)==0) {
		walker1 = walker1->next;
		walker2 = walker2->next;
	}
	if (comparePeople(walker2, a, name) == 1) {
		//walker2 is the last element
		if (walker2->next == NULL) {
			walker1->next = NULL;
			l->tail = walker1;
			free(walker2);
		}
		else {
			walker1->next = walker2->next;
			walker2->next->prev = walker1;
			free(walker2);
		}
	}
}

List* sortListByNumber(List* l, int a) {

	//if a > 0

	if (l == NULL) return NULL;
	if (l->head == NULL) return NULL;
	if (l->head->next == NULL) return l;
	//list has at least two elements
	int min = l->head->a;
	Person* smallest = l->head;
	Person* walker = l->head;

	while (walker != NULL) {
		if (walker->a < min) {
			min = walker->a;
			smallest = walker;
		}
		walker = walker->next;
	}

	//create new sorted list
	List* newList = initList();
	Person* p = cutPerson(l, smallest->a, smallest->name);
	newList->head = p;
	newList->tail = p;
	newList->head->prev = NULL;
	newList->head->next = NULL;


	min = l->head->a;
	smallest = l->head;
	walker = l->head;

	while (walker != NULL) {
		while (walker != NULL) {
			if (walker->a < min) {
				min = walker->a;
				smallest = walker;
			}
			walker = walker->next;
		}

		Person* p = cutPerson(l, smallest->a, smallest->name);
		newList->tail ->next = p;
		newList->tail = p;
		newList->tail->next = NULL;

		if (l->head != NULL) {
			min = l->head->a;
			smallest = l->head;
			walker = l->head;
		}
		else walker = NULL;

	}
	return newList;
}

void printPerson(List* l,int a,char name[30]){
    Person* p = findPerson(l,a,name);
    if(p!=NULL){
    	printf("\n Personal Id:  %d ", p->a);
    	printf("\n Person surname: ");
	printf(p->name);
    }
}

void printTest(){
	printf("\n Library test \n");
}

/*
 * linked_list.c
 *
 * Created: 4/7/2023 11:01:01 AM
 *  Author: genel
 */ 

#include "linked_list.h"

void init_ll(Linked_List *ll)
{
	ll->head = NULL;
	ll->tail = NULL;
	ll->size = 0;
}

void push_front(Linked_List *ll, int x, int y, int xvel)
{
	Node *newNode = malloc(sizeof(Node));
	newNode->xcoord = x;
	newNode->ycoord = y;
	newNode->xvel = xvel;
	newNode->next = NULL;
	newNode->prev = NULL;
	if (ll->head) {
		newNode->next = ll->head;
		ll->head->prev = newNode;
	} else {
		ll->tail = newNode;
	}
	ll->head = newNode;
	ll->size ++;
}

void pop_back(Linked_List *ll)
{
	if (ll->tail) {
		//singleton and above cases
		Node *newTail = ll->tail->prev;
		free(ll->tail);
		ll->tail = newTail;
		if (!newTail) {
			ll->head = NULL;
		} else {
			newTail->next = NULL;
		}
		ll->size --;
	}
}

void remove_node(Linked_List *ll, Node* n) {
	if (n->next) {
		n->next->prev = n->prev;
	} else {
		//tail
		ll->tail = n->prev;
	}
	if (n->prev) {
		n->prev->next = n->next;
	} else {
		//head
		ll->head = n->next;
	}
	free(n);
}

void ll_clear(Linked_List *ll) {
	while (ll->tail) {
		pop_back(ll);
	}
}
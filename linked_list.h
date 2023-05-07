/*
 * linked_list.h
 *
 * Created: 4/7/2023 10:51:43 AM
 *  Author: genel
 */ 
#pragma once
#include <stdio.h>
#include <stdlib.h>

//small form linked list for snake/tron game

//node, represents map cell in snake
typedef struct node
{
	int xcoord;
	int ycoord;	
	int xvel; //used in game2
	struct node *next;
	struct node *prev;
} Node;

typedef struct linked_list
{
	Node *head;
	Node *tail;
	int size;
} Linked_List;

void init_ll(Linked_List *ll);
//pushes a node with x, y to front of linked list
void push_front(Linked_List *ll, int x, int y, int xvel);
//pops the last element of ll
void pop_back(Linked_List *ll);

void remove_node(Linked_List *ll, Node* n);

void ll_clear(Linked_List *ll);
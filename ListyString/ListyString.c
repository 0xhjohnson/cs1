// Hunter Johnson
// COP 3502, Fall 2018
// NID | hu066164

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

double difficultyRating(void)
{
	return 4.5;
}

double hoursSpent(void)
{
	return 24.0;
}

// read and process input file in prep for string maniputlations
// code modified from zoom's file io notes
int processInputFile(char *filename)
{
	char input[1024];
	char str[1024];
	char key[1];
	ListyString *listy = NULL;
	
	FILE *ifp = NULL;

	// bad filename check
	if ((ifp = fopen(filename, "r")) == NULL)
		return 1;

	if (fscanf(ifp, "%s", input) != EOF)
		listy = createListyString(input);

	while (fscanf(ifp, "%s", str) != EOF)
	{
		switch(str[0])
		{
			case '@':
				fscanf(ifp, "%s%s", key, str);
				replaceChar(listy, key[0], str);
				break;
			case '+':
				fscanf(ifp, "%s", str);
				listyCat(listy, str);
				break;
			case '-':
				fscanf(ifp, "%s", key);
				replaceChar(listy, key[0], NULL);
				break;
			case '~':
				reverseListyString(listy);
				break;
			case '?':
				printf("%d\n", listyLength(listy));
				break;
			case '!':
				printListyString(listy);
				break;
		}
	}

	fclose(ifp);
	ifp = NULL;

	return 0;
}

// helper function for creating new nodes
ListyNode *createNode(char c)
{
	ListyNode *node = malloc(sizeof(ListyNode));

	node->data = c;
	node->next = NULL;

	return node;
}

// converts str to a ListyString 
// returns pointer to ListyString
ListyString *createListyString(char *str)
{
	int i;
	ListyNode *tmp;

	// make space for new ListyString
	ListyString *listy = malloc(sizeof(ListyString));

	// check in case malloc fails
	if (listy == NULL)
		return NULL;

	// special consideration if str is null or empty
	if (str == NULL || str[0] == '\0')
	{
		listy->head = NULL;
		listy->length = 0;

		return listy;
	}

	listy->head = createNode(str[0]);
	listy->length = strlen(str);

	tmp = listy->head;

	for (i = 1; i < listy->length; i++)
	{
		tmp->next = createNode(str[i]);
		tmp = tmp->next;
	}

	return listy;
}

// frees any dynamically allocated memory and returns NULL
// returns null
ListyString *destroyListyString(ListyString *listy)
{
	int i;
	ListyNode *tmp;

	if (listy == NULL || listy->head == NULL)
		return NULL;

	while ((tmp = listy->head) != NULL)
	{
		listy->head = listy->head->next;
		free(tmp);
	}

	// now we can free listy
	free(listy);

	return NULL;
}

// creates and returns new copy of listy
// returns pointer to listy unless case 2
// case 2: pointer passed to function is null, returns null
ListyString *cloneListyString(ListyString *listy)
{
	ListyNode *tmp1, *tmp2;

	if (listy == NULL)
		return NULL;

	ListyString *clone = malloc(sizeof(ListyString));

	// special consideration if if head is null
	if (listy->head == NULL)
	{
		clone->head = NULL;
		clone->length = 0;

		return clone;
	}

	tmp1 = listy->head;
	tmp2 = clone->head;
	clone->head = createNode(tmp1->data);
	
	while (tmp1->next == NULL)
	{
		tmp2->next = createNode(tmp1->next->data);
		tmp2 = tmp2->next;
		tmp1 = tmp1->next;
	}

	clone->length = listy->length;

	return clone;
}

// takes a ListyString (listy) and replaces all instances of certain char (key) with specified string (str)
// meets runtime requirement of O(n+km)
void replaceChar(ListyString *listy, char key, char *str)
{
	int i, length;
	// make sure that we init previous to NULL to avoid segfaults
	ListyNode *tmp, *current, *previous = NULL, *replaceNode;

	if (listy == NULL || listy->head == NULL)
		return;

	if (str == NULL)
		length = 0;
	else
		length = strlen(str);

	current = listy->head;

	while (current != NULL)
	{
		if (current->data == key)
		{
			// str is null or empty string
			if (str == NULL || str[0] == '\0')
			{
				if (previous == NULL)
				{
					tmp = current;
					current = current->next;
					listy->head = current;
					free(tmp);
				}
				else if (current->next == NULL)
				{
					tmp = current;
					previous->next = current->next;
					current = current->next;
					free(tmp);
					// update length member
					listy->length--;
				}
				else
				{
					tmp = current;
					previous->next = tmp->next;
					current = previous->next;
					free(tmp);
					// update length member
					listy->length--;
				}
			}
			// str is not null or empty
			else
			{
				current->data = str[0];

				for (i = 1; i < length; i++)
				{
					replaceNode = createNode(str[i]);
					replaceNode->next = current->next;
					current->next = replaceNode;
					current = current->next;
					listy->length++;
				}

				previous = current;
				current = current->next;
			}
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

// reverses the linked list contained within listy
void reverseListyString(ListyString *listy)
{
	ListyNode *tmp, *current, *previous;

	// guard against seg faults
	if (listy == NULL || listy->head == NULL)
		return;

	// head insert runtime consideration
	current = listy->head;
	previous = NULL;

	while (current != NULL)
	{
		tmp = current->next;
		current->next = previous;
		previous = current;
		current = tmp;
	}

	listy->head = previous;
}

// concats str to end of linked list string inside listy
// if this function causes creation of new ListyString, returns pointer to it
// if special consideration returns null pointer
// otherwise returns listy
ListyString *listyCat(ListyString *listy, char *str)
{
	int i, length;
	ListyNode *tmp;

	if (str == NULL || str[0] == '\0')
		return listy;

	// special consideration
	if (listy == NULL && str == NULL)
		return NULL;

	length = strlen(str);

	// listy is null and str is empty string
	if (listy == NULL && str[0] == '\0')
	{
		listy->head = createNode(str[0]);
		tmp = listy->head;

		for (i = 1; i < length; i++)
		{
			tmp->next = createNode(str[i]);
			tmp = tmp->next;
		}

		return listy;
	}
	else if (listy->head == NULL)
	{
		listy->head = createNode(str[0]);
		tmp = listy->head;

		for (i = 1; i < length; i++)
		{
			tmp->next = createNode(str[i]);
			tmp = tmp->next;
		}

		// set length member
		listy->length = length;

		return listy;
	}

	tmp = listy->head;

	while (tmp->next != NULL)
		tmp = tmp->next;

	for (i = 0; i < length; i++)
	{
		tmp->next = createNode(str[i]);
		tmp = tmp->next;
	}

	listy->length += length;

	return listy;
}

// conpares two ListyStrings, returning 0 if equivalent, otherwise returns 1
int listyCmp(ListyString *listy1, ListyString *listy2)
{
	int i;
	ListyNode *tmp1, *tmp2;

	if (listy1 == listy2)
		return 0;
	if (listy1 == NULL || listy2 == NULL)
		return 1;
	if (listy1->head == listy2->head)
		return 0;

	tmp1 = listy1->head;
	tmp2 = listy2->head;

	if (listy1->length == listy2->length)
	{
		while (tmp1->data == tmp2->data)
		{
			if (tmp1->next == NULL || tmp2->next == NULL)
			{
				return 0;
			}
			tmp1 = tmp1->next;
			tmp2 = tmp2->next;
		}
	}
}

// returns length of ListyString
// returns -1 is listy is null
int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	else if (listy->head == NULL)
		return 0;

	return listy->length;
}

// prints string stored in listy followed by newline
// prints (empty string) if listy is null or empty string
void printListyString(ListyString *listy)
{
	ListyNode *tmp;

	if (listy == NULL || listy->head == NULL)
	{
		printf("(empty string)\n");
		return;
	}

	tmp = listy->head;

	while (tmp != NULL)
	{
		printf("%c", tmp->data);
		tmp = tmp->next; 
	}
	printf("\n");
}


int main(int argc, char **argv)
{
	processInputFile(argv[1]);

	return 0;
}


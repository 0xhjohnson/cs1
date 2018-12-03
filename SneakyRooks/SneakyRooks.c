// Hunter Johnson
// COP 3502, Fall 2018
// NID | hu066164

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SneakyRooks.h"

double difficultyRating(void)
{
	return 3.5;
}

double hoursSpent(void)
{
	return 9.5;
}

// returns 1 if none of the rooks can attack one another
// returns 0 otherwise
int allTheRooksAreSafe(char **rookStrings, int numRooks, int boardSize)
{
	int i, *row, *col;
	Coordinate *tmp;

	// calloc some space for the rows and cols
	// initializing board to 0
	row = calloc(1, sizeof(int) * (boardSize + 1));
	col = calloc(1, sizeof(int) * (boardSize + 1));
	tmp = malloc(sizeof(Coordinate));

	// check for malloc failure, calloc generally safe
	if (row == NULL || col == NULL || tmp == NULL)
	{
		free(row);
		free(col);
		free(tmp);

		return 0;
	}

	// surely empty string means rooks are safe
	if(**rookStrings == '\0')
	{
		free(row);
		free(col);
		free(tmp);

		return 1;
	}

	for (i = 0; i < numRooks; i++)
	{
		parseCoordinateString(rookStrings[i], tmp);

		// rooks are not safe
		if(row[tmp->row] == 1 || col[tmp->col] == 1)
		{
			free(row);
			free(col);
			free(tmp);

			return 0;
		}

		row[tmp->row] = 1;
		col[tmp->col] = 1;
	}

	// clean up after ourselves
	free(row);
	free(col);
	free(tmp);

	// rooks are safe
	return 1;
}

// parses through rookString and determines row/col rook resides on
// populating rookCoordinate with result
void parseCoordinateString(char *rookString, Coordinate *rookCoordinate)
{
	int i, row = 0, col = 0;

	// best practice to ensure not dereferencing null pointer
	if(rookCoordinate == NULL)
		return;

	// loop through given string using strlen function
	for(i = 0; i < strlen(rookString); i++)
	{
		// parse the string setting col and row along the way
		if(isalpha(rookString[i]))
			col = (col * 26) + (rookString[i] - 'a' + 1);
		else
			row = (row * 10) + (rookString[i] - '0');
	}

	// populate rookCoordinate with result
	rookCoordinate->col = col;
	rookCoordinate->row = row;
}

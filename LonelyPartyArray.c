// Hunter Johnson
// COP 3502, Fall 2018
// NID | hu066164

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

double difficultyRating(void) 
{
  return 4.0;
}

double hoursSpent(void) 
{
  return 22.0;
}

// Creates a LPA that can hold up to num_fragments of length fragment_length
// Max number of integers is num_fragments x fragment_length
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  if (num_fragments <= 0 || fragment_length <= 0)
    return NULL;

  LonelyPartyArray *party = malloc(sizeof(LonelyPartyArray));

  // init struct members
  party->num_fragments = num_fragments;
  party->fragment_length = fragment_length;
  party->size = 0;
  party->num_active_fragments = 0;

  party->fragments = calloc(1,sizeof(int**) *num_fragments);
  party->fragment_sizes = calloc(1,sizeof(int*) *num_fragments);

  if(party == NULL || party->fragments == NULL || party->fragment_sizes == NULL)
  {
    free(party->fragments);
    free(party->fragment_sizes);
    return NULL;
  }

  // seperate print statements for readability 
  printf("-> A new LonelyPartyArray has emerged from the void. ");
  printf("(capacity: %d, fragments: %d)\n", (num_fragments * fragment_length), num_fragments);

  return party;
  
}

// Frees all dynamically allocated mem associated with LPA struct and returns NULL
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
  int i;

  if (party != NULL)
  {
    // free each fragment first
    for (i = 0; i < party->num_fragments; i++)
      free(party->fragments[i]);

    free(party->fragments);
    free(party->fragment_sizes);
    free(party);

    printf("-> The LonelyPartyArray has returned to the void.\n");
  }

  return NULL;
}

// Inserts key into the correct index of LPA 
int set(LonelyPartyArray *party, int index, int key)
{
  int i, row, col, startIndex, endIndex, isValidIndex;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in set().\n");
    return LPA_FAILURE;
  }

  // determining correct index, will be used in other functions
  row = index / party->fragment_length;
  col = index % party->fragment_length;
  startIndex = row * (party->fragment_length);
  endIndex = startIndex + (party->fragment_length - 1);
  isValidIndex = party->num_fragments * party->fragment_length - 1;

  // ensure the index is within bounds
  if (index < 0 || index > isValidIndex)
  {
    printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
    return LPA_FAILURE;
  }

  if (party->fragments[row] == NULL)
  {
    party->fragments[row] = malloc(sizeof(int) * party->fragment_length);

    if (party->fragments[row] == NULL)
      return LPA_FAILURE;
    else
      party->num_active_fragments++;
    
    // init each cell in new fragment to unused
    for (i = 0; i < party->fragment_length; i++)
      party->fragments[row][i] = UNUSED; // defined in headerfile

    // successful allocation of new fragment in mem
    printf("-> Spawned fragment %d. ", row);
    printf("(capacity: %d, indices: %d..%d)\n", party->fragment_length, startIndex, endIndex);
  }
  if (party->fragments[row][col] == UNUSED)
  {
    party->size++;
    party->fragment_sizes[row]++;
  }

  party->fragments[row][col] = key;
  return LPA_SUCCESS;
  
}

// Retrieves val stored at corresponding index of LPA
int get(LonelyPartyArray *party, int index)
{
  int row, col, isvalidindex;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in get().\n");
    return LPA_FAILURE;
  }

  // taken from the set function
  row = index / party->fragment_length;
  col = index % party->fragment_length;
  isvalidindex = party->num_fragments * party->fragment_length - 1;
  
  if (index < 0 || index > isvalidindex)
  {
    printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
    return LPA_FAILURE;
  }

  if (party->fragments[row] == NULL)
    return UNUSED;
  else
    return party->fragments[row][col];
}

// Sets value stored at corresponding index of LPA to UNUSED
int delete(LonelyPartyArray *party, int index)
{
  int row, col, startIndex, endIndex, isValidIndex;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in delete().\n");
    return LPA_FAILURE;
  }
  
  // taken from set function
  row = index / party->fragment_length;
  col = index % party->fragment_length;
  isValidIndex = party->num_fragments * party->fragment_length - 1;
  startIndex = row * (party->fragment_length);
  endIndex = startIndex + (party->fragment_length - 1);

  if (index < 0 || index > isValidIndex)
  {
    printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, row, col);
    return LPA_FAILURE;
  }

  if (party->fragments[row] == NULL)
    return LPA_FAILURE;

  if (party->fragments[row][col] != UNUSED)
  {
    // sets val to unused, decrements struct size and fragment size
    party->fragments[row][col] = UNUSED;
    party->size--;
    party->fragment_sizes[row]--;

    // if deleting val at index causes the fragment to become empty, deallocate array and set to null
    if (party->fragment_sizes[row] == 0)
    {
      free(party->fragments[row]);
      party->fragments[row] = NULL;
      party->num_active_fragments--;

      printf("-> Deallocated fragment %d. ", row);
      printf("(capacity: %d, indices: %d..%d)\n", party->fragment_length, startIndex, endIndex);
    }
  }
  else
    return LPA_FAILURE; // fragment already UNUSED

  return LPA_SUCCESS;
}

// Linear search through LPA to determine if it contains key
// Returns 1 if LPA contains key, otherwise 0
int containsKey(LonelyPartyArray *party, int key)
{
  int i, j;

  if (party == NULL)
    return 0;

  for (i = 0; i < party->num_fragments; i++)
  {
    if (party->fragments[i] != NULL)
    {
      for (j = 0; j < party->fragment_length; j++)
      {
        if (party->fragments[i][j] == key)
          return 1;
      }
    }
  }

  return 0;
}

// Determines whether value stored (other than UNUSED) at corresponding index
int isSet(LonelyPartyArray *party, int index)
{
  int row, col, isValidIndex;
  
  row = index / party->fragment_length;
  col = index % party->fragment_length;
  isValidIndex = party->num_fragments * party->fragment_length - 1;

  if (index < 0 || index > isValidIndex)
    return 0;

  if (party->fragments[row] == NULL || party->fragments[row][col] == UNUSED)
    return 0;

  return 1;
}

int printIfValid(LonelyPartyArray *party, int index)
{
  int row, col, isvalidindex;

  if (party == NULL)
    return LPA_FAILURE;

  // taken from set function
  row = index / party->fragment_length;
  col = index % party->fragment_length;
  isvalidindex = party->num_fragments * party->fragment_length - 1;
  
  if (index < 0 || index > isvalidindex)
    return LPA_FAILURE;
  
  if (party->fragments[row] == NULL || party->fragments[row][col] == UNUSED)
    return LPA_FAILURE;
  else
  {
    printf("%d\n", party->fragments[row][col]);
    return LPA_SUCCESS;
  }

  return LPA_SUCCESS;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;

  if (party == NULL)
  {
    printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
    return party;
  }
  else
  {
    for (i = 0; i < party->num_fragments; i++)
    {
      if (party->fragments[i] != NULL)
      {
        free(party->fragments[i]);
        party->fragments[i] = NULL;
      }
      if (party->fragment_sizes[i] != 0)
        party->fragment_sizes[i] = 0;
    }
    
    party->size = 0;
    party->num_active_fragments = 0;

    printf("-> The LonelyPartyArray has returned to its nascent state. ");
    printf("(capacity: %d, fragments: %d)\n", party->num_fragments * party->fragment_length, party->num_fragments);

    return party;
  }
}

// Returns number of elements currently in LPA excluding UNUSED
int getSize(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;

  return party->size;
}

// Returns max number of elements party can hold
int getCapacity(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;

  return party->num_fragments * party->fragment_length;
}

// Returns the max number of elements party can hold without allocation of new fragments
int getAllocatedCellCount(LonelyPartyArray *party)
{
  if (party == NULL)
    return -1;

  return party->num_active_fragments * party->fragment_length;
}

// Returns the amount in bytes necessary if standard array was used rather than LPA
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int arraySize;

  if (party == NULL)
    return 0;

  arraySize = (party->fragment_length * party->num_fragments);
  return arraySize * sizeof(int); 
}

// Returns amount in bytes currently taken up in memory by LPA
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int lpaPointer, lpaStruct, fragArr, fragSizesArr, fragActive, sum;

  if (party == NULL)
    return 0;

  lpaPointer = sizeof(party);
  lpaStruct = sizeof(LPA);
  fragArr = sizeof(int*) * party->num_fragments;
  fragSizesArr = sizeof(int) * party->num_fragments;
  fragActive = sizeof(int) * (party->num_active_fragments * party->fragment_length);

  sum = lpaPointer + lpaStruct + fragArr + fragSizesArr + fragActive;

  return sum;
}

// Bonus function
// Dynamically allocate a clone of LPA
LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party);

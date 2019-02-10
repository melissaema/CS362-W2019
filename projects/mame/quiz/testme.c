/************************************************************
Name: Melissa Ma
Date: 2/9/19
Course: CS 362
Description: This is the code for the random testing quiz.
***********************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	// Characters to choose from
	char pool[10] = "[({ ax})]";

	// Select random character from pool
	char randomChar = pool[rand() % 9];

    return randomChar;
}

char *inputString()
{
	// Characters to choose from
	char pool[5] = "rest";

	// String to build
	char buildstring[6] = "";

	// Set last char to the null terminator
	buildstring[5] = '\0';

	// Randomly assign characters from pool to each slot in randomstring
	int i;
	for (i = 0; i < 5; i++) {
		buildstring[i] = pool[rand() % 4];
	}
	
	char *randomstring = buildstring;

	return randomstring;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}

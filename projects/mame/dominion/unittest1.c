/* -----------------------------------------------------------------------
 * Name: Melissa Ma
 * Date: 2/7/19
 * Description: Tests the whoseTurn function in dominion.c
 * -----------------------------------------------------------------------
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int assertTrue(int a, int b, char *msg) {
	if (a == b) {
		printf("PASS: Expected: %d, Actual: %d, %s\n", b, a, msg);
		return 0;
	}
	else {
		printf("FAIL: Expected: %d, Actual: %d, %s\n", b, a, msg);
		return 1;
	}
}

int main() {
	printf("\n----------Testing whoseTurn()----------\n");

	int errors = 0;

	// Initalize game state
	struct gameState G = { 0 };
	
	// Testing for 2 players: Player 0
	errors += assertTrue(whoseTurn(&G), 0, "whoseTurn(): It is Player 0's turn.");

	// Testing for 2 players: Player 1
	G.whoseTurn = 1;
	errors += assertTrue(whoseTurn(&G), 1, "whoseTurn(): It is Player 1's turn.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End whoseTurn() testing--------\n");
    
	return 0;
}

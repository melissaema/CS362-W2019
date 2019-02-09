/* -----------------------------------------------------------------------
 * Name: Melissa Ma
 * Date: 2/7/19
 * Description: Tests the isGameOver function in dominion.c
 * -----------------------------------------------------------------------
 * Include the following lines in your makefile:
 *
 * unittest2: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
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
	printf("\n----------Testing isGameOver()----------\n");

	// Initalize game state and variables
	int errors = 0;
	int j;
	int seed = 1000;
	int numPlayer = 2;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
	struct gameState G;

	memset(&G, 23, sizeof(struct gameState)); // Clear the game state
	initializeGame(numPlayer, k, seed, &G); // Initalize new game
	
	// Test no empty piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	assertTrue(isGameOver(&G), 0, "isGameOver(): Test no empty piles");

	// Test all empty piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 0;
	}
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test all empty piles");

	// Test empty province pile and no other empty supply piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 0;
	}
	G.supplyCount[province] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test empty province pile and no other empty supply piles");

	// Test empty province pile and an empty supply pile
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 0;
	}
	G.supplyCount[province] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test empty province pile and an empty supply pile");

	// Test no empty province pile and all other empty supply piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 0;
	}
	G.supplyCount[province] = 10;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test no empty province pile and all other empty supply piles");

	// Test no empty province pile and 1 other empty supply pile
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	assertTrue(isGameOver(&G), 0, "isGameOver(): Test no empty province pile and 1 other empty supply pile");

	// Test no empty province pile and 2 other empty supply piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	assertTrue(isGameOver(&G), 0, "isGameOver(): Test no empty province pile and 2 other empty supply piles");

	// Test no empty province pile and 3 other empty supply piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[estate] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test no empty province pile and 3 other empty supply piles");
	
	// Test no empty province pile and 3 other empty supply piles (one is the first pile)
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[curse] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test no empty province pile and 3 other empty supply piles (one is the first pile)");


	// Test no empty province pile and 3 other empty supply piles (one is the last pile)
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[treasure_map] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test no empty province pile and 3 other empty supply piles (one is the last pile)");

	// Test no empty province pile and 4 other empty supply piles
	for (j = 0; j < treasure_map + 1; j++) {
		G.supplyCount[j] = 10;
	}
	G.supplyCount[province] = 10;
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[estate] = 0;
	G.supplyCount[village] = 0;
	assertTrue(isGameOver(&G), 1, "isGameOver(): Test no empty province pile and 4 other empty supply piles");
	
	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End isGameOver() testing--------\n");
    
	return 0;
}

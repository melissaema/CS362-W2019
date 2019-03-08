/* -----------------------------------------------------------------------
 * Name: Melissa Ma
 * Date: 2/7/19
 * Description: Tests the updateCoins function in dominion.c
 * -----------------------------------------------------------------------
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
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
	printf("\n----------Testing updateCoins()----------\n");

	// Initalize game state and variables
	int errors = 0;
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int maxBonus = 5;
	int p, handCount;
	int bonus;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
				  remodel, smithy, village, baron, great_hall };
	struct gameState G;
	int maxHandCount = 5;

	// arrays of all coppers, silvers, golds, and smithys
	int coppers[MAX_HAND];
	int silvers[MAX_HAND];
	int golds[MAX_HAND];
	int smithys[MAX_HAND];
	for (i = 0; i < MAX_HAND; i++)
	{
		coppers[i] = copper;
		silvers[i] = silver;
		golds[i] = gold;
		smithys[i] = smithy;
	}

	// Build a combination hand
	int otherhand[5] = { copper, silver, gold, smithy, silver }; // Should be 8 coins if counting all

	// Empty hand
	int emptyhand[5] = { -1 };
	
	// Test the copper, silver, gold, smithy hands
	printf("\n---Testing copper, silver, gold, other (Smithy) cards---\n");
	for (p = 0; p < numPlayer; p++) {
		for (handCount = 1; handCount <= maxHandCount; handCount++) {
			for (bonus = 0; bonus <= maxBonus; bonus++) {
				memset(&G, 23, sizeof(struct gameState)); // Clear the game state
				initializeGame(numPlayer, k, seed, &G); // Initalize new game
				G.handCount[p] = handCount; // Set number of cards in hand

				// Test copper hand
				memcpy(G.hand[p], coppers, sizeof(int) * handCount);
				updateCoins(p, &G, bonus);
				errors += assertTrue(G.coins, handCount + bonus, "updateCoins: copper");

				// Test silver hand
				memcpy(G.hand[p], silvers, sizeof(int) * handCount);
				updateCoins(p, &G, bonus);
				errors += assertTrue(G.coins, handCount * 2 + bonus, "updateCoins: silver");

				// Test gold hand
				memcpy(G.hand[p], golds, sizeof(int) * handCount);
				updateCoins(p, &G, bonus);
				errors += assertTrue(G.coins, handCount * 3 + bonus, "updateCoins: gold");

				// Test smithy hand (should always be 0 coins plus bonus)
				memcpy(G.hand[p], smithys, sizeof(int) * handCount);
				updateCoins(p, &G, bonus);
				errors += assertTrue(G.coins, bonus, "updateCoins: other (smithy)");
			}
		}
	}

	// Test player has no cards (coins should be 0 plus bonus)
	printf("\n-------Testing player has no cards-------\n");
	G.handCount[0] = 0;
	memcpy(G.hand[0], emptyhand, sizeof(int) * 5);
	for (bonus = 0; bonus <= 5; bonus++) {
		updateCoins(0, &G, bonus);
		errors += assertTrue(G.coins, bonus, "updateCoins: Coins should equal bonus.");
	}

	// Test combination hand
	printf("\n--------Testing combination hand--------\n");
	G.handCount[1] = 5;
	memcpy(G.hand[1], otherhand, sizeof(int) * 5);
	updateCoins(1, &G, 0);
	errors += assertTrue(G.coins, 8, "updateCoins: Player has 8 coins.");
	updateCoins(1, &G, 5);
	errors += assertTrue(G.coins, 13, "updateCoins: Player has 13 coins.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End updateCoins() testing--------\n");
    
	return 0;
}

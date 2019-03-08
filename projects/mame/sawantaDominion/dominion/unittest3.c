/* -----------------------------------------------------------------------
 * Name: Melissa Ma
 * Date: 2/7/19
 * Description: Tests the scoreFor function in dominion.c
 * -----------------------------------------------------------------------
 * Include the following lines in your makefile:
 *
 * unittest2: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int assertTrue(int a, int b) {
	if (a == b) {
		printf("PASS: Expected: %d, Actual: %d\n", b, a);
		return 0;
	}
	else {
		printf("FAIL: Expected: %d, Actual: %d\n", b, a);
		return 1;
	}
}

int main() {
	printf("\n----------Testing scoreFor()----------\n");

	// Initalize game state and variables
	int errors = 0;
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int p, count;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
	int maxCount = 5;
	struct gameState G;

	// Initialize arrays of cards
	int curses[MAX_HAND];
	int estates[MAX_HAND];
	int duchys[MAX_HAND];
	int provinces[MAX_HAND];
	int greathalls[MAX_HAND];
	int gardenscard[MAX_HAND];
	int smithys[MAX_HAND];
	for (i = 0; i < MAX_HAND; i++)
	{
		curses[i] = curse;
		estates[i] = estate;
		duchys[i] = duchy;
		provinces[i] = province;
		greathalls[i] = great_hall;
		gardenscard[i] = gardens;
		smithys[i] = smithy;
	}

	int emptypile[5] = { -1, -1, -1, -1, -1 };
	int testpile[6] = { curse, estate, duchy, province, great_hall, gardens };

	printf("\n---Testing each of the 6 cards---\n");
	for (p = 0; p < numPlayer; p++) {
		for (count = 1; count < maxCount; count++) {
			memset(&G, 23, sizeof(struct gameState)); // Clear the game state
			initializeGame(numPlayer, k, seed, &G); // Initalize new game
			G.handCount[p] = count; // Set number of cards in hand
			G.deckCount[p] = count; // Set number of cards in deck
			G.discardCount[p] = count; // Set number of cards in discard

			// Test all curse cards
			memcpy(G.hand[p], curses, sizeof(int) * count);
			memcpy(G.deck[p], curses, sizeof(int) * count);
			memcpy(G.discard[p], curses, sizeof(int) * count);
			errors += assertTrue(scoreFor(p, &G), 3 * count * -1);
			
			// Test all estate cards
			memcpy(G.hand[p], estates, sizeof(int) * count);
			memcpy(G.deck[p], estates, sizeof(int) * count);
			memcpy(G.discard[p], estates, sizeof(int) * count);
			errors += assertTrue(scoreFor(p, &G), 3 * count);

			// Test all duchy cards
			memcpy(G.hand[p], duchys, sizeof(int) * count);
			memcpy(G.deck[p], duchys, sizeof(int) * count);
			memcpy(G.discard[p], duchys, sizeof(int) * count);
			errors += assertTrue(scoreFor(p, &G), 3 * count * 3);

			// Test all province cards
			memcpy(G.hand[p], provinces, sizeof(int) * count);
			memcpy(G.deck[p], provinces, sizeof(int) * count);
			memcpy(G.discard[p], provinces, sizeof(int) * count);
			errors += assertTrue(scoreFor(p, &G), 3 * count * 6);

			// Test all great hall cards
			memcpy(G.hand[p], greathalls, sizeof(int) * count);
			memcpy(G.deck[p], greathalls, sizeof(int) * count);
			memcpy(G.discard[p], greathalls, sizeof(int) * count);
			errors += assertTrue(scoreFor(p, &G), 3 * count);

			// Test all smithy cards
			memcpy(G.hand[p], smithys, sizeof(int) * count);
			memcpy(G.deck[p], smithys, sizeof(int) * (count + 1));
			memcpy(G.discard[p], smithys, sizeof(int) * (count + 2));
			errors += assertTrue(scoreFor(p, &G), 0);
			
		}
	}

	// Test with different number of cards in each pile
	printf("\n---Testing different number of cards in each pile---\n");
	G.handCount[0] = 3;
	G.deckCount[0] = 6;
	G.discardCount[0] = 4;
	memcpy(G.hand[0], estates, sizeof(int) * 3);
	memcpy(G.deck[0], estates, sizeof(int) * 4);
	memcpy(G.discard[0], estates, sizeof(int) * 5);
	errors += assertTrue(scoreFor(0, &G), 13);
	
	// Test 9 gardens cards in each pile
	printf("\n---Testing 9 garden cards in each pile---\n");
	G.handCount[0] = 3;
	G.deckCount[0] = 3;
	G.discardCount[0] = 3;
	memcpy(G.hand[0], gardenscard, sizeof(int) * 3);
	memcpy(G.deck[0], gardenscard, sizeof(int) * 3);
	memcpy(G.discard[0], gardenscard, sizeof(int) * 3);
	errors += assertTrue(scoreFor(0, &G), 0); // Total of 9 cards, each gardens card is 0 points

	// Test 10 gardens cards in each pile
	printf("\n---Testing 10 garden cards in each pile---\n");
	G.handCount[0] = 10;
	G.deckCount[0] = 10;
	G.discardCount[0] = 10;
	memcpy(G.hand[0], gardenscard, sizeof(int) * 10);
	memcpy(G.deck[0], gardenscard, sizeof(int) * 10);
	memcpy(G.discard[0], gardenscard, sizeof(int) * 10);
	errors += assertTrue(scoreFor(0, &G), 90); // Total of 30 cards, each gardens card is 3 points

	// Test for 0 cards in hand, deck, and discard pile
	printf("\n---Testing 0 cards in hand, deck, and discard piles---\n");
	G.handCount[0] = 0;
	G.deckCount[0] = 0;
	G.discardCount[0] = 0;
	memcpy(G.hand[0], emptypile, sizeof(int) * 5);
	memcpy(G.deck[0], emptypile, sizeof(int) * 5);
	memcpy(G.discard[0], emptypile, sizeof(int) * 5);
	errors += assertTrue(scoreFor(0, &G), 0);
	
	// Test for 1 of each of the 6 cards in all 3 piles
	printf("\n---Testing 1 of each of the 6 cards in all 3 piles---\n");
	G.handCount[1] = 6;
	G.deckCount[1] = 6;
	G.discardCount[1] = 6;
	memcpy(G.hand[1], testpile, sizeof(int) * 6);
	memcpy(G.deck[1], testpile, sizeof(int) * 6);
	memcpy(G.discard[1], testpile, sizeof(int) * 6);
	errors += assertTrue(scoreFor(1, &G), 33); // Total of 18 cards, each gardens card is 1 point
	
	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End scoreFor() testing--------\n");
    
	return 0;
}

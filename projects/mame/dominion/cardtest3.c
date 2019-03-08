/* -----------------------------------------------------------------------
* Name: Melissa Ma
* Date: 2/9/19
* Description: Tests the village card in dominion.c
* -----------------------------------------------------------------------
* Include the following lines in your makefile:
*
* cardtest3: cardtest3.c dominion.o rngs.o
*      gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "village"

// assertTrue compares two values, a (tested value) and b (expected value)
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

// testStateChange compares two states to see if anything has changed
// Returns 0 if nothing has changed, 1 if there is a change
int testStateChange(struct gameState g1, struct gameState g2, int players) {
	int i, j;
	if (g1.numPlayers != g2.numPlayers) {
		return 1;
	}
	for (i = 0; i < treasure_map + 1; i++) {
		if (g1.supplyCount[i] != g2.supplyCount[i]) {
			return 1;
		}
	}
	for (i = 0; i < treasure_map + 1; i++) {
		if (g1.embargoTokens[i] != g2.embargoTokens[i]) {
			return 1;
		}
	}
	if (g1.outpostPlayed != g2.outpostPlayed) {
		return 1;
	}
	if (g1.outpostTurn != g2.outpostTurn) {
		return 1;
	}
	if (g1.whoseTurn != g2.whoseTurn) {
		return 1;
	}
	if (g1.phase != g2.phase) {
		return 1;
	}
	if (g1.coins != g2.coins) {
		return 1;
	}
	if (g1.numBuys != g2.numBuys) {
		return 1;
	}

	// Check other players' states
	for (i = 1; i < players; i++) {
		for (j = 0; j < MAX_HAND; j++) {
			if (g1.hand[i][j] != g2.hand[i][j]) {
				return 1;
			}
		}
		if (g1.handCount[i] != g2.handCount[i]) {
			return 1;
		}
	}
	for (i = 1; i < players; i++) {
		for (j = 0; j < MAX_HAND; j++) {
			if (g1.deck[i][j] != g2.deck[i][j]) {
				return 1;
			}
		}
		if (g1.deckCount[i] != g2.deckCount[i]) {
			return 1;
		}
	}
	for (i = 0; i < players; i++) {
		for (j = 0; j < MAX_HAND; j++) {
			if (g1.discard[i][j] != g2.discard[i][j]) {
				return 1;
			}
		}
		if (g1.discardCount[i] != g2.discardCount[i]) {
			return 1;
		}
	}
	return 0;
}

int main() {
	printf("\n----------Testing Village card----------\n");

	// Initialize variables
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0, i, errors = 0;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// Initialize game state (this will remain the same throughout and be used to test against)
	initializeGame(numPlayers, k, seed, &G);
	
	// Set first card in first player's hand to be village
	G.hand[thisPlayer][handpos] = village;
	G.handCount[thisPlayer] = 1;
	
	// Initialize deck of first player
	for (i = 0; i < 5; i++) {
		G.deck[thisPlayer][i] = copper;
	}
	G.deckCount[thisPlayer] = 5;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play Village card
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test hand has 1 card
	errors += assertTrue(testG.handCount[thisPlayer], 1, "village: Player hand contains 1 card.");
	
	// Test card comes from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 4, "village: Player deck contains 1 less card.");
	errors += assertTrue(testG.hand[thisPlayer][0], copper, "village: Player hand contains copper card.");

	// Test village card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], village, "village: Played card contains village card.");
	errors += assertTrue(testG.playedCardCount, 1, "village: Played card count = 1.");

	// Test numActions increase by 2
	errors += assertTrue(testG.numActions, 3, "village: Player has 3 actions.");

	// Test no state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "village: Check for other state changes.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End Village card testing--------\n");

	return 0;
}



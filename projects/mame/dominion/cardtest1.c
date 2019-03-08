/* -----------------------------------------------------------------------
* Name: Melissa Ma
* Date: 2/8/19
* Description: Tests the smithy card in dominion.c
* -----------------------------------------------------------------------
* Include the following lines in your makefile:
*
* cardtest1: cardtest1.c dominion.o rngs.o
*      gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

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
	if (g1.numActions != g2.numActions) {
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
	printf("\n----------Testing Smithy card----------\n");

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
	
	// Set first card in first player's hand to be a smithy
	G.hand[thisPlayer][handpos] = smithy;
	G.handCount[thisPlayer] = 1;
	
	// Initialize deck of first player
	for (i = 0; i < 10; i++) {
		G.deck[thisPlayer][i] = copper;
	}
	G.deckCount[thisPlayer] = 10;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play Smithy card
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test current player receives 3 cards
	errors += assertTrue(testG.handCount[thisPlayer], 3, "smithy: Player hand contains 3 cards.");

	// Test 3 cards come from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 7, "smithy: Player deck contains 3 less cards.");
	for (i = 0; i < 3; i++) {
		errors += assertTrue(testG.hand[thisPlayer][i], copper, "smithy: Player hand contains card from own deck.");
	}

	// Test smithy card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], smithy, "smithy: Played card contains smithy card.");
	errors += assertTrue(testG.playedCardCount, 1, "smithy: Played card count = 1.");

	// Test no state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "smithy: Check for other state changes.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n----------End Smithy card testing----------\n");

	return 0;
}



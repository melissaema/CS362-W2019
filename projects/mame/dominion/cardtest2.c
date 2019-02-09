/* -----------------------------------------------------------------------
* Name: Melissa Ma
* Date: 2/9/19
* Description: Tests the adventurer card in dominion.c
* -----------------------------------------------------------------------
* Include the following lines in your makefile:
*
* cardtest2: cardtest2.c dominion.o rngs.o
*      gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"

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
	for (i = 1; i < players; i++) {
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
	printf("\n----------Testing Adventurer card----------\n");

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
	
	// Case 1: Deck has at least 2 treasures.
	printf("\n---------Case 1: Deck has at least 2 treasures---------\n");

	// Set first card in first player's hand to be adventurer
	G.hand[thisPlayer][handpos] = adventurer;
	G.handCount[thisPlayer] = 1;
	
	// Initialize deck of first player
	G.deck[thisPlayer][0] = village;
	for (i = 1; i < 5; i++) {
		G.deck[thisPlayer][i] = copper;
	}
	G.deckCount[thisPlayer] = 5;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play Adventurer card
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test hand has 2 treasures/cards only
	errors += assertTrue(testG.handCount[thisPlayer], 2, "adventurer: Player hand contains 2 cards.");

	// Test 2 cards come from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 3, "adventurer: Player deck contains 2 less cards.");
	errors += assertTrue(testG.hand[thisPlayer][0], copper, "adventurer: Player hand contains card from own deck.");
	errors += assertTrue(testG.hand[thisPlayer][1], copper, "adventurer: Player hand contains card from own deck.");

	// Test adventurer card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], adventurer, "adventurer: Played card contains adventurer card.");
	errors += assertTrue(testG.playedCardCount, 1, "adventurer: Played card count = 1.");

	// Test village is in discard pile
	errors += assertTrue(testG.discard[thisPlayer][0], village, "adventurer: Discard pile contains village card.");
	errors += assertTrue(testG.discardCount[thisPlayer], 1, "adventurer: Discard pile contains 1 card.");

	// Test no state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "adventurer: Check for other state changes.");

	// Case 2: Deck only has 1 treasure, discard has none.
	printf("\n---------Case 2: Deck only has 1 treasure, discard has none---------\n");

	// Set first card in first player's hand to be adventurer
	G.hand[thisPlayer][handpos] = adventurer;
	G.handCount[thisPlayer] = 1;

	// Initialize deck of first player
	G.deck[thisPlayer][0] = village;
	G.deck[thisPlayer][1] = silver;
	G.deckCount[thisPlayer] = 2;

	// Initialize discard of first player
	G.discard[thisPlayer][0] = smithy;
	G.discardCount[thisPlayer] = 1;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play Adventurer card
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test hand has 1 treasure/card only
	errors += assertTrue(testG.handCount[thisPlayer], 1, "adventurer: Player hand contains 1 card.");

	// Test 2 cards come from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 0, "adventurer: Player deck contains no more cards (all drawn).");
	errors += assertTrue(testG.hand[thisPlayer][0], silver, "adventurer: Player hand contains card from own deck.");

	// Test adventurer card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], adventurer, "adventurer: Played card contains adventurer card.");
	errors += assertTrue(testG.playedCardCount, 1, "adventurer: Played card count = 1.");

	// Test 2 cards in discard pile (village and smithy)
	errors += assertTrue(testG.discardCount[thisPlayer], 2, "adventurer: Discard pile contains 2 cards.");

	// Test no state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "adventurer: Check for other state changes.");
	
	// Case 3: Deck has 1 treasure, discard has 2. Need to shuffle
	printf("\n---------Case 3: Deck has 1 treasure, discard has 2.---------\n");

	// Set first card in first player's hand to be adventurer
	G.hand[thisPlayer][handpos] = adventurer;
	G.handCount[thisPlayer] = 1;

	// Initialize deck of first player
	G.deck[thisPlayer][0] = village;
	G.deck[thisPlayer][1] = gold;
	G.deckCount[thisPlayer] = 2;

	// Initialize discard of first player
	G.discard[thisPlayer][0] = gold;
	G.discard[thisPlayer][1] = gold;
	G.discardCount[thisPlayer] = 2;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play Adventurer card
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test hand has 2 treasures/cards
	errors += assertTrue(testG.handCount[thisPlayer], 1, "adventurer: Player hand contains 2 cards.");

	// Test 2 cards come from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 1, "adventurer: Player deck contains 1 card.");
	errors += assertTrue(testG.hand[thisPlayer][0], gold, "adventurer: Player hand contains card from own deck.");
	errors += assertTrue(testG.hand[thisPlayer][1], gold, "adventurer: Player hand contains card from own deck.");

	// Test adventurer card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], adventurer, "adventurer: Played card contains adventurer card.");
	errors += assertTrue(testG.playedCardCount, 1, "adventurer: Played card count = 1.");

	// Test village is in discard pile
	errors += assertTrue(testG.discard[thisPlayer][0], village, "adventurer: Discard pile contains village card.");
	errors += assertTrue(testG.discardCount[thisPlayer], 1, "adventurer: Discard pile contains 1 card.");

	// Test no state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "adventurer: Check for other state changes.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End Adventurer card testing--------\n");

	return 0;
}



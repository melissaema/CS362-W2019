/* -----------------------------------------------------------------------
* Name: Melissa Ma
* Date: 2/9/19
* Description: Tests the council_room card in dominion.c
* -----------------------------------------------------------------------
* Include the following lines in your makefile:
*
* cardtest4: cardtest4.c dominion.o rngs.o
*      gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "council_room"

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
	int i;
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

	return 0;
}

int main() {
	printf("\n----------Testing council room card----------\n");

	// Initialize variables
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0, i, errors = 0;
	int seed = 1000;
	int numPlayers = 3;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// Initialize game state (this will remain the same throughout and be used to test against)
	initializeGame(numPlayers, k, seed, &G);
	
	// Set first card in first player's hand to be village
	G.hand[thisPlayer][handpos] = council_room;
	G.handCount[thisPlayer] = 1;

	// Set other 2 players' hands to 1 card (smithy)
	G.hand[1][0] = smithy;
	G.handCount[1] = 1;
	G.hand[2][0] = smithy;
	G.handCount[2] = 1;
	
	// Initialize deck of first player
	for (i = 0; i < 5; i++) {
		G.deck[thisPlayer][i] = copper;
	}
	G.deckCount[thisPlayer] = 5;

	// Initialize decks of other 2 players
	for (i = 0; i < 5; i++) {
		G.deck[1][i] = silver;
		G.deck[2][i] = gold;
	}
	G.deckCount[1] = 5;
	G.deckCount[2] = 5;

	// Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// Play council_room card
	cardEffect(council_room, choice1, choice2, choice3, &testG, handpos, &bonus);

	// Test Player 0 hand has 4 cards
	errors += assertTrue(testG.handCount[thisPlayer], 4, "council_room: Player 0 hand contains 4 cards.");
	
	// Test card comes from own deck
	errors += assertTrue(testG.deckCount[thisPlayer], 1, "council_room: Player 0 deck contains 4 less cards.");
	for (i = 0; i < 4; i++) {
		errors += assertTrue(testG.hand[thisPlayer][i], copper, "council_room: Player 0 hand contains copper card.");
	}

	// Test council_room card is in playedCard pile
	errors += assertTrue(testG.playedCards[0], council_room, "council_room: Played card contains council_room card.");
	errors += assertTrue(testG.playedCardCount, 1, "council_room: Played card count = 1.");

	// Test numBuys increase by 1
	errors += assertTrue(testG.numBuys, 2, "council_room: Player 0 has 2 buys.");

	// Test other 2 players have drawn a card from their own deck.
	errors += assertTrue(testG.handCount[1], 2, "council_room: Player 1 hand contains 2 cards.");
	errors += assertTrue(testG.handCount[2], 2, "council_room: Player 2 hand contains 2 cards.");
	errors += assertTrue(testG.hand[1][0], smithy, "council_room: Player 1 hand contains smithy.");
	errors += assertTrue(testG.hand[1][1], silver, "council_room: Player 1 hand contains silver.");
	errors += assertTrue(testG.hand[2][0], smithy, "council_room: Player 2 hand contains smithy.");
	errors += assertTrue(testG.hand[2][1], gold, "council_room: Player 2 hand contains gold.");

	// Test no other state change for other players and the victory card and kingdom card piles
	errors += assertTrue(testStateChange(G, testG, numPlayers), 0, "council_room: Check for other state changes.");

	printf("\nTotal Errors: %d\n", errors);

	printf("\n--------End council room card testing--------\n");

	return 0;
}



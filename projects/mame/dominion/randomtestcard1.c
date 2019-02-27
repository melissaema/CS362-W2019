/* -----------------------------------------------------------------------
* Name: Melissa Ma
* Date: 2/25/19
* Description: Tests the smithy card in dominion.c (Random)
* -----------------------------------------------------------------------
* Include the following lines in your makefile:
*
* randomtestcard1: randomtestcard1.c dominion.o rngs.o
*      gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
* -----------------------------------------------------------------------
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

int assertEquals(int actual, int expected, char *msg)
{
    if (actual == expected) {
        return 0;
    }
    printf("FAIL: %s, Actual: %d, Expected: %d\n", msg, actual, expected);
    return 1;
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
    for (i = 0; i < players; i++) {
        if (i != g1.whoseTurn) {
            for (j = 0; j < MAX_HAND; j++) {
                if (g1.hand[i][j] != g2.hand[i][j]) {
                    return 1;
                }
            }
            if (g1.handCount[i] != g2.handCount[i]) {
                return 1;
            }
        }
    }
    for (i = 0; i < players; i++) {
        if (i != g1.whoseTurn) {
            for (j = 0; j < MAX_HAND; j++) {
                if (g1.deck[i][j] != g2.deck[i][j]) {
                    return 1;
                }
            }
            if (g1.deckCount[i] != g2.deckCount[i]) {
                return 1;
            }
        }
    }
    for (i = 0; i < players; i++) {
        if (i != g1.whoseTurn) {
            for (j = 0; j < MAX_HAND; j++) {
                if (g1.discard[i][j] != g2.discard[i][j]) {
                    return 1;
                }
            }
            if (g1.discardCount[i] != g2.discardCount[i]) {
                return 1;
            }
        }
    }
    return 0;
}

// initializenumPlayers randomly selects the number of players (2-4) and returns it
int initializenumPlayers() {
    // Initialize random number of players (min = 2, max = 4)
    int p = rand() % (4 + 1 - 2) + 2;
    return p;
}

// initializeKingdomCards initializes the 10 Kingdom cards used in play
void initializeKingdomCards(int k[]) {
    int i, j, num;
    int same = 0;
    int redo = 1;
    for (i = 1; i < 10; i++) {
        num = rand() % (treasure_map + 1 - council_room) + council_room;
        redo = 1;
        // Get new number if duplicate
        while (redo == 1) {
            same = 0;
            for (j = 0; j < i; j++) {
                if (num == k[j]) {
                    num = rand() % (treasure_map + 1 - council_room) + council_room;
                    same++;
                }
            }
            if (same > 0) {
                redo = 1;
            }
            else {
                redo = 0;
            }
        }
        k[i] = num;
    }
}

// randomizeGame randomly selects whoseTurn, sets up a deck, hand, and discard pile for each player
void randomizeGame(struct gameState *G) {
    int i, j;
    int nPlayer = G->numPlayers;

    // Randomize whose turn it is
    G->whoseTurn = rand() % nPlayer;

    // Set up random deck for each player
    for (i = 0; i < nPlayer; i++) {
        G->deckCount[i] = rand() % (MAX_DECK - 1 + 1 - 2) + 2;
        for (j = 0; j < G->deckCount[i]; j++) {
            G->deck[i][j] = rand() % (treasure_map + 1 - curse) + curse;
        }
    }

    // Set up random hand for each player
    for (i = 0; i < nPlayer; i++) {
        G->handCount[i] = rand() % (MAX_HAND + 1 - 1) + 1;
        if (G->handCount[i] == 0) {
            G->handCount[i]++;
        }
        for (j = 0; j < G->handCount[i]; j++) {
            G->hand[i][j] = rand() % (treasure_map + 1 - curse) + curse;
        }
    }

    // Set up random discard for each player
    for (i = 0; i < nPlayer; i++) {
        G->discardCount[i] = rand() % (MAX_DECK - 1 + 1 - 0) + 0;
        for (j = 0; j < G->discardCount[i]; j++) {
            G->discard[i][j] = rand() % (treasure_map + 1 - curse) + curse;
        }
    }
}

// randomizePlayedCards randomly fills the played cards pile
void randomizePlayedCards(struct gameState *G) {
    G->playedCardCount = rand() % (MAX_DECK - 1 + 1 - 0) + 0;

    int i;
    for (i = 0; i < G->playedCardCount; i++) {
        G->playedCards[i] = rand() % (treasure_map + 1 - curse) + curse;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    int i;
    int errors = 0;

    // Kingdom cards
    int kingdomCards[10];

    // Games(original and test)
    struct gameState G, testG;

    int continueTest = 1; // Keep testing if = 1
    int counter = 0;
    int numTests = 0;

    // Timekeeping
    double timekeep = 0; // for total running time
    clock_t start = clock(); // start

    printf("---------------------Testing Smithy Card----------------------\n");

    while (continueTest) {
        errors = 0;

        // Get number of players
        int numPlayers = initializenumPlayers();

        // Initialize kingdom cards randomly
        kingdomCards[0] = smithy; // Because we are testing smithy card
        initializeKingdomCards(kingdomCards);

        // Initialize original game
        initializeGame(numPlayers, kingdomCards, 1000, &G);

        // Randomize game
        randomizeGame(&G);
        randomizePlayedCards(&G);

        int turn = G.whoseTurn;

        // Place smithy card in last slot of player's hand
        int handPos = G.handCount[turn] - 1;
        G.hand[turn][handPos] = smithy;

        // Copy everything from original game into test game
        memcpy(&testG, &G, sizeof(struct gameState));

        // Get original count of smithy cards in played cards
        int origSmt = 0;
        for (i = 0; i < G.playedCardCount; i++) {
            if (G.playedCards[i] == smithy) {
                origSmt++;
            }
        }

        // Get original deck and discard counts
        int origDD = G.deckCount[turn] + G.discardCount[turn];

        // Make sure it goes through 2-4 players
        if (counter == 0 && numPlayers == 2 && numTests >= 100) {
            counter++;
        }
        if (counter == 1 && numPlayers == 3 && numTests >= 200) {
            counter++;
        }
        if (counter == 2 && numPlayers == 4 && numTests >= 300) {
            counter++;
            continueTest = 0;
        }

        cardEffect(smithy, 0, 0, 0, &testG, handPos, 0);

        // TEST RESULTS OF PLAYING CARD
        numTests++;
        printf("\nTest #%d: %d Players, Player %d's turn, Deck Count: %d, Hand Count: %d, Discard Count: %d\n", numTests, numPlayers, G.whoseTurn, G.deckCount[turn], G.handCount[turn], G.discardCount[turn]);

        // Test player's hand has 3 new cards
        errors += assertEquals(testG.handCount[turn], G.handCount[turn] + 2, "smithy: Player has 3 new cards in hand"); // additional 2 because should discard smithy card

        // Test smithy card is not in hand at handPos anymore
        if (testG.hand[turn][handPos] == smithy && (testG.handCount[turn] == G.handCount[turn] + 3)) {
            printf("FAIL: smithy: smithy is still in hand.\n");
            errors++;
        }

        // Test played cards pile gets smithy card
        errors += assertEquals(testG.playedCardCount, G.playedCardCount + 1, "smithy: Played cards has 1 more card");

        // Get test count of smithy cards in played cards
        int testSmt = 0;
        for (i = 0; i < testG.playedCardCount; i++) {
            if (testG.playedCards[i] == smithy) {
                testSmt++;
            }
        }

        errors += assertEquals(testSmt, origSmt + 1, "smithy: Played cards has 1 more smithy card");

        // Check deck + discard count decremented by 3 cards
        int testDD = testG.deckCount[turn] + testG.discardCount[turn];
        errors += assertEquals(testDD, origDD - 3, "smithy: Deck + Discard decremented by 3 cards");

        // Test no state change for other players and the victory card and kingdom card piles
        errors += assertEquals(testStateChange(G, testG, numPlayers), 0, "smithy: Check for other state changes.");

        // Print total errors
        printf("Errors: %d\n", errors);
    }

    // Calculate run time
    clock_t end = clock();
    timekeep += (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTotal run time for smithy random testing: %f seconds\n\n", timekeep);

    return 0;
}

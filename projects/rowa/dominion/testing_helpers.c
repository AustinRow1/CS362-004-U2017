#include "./testing_helpers.h"
#include "./dominion_helpers.h"
#include "./dominion.h"
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_TRACKER_CAPACITY 10

void _doubleTrackerCapacity(struct StatusTracker*);
void _doubleTrackerContextCapacity(struct StatusTracker*);
void _addTestToTracker(struct StatusTracker*, Status, char*);
void _printAssertionResult(struct StatusTracker, int);
const char* _statusToString(Status);

void _doubleTrackerCapacity(struct StatusTracker* tracker) {
	int i;
	int newCapacity = tracker->_capacity * 2;
	char** newStatusReasons = (char**)malloc(sizeof(char*) * newCapacity);
	Status* newStatuses = (Status*)malloc(sizeof(Status*) * newCapacity); 

	for(i = 0; i < tracker->numTests; i++) {
		newStatuses[i] = tracker->_statuses[i];
		newStatusReasons[i] = (char*)malloc(sizeof(char) * (strlen(tracker->_statusReasons[i]) + 1));
		strcpy(newStatusReasons[i], tracker->_statusReasons[i]);
		free(tracker->_statusReasons[i]);
	}

	free(tracker->_statuses);
	free(tracker->_statusReasons);

	tracker->_capacity = newCapacity;
	tracker->_statuses = newStatuses;
	tracker->_statusReasons = newStatusReasons;
}

void _doubleTrackerContextCapacity(struct StatusTracker* tracker) {
	int i;
	int newCapacity = tracker->_contextCapacity * 2;
	char** newContexts = (char**)malloc(sizeof(char*) * newCapacity);
	int* newContextMap = (int*)malloc(sizeof(int) * newCapacity);

	for(i = 0; i < tracker->numContexts; i++) {
		newContexts[i] = (char*)malloc(sizeof(char) * (strlen(tracker->_contexts[i]) + 1));
		strcpy(newContexts[i], tracker->_contexts[i]);
		free(tracker->_contexts[i]);
		newContextMap[i] = tracker->_contextIndexToStatusIndex[i];
	}

	free(tracker->_contexts);
	free(tracker->_contextIndexToStatusIndex);

	tracker->_contextCapacity = newCapacity;
	tracker->_contexts = newContexts;
	tracker->_contextIndexToStatusIndex = newContextMap;
}

void _addTestToTracker(struct StatusTracker* tracker, Status status, char* reason) {
	if(tracker->_capacity == tracker->numTests) {
		_doubleTrackerCapacity(tracker);
	}

	tracker->_statuses[tracker->numTests] = status;
	tracker->_statusReasons[tracker->numTests] = (char*)malloc(sizeof(char) * (strlen(reason) + 1));
	strcpy(tracker->_statusReasons[tracker->numTests], reason);
	tracker->numTests++;
}

void initStatusTracker(struct StatusTracker* tracker) {
	tracker->_capacity = INITIAL_TRACKER_CAPACITY;
	tracker->_overallStatus = PASSED;
	tracker->_statuses = (Status*)malloc(sizeof(Status) * tracker->_capacity);
	tracker->_statusReasons = (char**)malloc(sizeof(char*) * tracker->_capacity);
	tracker->numTests = 0;

	tracker->_contextCapacity = INITIAL_TRACKER_CAPACITY;
	tracker->_contextIndexToStatusIndex = (int*)malloc(sizeof(int) * tracker->_contextCapacity);
	tracker->_contexts = (char**)malloc(sizeof(char*) * tracker->_contextCapacity);
	tracker->numContexts = 0;
}

void destroyStatusTracker(struct StatusTracker* tracker) {
	int i;

	for(i = 0; i < tracker->numTests; i++) {
		free(tracker->_statusReasons[i]);
	}

	free(tracker->_statusReasons);
	free(tracker->_statuses);
}

void addContextToTracker(char* context, struct StatusTracker* tracker) {
	if(tracker->_contextCapacity == tracker->numContexts) {
		_doubleTrackerContextCapacity(tracker);
	}

	tracker->_contexts[tracker->numContexts] = (char*)malloc(sizeof(char) * (strlen(context) + 1));
	strcpy(tracker->_contexts[tracker->numContexts], context);
	tracker->_contextIndexToStatusIndex[tracker->numContexts] = tracker->numTests;
	tracker->numContexts++;
}

void assertTrue(int condition, char* description, struct StatusTracker* tracker) {
	if(condition) {
		passTest(tracker, description);
	} else {
		failTest(tracker, description);
	}
}	

void failTest(struct StatusTracker* tracker, char* reason)  {
	tracker->_overallStatus = FAILED;
	_addTestToTracker(tracker, FAILED, reason);
}	

void passTest(struct StatusTracker* tracker, char* reason) {
	_addTestToTracker(tracker, PASSED, reason);
}

void printTestResults(struct StatusTracker tracker) {
	int numPassed = 0, printedContexts = 0, i;

	for(i = 0; i < tracker.numTests; i++) {
		if(tracker._statuses[i] == PASSED) {
			numPassed++;
		}
	}	

	if(tracker._overallStatus == PASSED) {
		printf("TEST SUCCESSFULLY COMPLETED\nALL TESTS PASSED (%d)\n", tracker.numTests);
	} else {
		printf("TEST FAILED\nPassed Assertions: %d\nFailed Assertions: %d\n", numPassed, tracker.numTests - numPassed);
	}

	for(i = 0; i < tracker.numTests; i++) {
		printf("\t");
		if(printedContexts < tracker.numContexts && i == tracker._contextIndexToStatusIndex[printedContexts]) {
			printf("--SUB TEST %d: %s\n\t", printedContexts + 1, tracker._contexts[printedContexts]);
			printedContexts++;
		}
		_printAssertionResult(tracker, i);
	}

}

void _printAssertionResult(struct StatusTracker tracker, int testIndex) {
	printf("(%d) %s", testIndex + 1, _statusToString(tracker._statuses[testIndex]));
	if(tracker._statuses[testIndex] == FAILED) {
		printf("*");
	}

	printf(": %s\n", tracker._statusReasons[testIndex]);
}	

const char* _statusToString(Status status) {
	if(status == 0) {
		return "FAILED";
	}

	return "PASSED";
}
///////////////////////////////////////////////////////////////////

void initializeTestGame(int numPlayers, const int hands[][MAX_HAND], const int* handSizes, const int decks[][MAX_DECK], const int* deckSizes, const int* chosenKingdomCards, int numKingdomCards, int randomSeed, struct gameState* state) {
	int i;

	//set players' hands and decks and clear everything else
	for(i = 0; i < MAX_PLAYERS; i++) {
		memset(state->hand[i], 0, sizeof(int) * MAX_HAND);
		memset(state->deck[i], 0, sizeof(int) * MAX_DECK);
		memset(state->discard[i], 0, sizeof(int) * MAX_DECK);
	}	

	memset(state->handCount, 0, sizeof(int) * MAX_PLAYERS);
	memset(state->deckCount, 0, sizeof(int) * MAX_PLAYERS);
	memset(state->discardCount, 0, sizeof(int) * MAX_PLAYERS);
	memset(state->playedCards, 0, sizeof(int) * MAX_DECK);
	memset(state->supplyCount, 0, sizeof(int) * treasure_map);
	memset(state->embargoTokens, 0, sizeof(int) * treasure_map);


	for(i = 0; i < numPlayers; i++) {
		memcpy(state->hand[i], hands[i], sizeof(int) * handSizes[i]);
		memcpy(state->deck[i], decks[i], sizeof(int) * deckSizes[i]);
		state->handCount[i] = handSizes[i];
		state->deckCount[i] = deckSizes[i];
	}

	//set supply stacks	
	for(i = 0; i < numKingdomCards; i++) {
		state->supplyCount[chosenKingdomCards[i]] = 10;
	}

	state->numPlayers = numPlayers;
	state->outpostPlayed = 0;
	state->outpostTurn = 0;
	state->phase = 0;
	state->numActions = 1;
	state->coins = 0;
	state->numBuys = 1;
	state->playedCardCount = 0;
	state->whoseTurn = 0;

	//set up random number generator
	SelectStream(1);
	PutSeed((long)randomSeed);
}

int numCardIn(const int card, const int* collection, const int size) {
	int i, count = 0;
	
	for(i = 0; i < size; i++) {
		if(collection[i] == card) {
			count++;
		}
	}	

	return count;
}

int findInHand(int card, struct gameState *state) {
	int i;

	for(i = 0; i < numHandCards(state); i++) {
		if(handCard(i, state) == card) {
			return i;
		}
	}

	return -1;
}

int fullDeckValue(int player, struct gameState state) {
	int i, val = 0;

	for(i = 0; i < state.handCount[player]; i++) {
		val += getCost(state.hand[player][i]);
	}

	for(i = 0; i < state.deckCount[player]; i++) {
		val += getCost(state.deck[player][i]);
	}	

	for(i = 0; i < state.discardCount[player]; i++) {
		val += getCost(state.discard[player][i]);
	}

	return val;
}

int onlyGainedCardNumTimes(const int card, const int numTimes, const int* preGain, const int preSize, const int* postGain, const int postSize) {
	int i;

	for(i = 0; i < treasure_map; i++) {
		if(i == card) {
		   	if(numCardIn(i, preGain, preSize) + numTimes != numCardIn(i, postGain, postSize)) {
				return 0;
			}
		} else if(numCardIn(i, preGain, preSize) != numCardIn(i, postGain, postSize)) {
			return 0;
		}	
	}

	return 1;
	
}

int sameCompositionOfCards(const int* collection1, const int* collection2, const int size) {
	int i;
	int cardCounts[treasure_map] = { 0 };

	for(i = 0; i < size; i++) {
		cardCounts[collection1[i]]++;
	  	cardCounts[collection2[i]]--;	
	}

	for(i = 0; i < treasure_map; i++) {
		if(cardCounts[i] != 0) {
			return 0;
		}
	}

	return 1;
}

int sameOrderedCollectionOfCards(const int* collection1, const int* collection2, const int size) {
	int i;
	for(i = 0; i < size; i++) {
		if(collection1[i] != collection2[i]) {
			return 0;
		}
	}

	return 1;
}

int treasureInHand(struct gameState state, int player) {
	int found = 0, i;

	for(i = 0; i < state.handCount[player]; i++) {
		if(treasure(state.hand[player][i])) {
			found++;
		}	
	}

	return found;
}

int treasure(int card) {
	return card >= copper && card <= gold;
}

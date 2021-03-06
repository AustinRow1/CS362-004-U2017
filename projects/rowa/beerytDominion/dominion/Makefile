
SRCS := dominion cardEffects rngs
UNIT_TESTS := unittest1.o unittest2.o unittest3.o unittest4.o \
			cardtest1.o cardtest2.o cardtest3.o cardtest4.o
UNITY := Unity/src/unity.c Unity/extras/fixture/src/unity_fixture.c

SRC_OBJS := $(SRCS:=.o)
TEST_OBJS := unity.o unity_fixture.o

CC := gcc
CFLAGS = -g -Wall -fpic -coverage -IUnity/src -IUnity/extras/fixture/src
LDLIBS := -lm

playdom: playdom.o $(SRC_OBJS)
	gcc $(CFLAGS) -o playdom $^ $(LDLIBS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

dominion.o: dominion.h dominion_helpers.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS) $(LDLIBS)

testDrawCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o cardEffects.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o cardEffects.o rngs.o $(CFLAGS)

testAll: dominion.o cardEffects.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o cardEffects.o rngs.o $(CFLAGS)

unity.o: Unity/src/unity.c
	$(CC) $(CFLAGS) -c $^

unity_fixture.o: Unity/extras/fixture/src/unity_fixture.c
	$(CC) $(CFLAGS) -c $^

unittestresults.out: $(UNIT_TESTS) $(TEST_OBJS) $(SRC_OBJS)
	gcc $(CFLAGS) -o unittest1.exe unittest1.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest2.exe unittest2.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest3.exe unittest3.o $(TEST_OBJS) $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o unittest4.exe unittest4.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest1.exe cardtest1.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest2.exe cardtest2.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest3.exe cardtest3.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)
	gcc $(CFLAGS) -o cardtest4.exe cardtest4.o unity.o unity_fixture.o $(SRC_OBJS) $(LDLIBS)

	echo "Running Unit Tests..." > unittestresults.out

	echo "\nRunning Unit Test 1" >> unittestresults.out
	@./unittest1.exe >> unittestresults.out
	@gcov unittest1.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 2" >> unittestresults.out
	@./unittest2.exe >> unittestresults.out
	@gcov unittest2.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 3" >> unittestresults.out
	@./unittest3.exe >> unittestresults.out
	@gcov unittest3.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Unit Test 4" >> unittestresults.out
	@./unittest4.exe >> unittestresults.out
	@gcov unittest4.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 1" >> unittestresults.out
	@./cardtest1.exe >> unittestresults.out
	@gcov cardtest1.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 2" >> unittestresults.out
	@./cardtest2.exe >> unittestresults.out
	@gcov cardtest2.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 3" >> unittestresults.out
	@./cardtest3.exe >> unittestresults.out
	@gcov cardtest3.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out

	echo "\nRunning Card Test 4" >> unittestresults.out
	@./cardtest4.exe >> unittestresults.out
	@gcov cardtest4.c $(SRCS:=.c) >> unittestresults.out
	@echo "----------------------------------------------" >> unittestresults.out


interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o cardEffects.o rngs.o interface.o $(CFLAGS)

testing_helpers.o: testing_helpers.c testing_helpers.h
	gcc -c testing_helpers.c -g $(CFLAGS)

rowarandomtestadventurer: rowarandomtestadventurer.c testing_helpers.o dominion.o cardEffects.o rngs.o
	gcc -o rowarandomtestadventurer rowarandomtestadventurer.c -g cardEffects.o testing_helpers.o dominion.o rngs.o $(CFLAGS) $(LDLIBS)

rowarandomtestsmithy: rowarandomtestsmithy.c testing_helpers.o dominion.o cardEffects.o rngs.o
	gcc -o rowarandomtestsmithy rowarandomtestsmithy.c -g cardEffects.o testing_helpers.o dominion.o rngs.o $(CFLAGS) $(LDLIBS)

rowarandomtestadventurer.out: rowarandomtestadventurer
	./rowarandomtestadventurer &> rowarandomtestadventurer.out		
	gcov dominion.c >> rowarandomtestadventurer.out
	cat dominion.c.gcov >> rowarandomtestadventurer.out

rowarandomtestsmithy.out: rowarandomtestsmithy
	./rowarandomtestsmithy &> rowarandomtestsmithy.out		
	gcov dominion.c >> rowarandomtestsmithy.out
	cat dominion.c.gcov >> rowarandomtestsmithy.out

runrowatests:rowarandomtestsmithy.out rowarandomtestadventurer.out

all: playdom player testDrawCard testBuyCard badTestDrawCard rowarandomtestsmithy rowarandomtestadventurer

clean:
	rm -f *.exe *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe rowarandomtestsmithy rowarandomtestadventurer

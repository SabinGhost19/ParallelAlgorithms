compile: raceCondition.c raceCondition-seq.c barrier.c barrier-seq.c semaphoreSignal.c semaphoreSignal-seq.c deadLock1.c deadLock2.c
	gcc -o raceCondition raceCondition.c -lm -lpthread
	gcc -o raceCondition-seq raceCondition-seq.c -lm -lpthread
	gcc -o barrier barrier.c -lm -lpthread
	gcc -o barrier-seq barrier-seq.c -lm -lpthread
	gcc -o semaphoreSignal-seq semaphoreSignal-seq.c -lm -lpthread
	gcc -o semaphoreSignal semaphoreSignal.c -lm -lpthread
	gcc -o deadLock1 deadLock1.c -lm -lpthread
	gcc -o deadLock2 deadLock2.c -lm -lpthread






clean:
	rm raceCondition raceCondition-seq barrier barrier-seq semaphoreSignal semaphoreSignal-seq deadLock1 deadLock2
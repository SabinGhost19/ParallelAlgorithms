compile: helloWorld.c addVectors-seq.c addVectors-par.c
	gcc -o helloWorld helloWorld.c -lm -O3 -lpthread
	gcc -o addVectors-seq addVectors-seq.c -lm -O3 -lpthread
	gcc -o addVectors-par addVectors-par.c -lm -O3 -lpthread


clean:
	rm helloWorld addVectors-seq addVectors-par
compile: multiplyMatrices-seq.c multiplyMatrices-out.c multiplyMatrices-mid.c multiplyMatrices-in.c
	gcc -o multiplyMatrices-out multiplyMatrices-out.c -lm -lpthread
	gcc -o multiplyMatrices-seq multiplyMatrices-seq.c -lm -lpthread
	gcc -o multiplyMatrices-mid multiplyMatrices-mid.c -lm -lpthread
	gcc -o multiplyMatrices-in multiplyMatrices-in.c -lm -lpthread



clean:
	rm multiplyMatrices-out multiplyMatrices-seq multiplyMatrices-mid multiplyMatrices-in
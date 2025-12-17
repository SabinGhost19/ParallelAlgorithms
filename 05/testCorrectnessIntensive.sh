#!/bin/bash

seq_program=$1
par_program=$2
N=$3
NumTests=$4
printLevel=2
if [ -n "$5" ]; then
	runs=$5
else
	runs="1 2 4 8"
fi

rm out*
./$seq_program $N $printLevel 1 > out
# we add 1 here just so we can use a par_program instead of a missing sequential one
echo The result of your parallel program is
echo ======================================
cat out
echo ======================================
echo Running intensive correctness test with $P threads
for i in `seq 1 $NumTests`; do
	echo Test $i/$NumTests
	for P in $runs; do
		./$par_program $N $printLevel $P > out.$i.$P 
	done
done

diff -q --from-file out out.*
if [ $? -eq 0 ]; then
	echo Output correct on intensive test
	rm out*
fi

#seq
echo "START: sequential program: $seq_program"
total_seq=0
for i in `seq 1 $NumTests`; do
    echo "  Test $i/$NumTests"
    start=$(date +%s.%N)
    ./$seq_program $N $printLevel 1 > /dev/null
    end=$(date +%s.%N)
    exec_time=$(echo "$end - $start" | bc)
    total_seq=$(echo "$total_seq + $exec_time" | bc)
done
avg_seq=$(echo "scale=6; $total_seq / $NumTests" | bc)
echo "Average time for sequential: ${avg_seq}s"
echo ""

#par
echo "START: parallel program: $par_program"
for P in $runs; do
    echo "Running parallel program with $P threads..."
    total_par=0
    for i in `seq 1 $NumTests`; do
        echo "  Test $i/$NumTests"
        start=$(date +%s.%N)
        ./$par_program $N $printLevel $P > /dev/null
        end=$(date +%s.%N)
        exec_time=$(echo "$end - $start" | bc)
        total_par=$(echo "$total_par + $exec_time" | bc)
    done
    avg_par=$(echo "scale=6; $total_par / $NumTests" | bc)
    speedup=$(echo "scale=3; $avg_seq / $avg_par" | bc)
    echo "Average time for $P threads: ${avg_par}s (speedup: ${speedup}x)"
    echo ""
done
#!/bin/bash
## copy your code "main.c" in this testing directory
## Command to run
## ./verify.sh main.c
## DO NOT MODIFY THIS SCRIPT
PROGRAM=$1
THREAD_COUNTS=(1 2 4 8)
POINTS_PER_TEST=(50 40 10) 
SPEEDUP_THRESHOLD=(1.5 1.75 2.5)
# Test files - each test has 2 input files
INPUT_FILES_1=("in1_1.txt" "in1_2.txt" "in1_3.txt" )
INPUT_FILES_2=("in2_1.txt" "in2_2.txt" "in2_3.txt" )
OUTPUT_FILES=("out1.txt" "out2.txt" "out3.txt" )
total_points=0
rm -rf temp
echo "Compiling $PROGRAM..."
gcc -o main "$PROGRAM" -lpthread -lm -O3
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    echo "Total points: 0"
    exit 1
fi
echo "Compilation successful!"
echo "============================================"
mkdir -p temp
for test_index in {0..2}; do
    input_file_1="${INPUT_FILES_1[$test_index]}"
    input_file_2="${INPUT_FILES_2[$test_index]}"
    expected_output="${OUTPUT_FILES[$test_index]}"
    test_points="${POINTS_PER_TEST[$test_index]}"
    
    echo "Running test $((test_index + 1)) with $input_file_1 and $input_file_2..."
    
    if [ ! -f "$input_file_1" ] || [ ! -f "$input_file_2" ]; then
        echo "  ✗ Input files not found!"
        echo "  Points earned: 0"
        echo "--------------------------------------------"
        continue
    fi
    
    correct_outputs=0
    execution_times=()
   
    for threads in "${THREAD_COUNTS[@]}"; do
       
        rm -f temp/*
        
        
        cp "$input_file_1" temp/in1.txt
        cp "$input_file_2" temp/in2.txt
        
       
        cp main temp/
        
        
        start_time=$(date +%s.%N)
        (cd temp && ./main "$threads" > out.txt)
        exit_code=$?
        end_time=$(date +%s.%N)
        
        execution_time=$(echo "$end_time - $start_time" | bc)
        execution_times+=("$execution_time")
        
       
        if [ $exit_code -ne 0 ]; then
            echo "  ✗ $threads thread(s): Program execution failed"
            break
        fi
        
       
        if [ ! -f "temp/out.txt" ]; then
            echo "  ✗ $threads thread(s): Output file not created"
            break
        fi
        
       
        if cmp -s "temp/out.txt" "$expected_output"; then
            echo "  ✓ $threads thread(s): ${execution_time}s - Output correct"
            correct_outputs=$((correct_outputs + 1))
        else
            echo "  ✗ $threads thread(s): Output incorrect"
           
            break
        fi
    done
    
   
    if [ "$correct_outputs" -eq 4 ]; then
       
        t1="${execution_times[0]}"
        t8="${execution_times[3]}"
        
       
        speedup_1_to_8=$(echo "scale=6; $t1 / ($t8 + 0.000001)" | bc -l)
        
        echo "  Speedup 1->8 threads: $speedup_1_to_8"
        
       
        meets_threshold=$(echo "$speedup_1_to_8 >= ${SPEEDUP_THRESHOLD[$test_index]}" | bc -l)
        
        if [ "$meets_threshold" -eq 1 ]; then
            echo "  ✓ Speedup meets threshold of ${SPEEDUP_THRESHOLD[$test_index]}"
            echo "  Points earned: $test_points"
            total_points=$((total_points + test_points))
        else
            echo "  ✗ Speedup does not meet threshold of ${SPEEDUP_THRESHOLD[$test_index]}"
            echo "  Points earned: 0"
        fi
    else
        echo "  Points earned: 0 (not all thread counts passed)"
    fi
    
    echo "--------------------------------------------"
done
rm -rf temp main
echo "============================================"
echo "Total points: $total_points"
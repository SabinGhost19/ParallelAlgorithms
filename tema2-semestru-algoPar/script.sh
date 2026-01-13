#!/bin/bash

# Add .dotnet to PATH just in case
export PATH=$PATH:$HOME/.dotnet

echo "Starting stress test (100 runs)..."

for i in {1..100}; do
    # Run the C# project and redirect output to test/results.txt
    # Using --project to specify the project folder
    dotnet run --project RateLimiterProject > test/results.txt
    
    # Run the python validation script
    # We change directory to test/ so python script finds results.txt easily
    cd test
    OUTPUT=$(python3 test.py)
    cd ..
    
    # Check if the output contains the success message
    if echo "$OUTPUT" | grep -q "No errors detected. All tests passed basic validation."; then
        echo "Run #$i: PASSED"
    else
        echo "Run #$i: FAILED"
        echo "--------------------------------------------------"
        echo "$OUTPUT"
        echo "--------------------------------------------------"
        exit 1
    fi
done

echo "Success! All 100 runs passed."
import os, sys

def parse_file(path):    
	with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), path), "r") as f:
		parsed_input = f.read().split('\n')
	return parsed_input

def is_arithmetic(seq):
    if len(seq) < 2:
        return True  # A single number or empty sequence is trivially arithmetic
    diff = seq[1] - seq[0]
    return all(seq[i+1] - seq[i] == diff for i in range(len(seq)-1))

def main(input):  
    clean = [' '.join(s.split()[1:]) for s in input if len(s.split()) > 1]  # Get rid of useless info
    num_arrays = []
    for s in clean: # Sort numbers into arrays so we can use the set() function later
        numbers = [int(x) for x in s.split()]
        num_arrays.append(numbers)
        
    arr_set_sorted = []
    for arr in num_arrays:  # Sweep 1
        if is_arithmetic(arr):
            print(arr, "is already arithmetic")
        else:
            arr_set_sorted.append(sorted(set(arr)))
            
    for arr in arr_set_sorted: # Sweep 2 after set()
        if is_arithmetic(arr):
            print(arr, "can be reordered to arithmetic:", arr)
        else:
            print(arr, "cannot form an arithmetic sequence")

    return 0

if __name__ == "__main__":
    input = parse_file("permuted_arithmetic_sequence.txt")
    print()
    print(f"Output: {main(input)}")
    
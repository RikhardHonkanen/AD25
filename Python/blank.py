import os, sys

def parse_file(path):    
	with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), path), "r") as f:
		parsed_input = f.read().split('\n')
	return parsed_input

def main(input):      
    answer = 'Part one'            
    return answer

if __name__ == "__main__":
    TEST = 0
    test_input, input = parse_file("0test.txt"), parse_file("0.txt")
    print(f"Output Test: {main(test_input)} (expected {TEST})")
    print()
    print(f"Output: {main(input)}")
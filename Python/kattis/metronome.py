import os, sys

def parse_file(path):    
	with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), path), "r") as f:
		parsed_input = f.read().split('\n')
	return parsed_input

def main():  
    print("Enter song length in ticks: ")
    ticks = input()
    revs = int(ticks)/4

    return "Milo needs to turn the key % s times" % revs

if __name__ == "__main__":
    print(f"Output: {main()}")
    
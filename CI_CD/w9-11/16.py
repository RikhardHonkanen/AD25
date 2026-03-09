'''
1) Make a function to search for integer numbers which have min. 1 and max. 3 digits, in a given string.

 

2) Make a function to validate an IPv4 address. IPv4 format is xxx.xxx.xxx.xxx and xxx shall be between 0 and 255.

 

3) Make a function to replace whitespaces in a string with underscores and vice versa.

 

4) Make a function to validate and convert a date of yyyy-mm-dd format to dd/mm/yyyy format.

 

5) Make a function  to find all words starting with 'a' or 'e' in a given string.
'''

def find_valid_numbers(string):
    curr = ""
    valid = []
    for idx, c in enumerate(string):
        if c.isdigit():
            curr += c
            if (len(string) - 1 == idx and (len(curr) > 0 and len(curr) < 4)):
                valid.append(int(curr))
        else:
            if(len(curr) > 0 and len(curr) < 4):
                valid.append(int(curr))
            curr = ""
            
    print(valid)

def validate_IPv4():
    pass

find_valid_numbers("abc123 xx 1234m1 22")
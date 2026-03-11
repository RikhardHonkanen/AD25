#!/usr/bin/env python3

# Look at: https://docs.python.org/3/howto/regex.html and https://docs.python.org/3/library/re.html

import re
from datetime import datetime

# 1) Make a function to search for integer numbers which have min. 1 and max. 3 digits, in a given string.

def find_1_to_3_digit_integers(text):
    """
    Returns a list of integers (1 to 3 digits) found in the input string.
    Matches optional leading minus sign.
    """
    pattern = r'(?<![\d.])-?\b\d{1,3}\b(?!\.\d)'
    matches = re.findall(pattern, text)
    return [int(num) for num in matches]

text = "Values: 5, 23, -100, 999, 1000, 1234, -7, 345.89"
result = find_1_to_3_digit_integers(text)
print(result)

# 2) Make a function to validate an IPv4 address. IPv4 format is xxx.xxx.xxx.xxx and xxx shall be between 0 and 255.

def is_valid_ipv4(ip: str) -> bool:
    pattern = re.compile(
        r'^('
        r'(25[0-5]|'        # 250-255
        r'2[0-4][0-9]|'     # 200-249
        r'1[0-9]{2}|'       # 100-199
        r'[1-9]?[0-9])'     # 0-99
        r'\.){3}'
        r'(25[0-5]|'
        r'2[0-4][0-9]|'
        r'1[0-9]{2}|'
        r'[1-9]?[0-9])$'
    )
    return bool(pattern.match(ip))

print(is_valid_ipv4("192.168.1.1"))     # True
print(is_valid_ipv4("255.255.255.255")) # True
print(is_valid_ipv4("0.0.0.0"))         # True
print(is_valid_ipv4("256.1.1.1"))       # False
print(is_valid_ipv4("192.168.1"))       # False
print(is_valid_ipv4("192.168.01.1"))    # True (leading zeros allowed)

#3) Make a function to replace whitespaces in a string with underscores and vice versa.

def swap_whitespace_and_underscore(text: str) -> str:
    if re.search(r'\s', text):
        return re.sub(r'\s+', '_', text)
    else:
        return re.sub(r'_+', ' ', text)

print(swap_whitespace_and_underscore("Hello _ world\tPython\nRegex"))

# 4) Make a function to validate and convert a date of yyyy-mm-dd format to dd/mm/yyyy format.

def convert_date(date_str: str) -> str:
    # Validate format yyyy-mm-dd
    pattern = r'^(\d{4})-(\d{2})-(\d{2})$'
    match = re.match(pattern, date_str)
    
    if not match:
        raise ValueError("Invalid format. Expected yyyy-mm-dd")

    year, month, day = match.groups()

    # Validate actual calendar date
    try:
        datetime(int(year), int(month), int(day))
    except ValueError:
        raise ValueError("Invalid calendar date")

    # Convert format
    return f"{day}/{month}/{year}"

print(convert_date("2025-02-20")) # 20/02/2025
print(convert_date("2024-02-29"))  # Leap year: 29/02/2024

try:
    print(convert_date("2025-02-30")) # ValueError: Invalid calendar date
except Exception as e:
    print(e)

# 5) Make a function  to find all words starting with 'a' or 'e' in a given string.

def find_words_starting_with_a_or_e(text: str):
    return re.findall(r'\b[ae]\w*', text, re.IGNORECASE)

text = "string: An elephant and an eagle are amazing animals in Europe."
words = find_words_starting_with_a_or_e(text)
print(words)
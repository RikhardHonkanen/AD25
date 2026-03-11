#!/usr/bin/env python3

"""
Make a program to read a string from the input 
and print the index of uppercase letters in the string to the output.
"""

# Read a string from the input
user_input = input("Enter a string: ")

# Iterate through the characters in the string to print the index of uppercase letters
for index, char in enumerate(user_input):
    if char.isupper():
        print("Uppercase letter '{}' found at index {}".format(char,index))
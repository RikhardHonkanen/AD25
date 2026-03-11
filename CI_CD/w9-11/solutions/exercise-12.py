#!/usr/bin/env python3

"""
Make a program to read an integer greater than 0 from the input 
and calculate the factorial of the number and print it to the output.
"""
# Function to calculate factorial
def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n - 1)
    
# Function to print the result in a readable format
def print_expression(n):
    return " * ".join([str(i) for i in range(n, 0,-1)])

# Read an integer greater than 0 from the input
while True:
    try:
        num = int(input("Enter a positive number: "))
        if num <= 0:
            print("Please enter a number greater than 0.")
        else:
            break
    except ValueError:
        print("Oops!... An error ocurred!")

fact = factorial(num)

exp = print_expression(num)

print("The factorial of {} is: {} = {}".format(num, exp ,fact))
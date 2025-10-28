/**
 * @file 30.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief 1) Create two pointers, pa and pb to point to a and b; int a = 5; char b = ’s’;
 * 2) Change values of a and b using the pointers.
 * 3) Create a function that takes address of an int and adds 5 to the integer.
 * 4) Create a function pointer, funcptr, to the following function and call it.
 *  void say_value(int a) { printf("Value of a is %d\n", a); }
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

typedef void (*func_t)(int);

void add_five(int *x);

void add_five(int *x)
{
    *x += 5;
};
void say_value(int x)
{
    printf("Value of a is %d\n", x);
};
void print(func_t fptr, int x)
{
    (void)printf("Calling the func ...\n");
    fptr(x); // callback function is called
}

int main(void)
{
    int a = 5;
    char b = 's';
    int *a_ptr = &a;
    char *b_ptr = &b;
    *a_ptr = 10;
    *b_ptr = 't';
    add_five(a_ptr);
    func_t say_value_ptr = say_value;
    print(say_value_ptr, a);

    return 0;
};
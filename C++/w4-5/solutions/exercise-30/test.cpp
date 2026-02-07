#include "stack.h"
#include <cassert>
#include <iostream>

int main(void)
{
    try
    {
        Stack<int> stack{2};
    }
    catch (const std::exception &exp)
    {
        std::cout << exp.what() << ": passed!" << std::endl;
    }

    try
    {
        Stack<int> stack{4};
        assert(4 == stack.capacity());
        assert(0 == stack.available());

        std::cout << "Creating a stack: passed!" << std::endl;

        assert(stack.push(10));
        assert(1 == stack.available());
        assert(stack.push(20));
        assert(2 == stack.available());
        assert(stack.push(30));
        assert(3 == stack.available());
        assert(stack.push(40));
        assert(4 == stack.available());
        assert(!stack.push(50));
        std::cout << "Pushing to the stack: passed!" << std::endl;

        int value;

        assert(stack.pop(value));
        assert(3 == stack.available());
        assert(40 == value);

        assert(stack.pop(value));
        assert(2 == stack.available());
        assert(30 == value);

        assert(stack.pop(value));
        assert(1 == stack.available());
        assert(20 == value);

        assert(stack.pop(value));
        assert(0 == stack.available());
        assert(10 == value);

        assert(!stack.pop(value));

        std::cout << "Popping from the stack: passed!" << std::endl;

        assert(stack.push(10));
        assert(1 == stack.available());
        assert(stack.push(20));
        assert(2 == stack.available());
        stack.clear();
        assert(0 == stack.available());

        std::cout << "Clearing the stack: passed!" << std::endl;

        assert(stack.push(10));
        assert(1 == stack.available());
        assert(stack.push(20));
        assert(2 == stack.available());

        Stack<int> temp{std::move(stack)};

        assert(4 == temp.capacity());
        assert(2 == temp.available());
        assert(0 == stack.capacity());
        assert(0 == stack.available());

        stack = std::move(temp);

        assert(4 == stack.capacity());
        assert(2 == stack.available());
        assert(0 == temp.capacity());
        assert(0 == temp.available());

        std::cout << "Moving the stack: passed!" << std::endl;

        std::cout << "All tests passed!" << std::endl;
    }
    catch (const std::exception &exp)
    {
        std::cout << "Failed to run the test: " << exp.what() << std::endl;
    }

    return 0;
}
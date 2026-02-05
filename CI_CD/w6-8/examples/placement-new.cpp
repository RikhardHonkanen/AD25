#include <iostream>

template <typename T>
class A
{
public:
    template <typename U = T>
    struct B
    {
        template <typename V = U>
        void func(void);
    };

    A() { std::cout << "A()" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
};

template <typename T>
template <typename U>
template <typename V>
void A<T>::B<U>::func(void) {}

int main(void)
{
    A<int> *ptr{static_cast<A<int> *>(std::malloc(sizeof(A<int>)))};
    (void)new (ptr) A<int>{};

    ptr->~A();
    std::free(ptr);

    return 0;
}
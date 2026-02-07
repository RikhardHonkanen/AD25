#include <set>
#include <iostream>
#include <algorithm>
#include <unordered_set>

int main(void)
{
    std::set<int> set{10, 2, 8, 8, 5};
    std::cout << "Set Size: " << set.size() << std::endl;

    std::for_each(set.begin(), set.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    set.insert(100);
    std::for_each(set.begin(), set.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::cout << ((set.end() != set.find(100)) ? "Yes" : "No") << std::endl;
    std::cout << ((set.end() != set.find(1000)) ? "Yes" : "No") << std::endl;

    set.emplace(1000);
    std::for_each(set.begin(), set.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    set.erase(8);
    std::for_each(set.begin(), set.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::multiset<int> multiset{10, 2, 8, 8, 5};
    std::cout << "Multiset Size: " << multiset.size() << std::endl;

    std::cout << "Number of 8s: " << multiset.count(8) << std::endl;
    std::for_each(multiset.begin(), multiset.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    multiset.clear();
    std::cout << "Multiset Size: " << multiset.size() << std::endl;

    std::unordered_set<int> unordered_set{10, 2, 8, 8, 5};
    std::cout << "Unordered Set Size: " << unordered_set.size() << std::endl;

    std::for_each(unordered_set.begin(), unordered_set.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    std::unordered_multiset<int> unordered_multiset{10, 2, 8, 8, 5};
    std::cout << "Unordered Multiset Size: " << unordered_multiset.size() << std::endl;

    std::for_each(unordered_multiset.begin(), unordered_multiset.end(), [](const auto &elem)
                  { std::cout << elem << "\t"; });
    std::cout << std::endl;

    return 0;
}

#include <map>
#include <iostream>
#include <algorithm>
#include <unordered_map>

int main(void)
{
    std::map<std::string, uint32_t> map;
    map["Eva"] = 123456789;
    map["Lars"] = 523456289;
    map["Markus"] = 623856729;
    map["Linda"] = 993456789;
    map["Linda"] = 983456789;

    std::cout << "Map Size: " << map.size() << std::endl;

    std::for_each(map.begin(), map.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    map.insert({"Anders", 1212323232});
    map.emplace("Niklas", 454544545);
    std::for_each(map.begin(), map.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    map.erase("Eva");
    std::for_each(map.begin(), map.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    std::cout << "Map Size: " << map.size() << std::endl;
    map.clear();
    std::cout << "Map Size: " << map.size() << std::endl;

    std::multimap<std::string, uint32_t> multimap{
        {"Eva", 2232342343},
        {"Linda", 3243435343},
        {"Markus", 6234324343},
        {"Linda", 8932443241},
        {"Oliver", 5534327346}};
    std::cout << "Multimap Size: " << multimap.size() << std::endl;

    std::cout << std::endl;
    std::for_each(multimap.begin(), multimap.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    std::cout << ((multimap.end() != multimap.find("Eva")) ? "Yes" : "No") << std::endl;
    std::cout << ((multimap.end() != multimap.find("Lars")) ? "Yes" : "No") << std::endl;

    std::cout << "Number of Lind: " << multimap.count("Linda") << std::endl;

    multimap.clear();
    std::cout << "Multmap Size: " << multimap.size() << std::endl;

    std::unordered_map<uint32_t, std::string> unordered_map{
        {120, "AAA"},
        {12, "BBB"},
        {12, "CCC"},
        {100, "DDD"}};
    std::cout << "Unordered Map Size: " << unordered_map.size() << std::endl;

    std::cout << std::endl;
    std::for_each(unordered_map.begin(), unordered_map.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    std::unordered_multimap<uint32_t, std::string> unordered_multimap{
        {120, "AAA"},
        {12, "BBB"},
        {12, "CCC"},
        {10, "DDD"}};
    std::cout << "Unordered Multimap Size: " << unordered_multimap.size() << std::endl;

    std::cout << std::endl;
    std::for_each(unordered_multimap.begin(), unordered_multimap.end(), [](const auto &elem)
                  { std::cout << elem.first << ": " << elem.second << std::endl; });
    std::cout << std::endl;

    return 0;
}
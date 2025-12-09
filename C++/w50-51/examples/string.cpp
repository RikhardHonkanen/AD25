#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

int main(void)
{
    char cstr[100]{"....."};
    sprintf(cstr, "My name is %s and I am %d years old", "Eva", 30);
    strcat(cstr, "!\n");
    std::cout << cstr;

    std::string cppstr{"....."};
    cppstr = "My name is ";
    cppstr.append("Eva");
    cppstr += " and I am " + std::to_string(30) + " years old!\n";
    std::cout << cppstr;

    std::ostringstream oss;
    oss << "My name is "
        << "Eva"
        << " and I am " << 30 << " years old!\n";
    std::cout << oss.str();

    int year, month, day;

    std::istringstream iss("2023-01-17");
    std::getline(iss, cppstr, '-');
    std::istringstream(cppstr) >> year;
    std::getline(iss, cppstr, '-');
    std::istringstream(cppstr) >> month;
    std::getline(iss, cppstr, '-');
    std::istringstream(cppstr) >> day;
    std::cout << "Date: " << month << "/" << day << "/" << year << std::endl;

    year = month = day = 0;
    (void)sscanf("2023-01-17", "%04d-%02d-%02d", &year, &month, &day);
    std::cout << "Date: " << month << "/" << day << "/" << year << std::endl;

    return 0;
}

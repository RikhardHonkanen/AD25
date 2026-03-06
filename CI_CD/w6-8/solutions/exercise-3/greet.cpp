#include <vector>
#include "greet.h"

constexpr char const *HELLO_MY_FRIEND{"Hello, my friend."};
constexpr char const *HELLO_L{"Hello, "};
constexpr char const *HELLO_U{"HELLO "};

static bool isUpper(const std::string &string)
{
    bool upper{true};

    for (const auto &chr : string)
    {
        if (std::islower(chr))
        {
            upper = false;
            break;
        }
    }

    return upper;
}

static std::vector<std::string> extractNames(const std::string &string)
{
    std::vector<std::string> names;

    std::string name{""};
    for (const auto &chr : string)
    {
        if (std::isalpha(chr))
        {
            name += chr;
        }
        else if (chr == ',')
        {
            if (name != "")
            {
                names.push_back(name);
                name = "";
            }
        }
        else
        {
            ;
        }
    }

    if (name != "")
    {
        names.push_back(name);
    }

    return names;
}

std::string greet(const std::string &name)
{
    std::string greeting{""};
    std::vector<std::string> names{extractNames(name)};

    if (0 == names.size())
    {
        greeting = HELLO_MY_FRIEND;
    }
    else
    {
        size_t unum{0}, lnum{0};
        for (const auto &elem : names)
        {
            if (isUpper(elem))
            {
                unum++;
            }
            else
            {
                lnum++;
            }
        }

        size_t i{unum}, j{lnum};
        std::string lower{""}, upper{""};

        for (const auto &elem : names)
        {
            if (unum > 0)
            {
                if (isUpper(elem))
                {
                    if (unum == 1)
                    {
                        upper = elem + "!";
                    }
                    else
                    {
                        i--;
                        if (unum == 2)
                        {
                            if (i == 0)
                            {
                                upper += elem + "!";
                            }
                            else
                            {
                                upper += elem + " AND ";
                            }
                        }
                        else
                        {
                            if (i == 0)
                            {
                                upper += "AND " + elem + "!";
                            }
                            else
                            {
                                upper += elem + ", ";
                            }
                        }
                    }
                }
            }

            if (lnum > 0)
            {
                if (!isUpper(elem))
                {
                    if (lnum == 1)
                    {
                        lower = elem + ".";
                    }
                    else
                    {
                        j--;
                        if (lnum == 2)
                        {
                            if (j == 0)
                            {
                                lower += elem + ".";
                            }
                            else
                            {
                                lower += elem + " and ";
                            }
                        }
                        else
                        {
                            if (j == 0)
                            {
                                lower += "and " + elem + ".";
                            }
                            else
                            {
                                lower += elem + ", ";
                            }
                        }
                    }
                }
            }
        }

        if (lnum > 0)
        {
            greeting = HELLO_L + lower;
        }

        if (unum > 0)
        {
            if (lnum > 0)
            {
                greeting += " AND ";
            }

            greeting += HELLO_U + upper;
        }
    }

    return greeting;
}
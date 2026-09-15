#include <iostream>
#include <map>
#include <string>
#include <tuple>

#define SIGNALS                                                                                    \
    {                                                                                              \
        {{8, 0, 0, 240}, "speed"}, {{7, 8, -60, 60}, "temperature"},                               \
            {{7, 15, 0, 100}, "battery_level"}, {{1, 22, 0, 1}, "left_light"},                     \
            {{1, 23, 0, 1}, "right_light"},                                                        \
    }

struct Signal_values
{
    int length;
    int start;
    int range_min;
    int range_max;
};

int main()
{

    const std::initializer_list<std::pair<std::tuple<int, int, int, int>, const char *>>
        signal_defs = SIGNALS;
    std::map<std::string, Signal_values> signals;

    for (auto const &[info, name] : signal_defs)
    {
        auto [length, start, range_min, range_max] = info;
        signals[name] = {length, start, range_min, range_max};
    }

    for (auto const signal : signals)
    {
        std::cout << signal.first << std::endl;
        std::cout << signal.second.length << ", " << signal.second.start << ", "
                  << signal.second.range_min << ", " << signal.second.range_max << std::endl;
    }

    return 0;
};
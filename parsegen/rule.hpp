#pragma once

#include <string>
#include <vector>

struct Rule {
    std::string name;
    std::vector<std::vector<std::string>> alts;

    inline constexpr bool operator<(const Rule& rhs) const {
        return this->name < rhs.name;
    }
};


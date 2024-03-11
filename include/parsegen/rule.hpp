#pragma once

#include <string>
#include <vector>
#include <optional>

struct Alt { 
    std::vector<std::string> items;
    std::optional<std::string> action;
};


struct Rule {
    std::string name;
    std::vector<Alt> alts;
    std::string return_type = "std::optional<Node>";

    inline constexpr bool operator<(const Rule& rhs) const {
        return this->name < rhs.name;
    }
};



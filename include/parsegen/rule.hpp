#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct Item {
    std::string name;
    std::string type;
    std::string expect_value;
    int count{};

    [[nodiscard]] std::string var_name() const;
    [[nodiscard]] std::string eval_string() const;
    // friend auto operator<=>(const Item&, const Item&) = default;
    const bool operator==(const Item& rhs) const;
};

struct Alt {
    std::vector<std::string> items;
    std::optional<std::string> action;
};

struct Rule {
    std::string name;
    std::vector<Alt> alts;
    std::string return_type = "Node";

    inline constexpr bool operator<(const Rule& rhs) const { return this->name < rhs.name; }
};

using Rules = std::vector<Rule>;
using Alts = std::vector<Alt>;
using Items = std::vector<Item>;
using String = std::string;
using Strings = std::vector<std::string>;

std::ostream& operator<<(std::ostream& os, const Rule& r);
std::ostream& operator<<(std::ostream& os, const Alt& r);
std::ostream& operator<<(std::ostream& os, const Item& r);
std::ostream& operator<<(std::ostream& os, const std::vector<Rule>& rs);
std::ostream& operator<<(std::ostream& os, const std::vector<Alt>& as);
std::ostream& operator<<(std::ostream& os, const std::vector<Item>& as);

std::ostream& operator<<(std::ostream& os, const std::optional<Rule>& r);
std::ostream& operator<<(std::ostream& os, const std::optional<Alt>& r);
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Rule>>& rs);
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Alt>>& as);

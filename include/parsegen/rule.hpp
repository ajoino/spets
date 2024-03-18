#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

struct Item {
    std::string item;
    std::optional<std::string> name;
    std::string type;
    std::string expect_value;
    int count{};

    Item() = default;
    Item(std::string item) : item{std::move(item)} {};
    Item(std::string item, std::string name) : item{std::move(item)}, name{std::move(name)} {};
    Item(std::string item, std::string name, std::string type, std::string expect_value, int count) :
        item{std::move(item)}, name{name}, type{std::move(type)}, expect_value{std::move(expect_value)}, count{count} {};
    [[nodiscard]] std::string var_name() const;
    [[nodiscard]] std::string eval_string() const;
    // friend auto operator<=>(const Item&, const Item&) = default;
    bool operator==(const Item& rhs) const;
};

struct Alt {
    std::vector<Item> items;
    std::optional<std::string> action;

    Alt() = default;
    Alt(std::vector<Item> items) : items{std::move(items)} {};
    Alt(std::vector<Item> items, const std::string& action) : items{std::move(items)}, action{action} {};
};

struct Rule {
    std::string name;

    std::vector<Alt> alts;
    std::string return_type = "Node";

    inline constexpr bool operator<(const Rule& rhs) const { return this->name < rhs.name; }

    Rule() = default;
    Rule(std::string name, const std::vector<Alt>& alts, std::string return_type) :
        name{std::move(name)}, alts{alts}, return_type{std::move(return_type)} {};
    Rule(std::string name, const std::vector<Alt>& alts) : name{std::move(name)}, alts{alts} {};
};

struct Grammar {
    std::vector<Rule> rules;
    std::vector<std::string> metas;

    Grammar() = default;
    Grammar(std::vector<Rule> rules) : rules{std::move(rules)} {};
    Grammar(std::vector<Rule> rules, std::vector<std::string> metas) : rules{std::move(rules)}, metas{std::move(metas)} {};
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

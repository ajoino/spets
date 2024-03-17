#include <algorithm>
#include <format>
#include <parsegen/rule.hpp>
#include <ranges>

std::ostream& operator<<(std::ostream& os, const Rule& r) {
    return os << std::format("Rule({}, {})", r.name, r.return_type);
}

std::ostream& operator<<(std::ostream& os, const Alt& a) {
    std::string item_str{};
    for (const auto& item : a.items) {
        item_str.append(item.name);
    }
    return os << std::format("Alt({})", item_str);
}

std::ostream& operator<<(std::ostream& os, const Item& r) {
    return os << std::format("Item({}, {}, {}, {}, {}, {})", r.name, r.type, r.count, r.expect_value, r.eval_string(), r.var_name());
}

std::ostream& operator<<(std::ostream& os, const std::vector<Rule>& rs) { return os << "Rules"; }

std::ostream& operator<<(std::ostream& os, const std::vector<Alt>& as) { return os << "Alts"; }

std::ostream& operator<<(std::ostream& os, const std::vector<Item>& as) {
    os << "Items(";
    for (const auto& i : as) {
        os << i << ", ";
    }
    return os << ")";
}

std::ostream& operator<<(std::ostream& os, const std::optional<Rule>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
}

std::ostream& operator<<(std::ostream& os, const std::optional<Alt>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::ostream& operator<<(std::ostream& os, const std::optional<Item>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Rule>>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Alt>>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Item>>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::string Item::var_name() const {
    std::string count_str = count > 0 ? std::format("_{}", count) : "";
    // item is all uppercase
    if (!std::ranges::all_of(name, [](char ch) { return ch < 0x41 || ch > 0x5A; })) {
        std::string r{};
        std::transform(
            name.begin(), name.end(), r.begin(), [](unsigned char c) { return std::tolower(c); } // correct
        );
        return r.append(count_str);
    }

    auto name_ = name;
    return name_.append(count_str);
}

std::string Item::eval_string() const {
    // std::string eval_str = type == "Token" ? ".value().value" : ".value()";
    return var_name().append(".value()");
}

const bool Item::operator==(const Item& rhs) const {
    return this->name == rhs.name && this->count == rhs.count;
}

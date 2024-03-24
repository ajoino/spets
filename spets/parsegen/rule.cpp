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
        item_str.append(item.item);
    }
    return os << std::format("Alt({})", item_str);
}

std::ostream& operator<<(std::ostream& os, const NamedItem& r) {
    return os << std::format("Item({}, {}, {}, {}, {}, {}, {})", r.item, r.name ? r.name.value() : "nullopt", r.type, r.count, r.expect_value, r.eval_string(), r.var_name());
}

std::ostream& operator<<(std::ostream& os, const std::vector<Rule>& rs) { return os << "Rules"; }

std::ostream& operator<<(std::ostream& os, const std::vector<Alt>& as) { return os << "Alts"; }

std::ostream& operator<<(std::ostream& os, const std::vector<NamedItem>& as) {
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

std::ostream& operator<<(std::ostream& os, const std::optional<NamedItem>& r) {
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

std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<NamedItem>>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

std::string NamedItem::var_name() const {
    std::string count_str = count > 0 ? std::format("_{}", count) : "";

    if (name) {
        auto name_ = name.value();
        return name_.append(count_str);
    }
    // item is all uppercase
    if (!std::ranges::all_of(item, [](char ch) { return ch < 0x41 || ch > 0x5A; })) {
        std::string r{};
        std::transform(
            item.begin(), item.end(), r.begin(), [](unsigned char c) { return std::tolower(c); } // correct
        );
        return r.append(count_str);
    }

    auto name_ = item;
    return name_.append(count_str);
}

std::string NamedItem::eval_string() const {
    // std::string eval_str = type == "Token" ? ".value().value" : ".value()";
    return var_name().append(".value()");
}

bool NamedItem::operator==(const NamedItem& rhs) const {
    return this->var_name() == rhs.var_name() && this->count == rhs.count;
}

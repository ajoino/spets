#include <format>
#include <parsegen/rule.hpp>

std::ostream& operator<<(std::ostream& os, const Rule& r) {
    return os << std::format("Rule({}, {})", r.name, r.return_type);
}
std::ostream& operator<<(std::ostream& os, const Alt& a) {
    std::string item_str{};
    for (const auto& item : a.items) {
        item_str.append(item);
    }
    return os << std::format("Alt({})", item_str);
}
std::ostream& operator<<(std::ostream& os, const std::vector<Rule>& rs) {
    return os << "Rules";
}
std::ostream& operator<<(std::ostream& os, const std::vector<Alt>& as) {
    return os << "Alts";
}

std::ostream& operator<<(std::ostream& os, const std::optional<Rule>& r) {
    if (r) {
        return os << r;
    }
    return os << "nullopt";
} 
std::ostream& operator<<(std::ostream& os, const std::optional<Alt>& r){
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Rule>>& r){
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<Alt>>& r){
    if (r) {
        return os << r;
    }
    return os << "nullopt";
};

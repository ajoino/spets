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

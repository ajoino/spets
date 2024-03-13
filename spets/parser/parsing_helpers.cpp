#include <vector>
#include <parser/node.hpp>
#include <parser/parsing_helpers.hpp>
#include <iostream>

Node& prepend_child(Node& parent, Node& child) {

}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs)  {
    for (const auto& i : vs) {
        os << i;
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<std::string>>& vs)  {
    if (vs) {
    for (const auto& i : vs.value()) {
        os << i;
    }
    return os;
    }
    return os << "nullopt";
}
std::ostream& operator<<(std::ostream& os, const std::optional<std::string>& vs)  {
    if (vs) {
    return os << vs.value();
    }
    return os << "nullopt";
}

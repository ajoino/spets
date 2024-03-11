#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

struct Node;
inline std::ostream& operator<<(std::ostream& os, const Node& n);

struct Node {
    std::string node_type;
    std::string text;
    std::vector<Node> children;

    Node(std::string nt, std::string t, std::vector<Node> chdn) = delete;
    Node(std::string nt, std::string t) : node_type(std::move(nt)), text(std::move(t)) {};

    Node(std::string nt, const std::vector<Node>& chdn) : node_type{std::move(nt)} {
        for (const auto& child : chdn) {
            add_child(child);
        }
    }
    Node(std::string nt, const std::vector<Node>& chdn, const Node& extra_chld) : node_type{std::move(nt)} {
        for (const auto& child : chdn) {
            add_child(child);
        }
        add_child(extra_chld);
    }

    Node& add_child(const Node& child) {
        children.push_back(child);
        text += child.text;
        return *this;
    }

    Node& prepend_child(const Node& child) {
        children.insert(children.begin(), child);
        return *this;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Node& n) {
    auto& ret = os << n.node_type << "_Node("
                   << "\"" << n.text << "\"";
    if (!n.children.empty()) {
        for (const auto& child : n.children) {
            ret << ", " << child;
        }
    }
    return ret << ")";
}

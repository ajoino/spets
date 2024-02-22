#ifndef __SPETS_NODE_H__
#define __SPETS_NODE_H__
#include <string>
#include <vector>
#include <ostream>

#include "lexer.hpp"

struct Node {
  std::string node_type;
  std::string text;
  std::vector<Node> children;
  
  Node(std::string nt, std::string t, std::vector<Node> chdn) = delete;
  Node(std::string nt, std::string t) : node_type(nt), text(t), children{} {};
  Node(std::string nt, std::vector<Node> chdn) : node_type{nt}, text{}, children{} {
    for (const auto& child : children) {
        add_child(child);
    }
  }


  Node &add_child(Node child) {
    children.push_back(child);
    text += child.text;
    return *this;
  }
};

inline std::ostream &operator<<(std::ostream &os, const Node &n) {
  auto &ret = os << n.node_type << "_Node("
                 << "\"" << n.text << "\"" ;
  if (!n.children.empty()) {
    for (const auto &child : n.children) {
      ret << ", " << child;
    }
  }
  return ret << ")";
}

// struct Node {
//   std::string node_type;
//   std::vector<Node> children;
//
//   Node &add_child(Node child) {
//     children.push_back(child);
//     return *this;
//   }
// };
//
// inline std::ostream &operator<<(std::ostream &os, const Node &n) {
//   auto &ret = os << n.node_type << "_Node(";
//   if (!n.children.empty()) {
//     for (const auto &child : n.children) {
//       ret << ", " << child;
//     }
//   }
//   return ret << ")";
// }
#endif // __SPETS_NODE_H__

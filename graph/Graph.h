//
// Created by rbroggi on 10/17/20.
//

#pragma once

#include <cstdint>
#include <ostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class Node;
template <typename T>
struct NodeHash {
  size_t operator()(const Node<T> *iNode) const;
};

template <typename T>
struct NodeEquality {
  bool operator()(Node<T> *const &lhs, Node<T> *const &rhs) const;
};


template <typename T>
class Node {

  using Edges = std::unordered_set<Node<T> *, NodeHash<T>, NodeEquality<T>>;

public:

  Node() = default;
  Node(uint32_t iD, T iValue, Node<T> *iNeighbors)
      : id(iD), value(iValue), neighbors(iNeighbors) {}
  Node(uint32_t iD, Edges iNeighbors)
      : id(iD), neighbors(std::move(iNeighbors)) {}
  explicit Node(uint32_t id) : Node(id, {}){};

  uint32_t getId() const { return id; }
  const T& getValue() const { return value; }
  void setValue(T iValue) { value = iValue; }

  bool addNode(Node<T> *iNodePtr) {
    if (iNodePtr) {
      neighbors.insert(iNodePtr);
    }
  }

  const Edges &getNeighbors() const { return neighbors; }

  friend std::ostream &operator<<(std::ostream &iOs, const Node &iNode) {
    iOs << "id: " << iNode.id << " [";
    for (auto* node : iNode.neighbors) {
      if (node) {
        iOs << node->getId() << ", ";
      }
    }
    iOs <<  "]";
    return iOs;
  }
  //  friend std::ostream &operator<<(std::ostream &iOs, const Node &iNode) {
//    iOs << "id: " << iNode.id << " [";
//    for (auto* node : iNode.neighbors) {
//      iOs << "  " << iNode.neighbors;
//    }
//    iOs <<  "]";
//    return iOs;
//  }

private:
  uint32_t id;
  T value;
  Edges neighbors;
};

template <typename T>
size_t NodeHash<T>::operator()(const Node<T> *iNode) const {
  if (not iNode)
    return 0;
  return iNode->getId();
}

template <typename T>
bool NodeEquality<T>::operator()(Node<T> *const &lhs,
                                 Node<T> *const &rhs) const {
  if ((not lhs) and (not rhs))
    return true;
  if (not lhs)
    return false;
  if (not rhs)
    return false;
  return (lhs->getId() == rhs->getId());
}

template <typename T>
using Graph = std::unordered_map<uint32_t, Node<T>>;

Graph<std::string>
buildGraph(const std::vector<uint32_t> &iNodes,
           const std::vector<std::pair<uint32_t, uint32_t>> &iVertices) {
  std::unordered_map<uint32_t, Node<std::string>> aGraph;
  for (auto el : iNodes) {
    aGraph.insert({el, Node<std::string>(el)});
  }

  for (auto el : iVertices) {
    auto sourceNode = aGraph.find(el.first);
    auto targetNode = aGraph.find(el.second);
    if ((sourceNode != aGraph.end()) && (targetNode != aGraph.end())) {
      sourceNode->second.addNode(&(targetNode->second));
    }
  }
  return aGraph;
}

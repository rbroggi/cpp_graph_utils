//
// Created by rbroggi on 10/17/20.
//

#pragma once

#include "../graph/Graph.h"
#include "../visit/DFS.h"
#include <iostream>

namespace scc {
template <typename T>
std::unordered_map<uint32_t, uint32_t> nodeIdToSCCId(const Graph<T>& iGraph) {
  std::unordered_set<uint32_t> visitedSet;
  std::stack<uint32_t> stack;
  auto visitFunc = [&stack](const Node<T>& iNode) {stack.push(iNode.getId());};
  auto aVisitFilter = [&visitedSet] (const Node<T>& iNode) {
    auto isAlreadyVisited = dfs::isVisited(visitedSet, iNode.getId());
    if (not isAlreadyVisited) {
      visitedSet.insert(iNode.getId());
      return true;
    }
    return false;
  };
  Graph<T> reversedGraph;
  for (const auto &[id, node] : iGraph) {
    reversedGraph.insert({id, Node<T>(id)});
    if (not dfs::isVisited(visitedSet, id)) {
      // 1. DFS with stack ordering
      dfs::PostVisit<T>(node, visitFunc, aVisitFilter);
    }
  }

  // 2. Graph reverse
  for (const auto &[id, node] : iGraph) {
    auto& aNodeReversed = reversedGraph[id];
    for (const Node<T>* neighbor : node.getNeighbors()) {
      if (neighbor) {
        Node<T>& aNeighborReversed = reversedGraph[neighbor->getId()];
        aNeighborReversed.addNode(&aNodeReversed);
      }
    }
  }

  // 3. DFS Navigate reversed graph in stack order
  std::unordered_map<uint32_t, uint32_t> nodeIDToSCCID;
  visitedSet.clear();
  uint32_t sccId = 0U;
  while (not stack.empty()) {
    auto id = stack.top();
    Node<T>& node = reversedGraph[id];
    stack.pop();
    if (not dfs::isVisited(visitedSet, id)) {
      ++sccId;
      auto visitFunc2 = [&nodeIDToSCCID, &visitedSet,
                         &sccId](const auto &iNode) {
        nodeIDToSCCID.insert({iNode.getId(), sccId});
        visitedSet.insert(iNode.getId());
      };
      dfs::PreVisit<T>(node, visitFunc2, aVisitFilter);
    }
  }
  return nodeIDToSCCID;
}

}

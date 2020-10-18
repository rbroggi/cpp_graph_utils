
//
// Created by rbroggi on 10/17/20.
//

#pragma once
#include "../graph/Graph.h"
#include <functional>
#include <queue>
#include <unordered_set>

namespace bfs {

bool isVisited(const std::unordered_set<uint32_t> &visitedSet,
               uint32_t iNodeId) {
  return visitedSet.find(iNodeId) != visitedSet.end();
}

// Node visited before then children
template <typename T>
void Visit(Node<T> &iGraphNode,
              std::function<void(Node<T> &)> iVisitFunction) {
  std::queue<Node<T> *> aQueue;
  aQueue.push(&iGraphNode);
  std::unordered_set<uint32_t> visited;
  visited.insert(iGraphNode.getId());

  while (not aQueue.empty()) {
    Node<T> *aNode = aQueue.front();
    aQueue.pop();
    if (aNode) {
      iVisitFunction(*aNode);
      for (Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId()))) {
          aQueue.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }
}

}

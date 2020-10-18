//
// Created by rbroggi on 10/17/20.
//

#pragma once
#include "../graph/Graph.h"
#include <functional>
#include <stack>
#include <unordered_set>

namespace dfs {

bool isVisited(const std::unordered_set<uint32_t> &visitedSet,
               uint32_t iNodeId) {
  return visitedSet.find(iNodeId) != visitedSet.end();
}

// Node visited before then children
template <typename T>
void PreVisit(Node<T> &iGraphNode,
              std::function<void(Node<T> &)> iVisitFunction,
              std::function<bool(Node<T> &)> shouldVisit = [](auto &) {return true;}) {
  std::stack<Node<T> *> aStack;
  std::unordered_set<uint32_t> visited;
  if (shouldVisit(iGraphNode)) {
    aStack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not aStack.empty()) {
    Node<T> *aNode = aStack.top();
    aStack.pop();
    if (aNode) {
      iVisitFunction(*aNode);
      for (Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          aStack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }
}

// Node visited before then children
template <typename T>
void PreVisit(const Node<T> &iGraphNode,
              std::function<void(const Node<T> &)> iVisitFunction,
              std::function<bool(const Node<T> &)> shouldVisit = [](auto &) {return true;}) {
  std::stack<const Node<T> *> aStack;
  std::unordered_set<uint32_t> visited;
  if (shouldVisit(iGraphNode)) {
    aStack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not aStack.empty()) {
    const Node<T> *aNode = aStack.top();
    aStack.pop();
    if (aNode) {
      iVisitFunction(*aNode);
      for (const Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          aStack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }
}

// Node visited after then children
template <typename T>
void PostVisit(Node<T> &iGraphNode,
               std::function<void(Node<T> &)> iVisitFunction,
               std::function<bool(Node<T> &)> shouldVisit = [](auto &) {return true;}) {
  std::stack<Node<T> *> aStack;
  std::unordered_set<uint32_t> visited;
  std::vector<Node<T> *> aVisitOrder;
  if (shouldVisit(iGraphNode)) {
    aStack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not aStack.empty()) {
    Node<T> *aNode = aStack.top();
    aStack.pop();
    if (aNode) {
      aVisitOrder.push_back(aNode);
      for (Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          aStack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }

  for (auto it = aVisitOrder.rbegin(); it != aVisitOrder.rend(); ++it) {
    iVisitFunction(*(*it));
  }
}

// Node visited after then children
template <typename T>
void PostVisit(const Node<T> &iGraphNode,
               std::function<void(const Node<T> &)> iVisitFunction,
               std::function<bool(const Node<T> &)> shouldVisit = [](auto &) {return true;}) {
  std::stack<const Node<T> *> aStack;
  std::unordered_set<uint32_t> visited;
  std::vector<const Node<T> *> aVisitOrder;
  if (shouldVisit(iGraphNode)) {
    aStack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not aStack.empty()) {
    const Node<T>* aNode = aStack.top();
    aStack.pop();
    if (aNode) {
      aVisitOrder.push_back(aNode);
      for (const Node<T>* neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          aStack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }

  for (auto it = aVisitOrder.rbegin(); it != aVisitOrder.rend(); ++it) {
    iVisitFunction(*(*it));
  }
}

// Node visited after then children
template <typename T>
void PrePostVisit(
    Node<T> &iGraphNode,
    std::function<void(Node<T> &)> visitPre,
    std::function<void(Node<T> &)> visitPost,
    std::function<bool(Node<T> &)> shouldVisit = [](auto &) { return true; }) {
  std::stack<Node<T> *> aStack;
  std::unordered_set<uint32_t> visited;
  std::vector<Node<T> *> postVisitOrder;
  if (shouldVisit(iGraphNode)) {
    aStack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not aStack.empty()) {
    Node<T> *aNode = aStack.top();
    aStack.pop();
    if (aNode) {
      visitPre(*aNode);
      postVisitOrder.push_back(aNode);
      for (Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          aStack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }

  for (auto it = postVisitOrder.rbegin(); it != postVisitOrder.rend(); ++it) {
    visitPost(*(*it));
  }
}

// Node visited after then children
template <typename T>
void PrePostVisit(
    const Node<T> &iGraphNode,
    std::function<void(const Node<T> &)> visitPre,
    std::function<void(const Node<T> &)> visitPost,
    std::function<bool(const Node<T> &)> shouldVisit = [](auto &) {
      return true;
    }) {
  std::stack<const Node<T> *> stack;
  std::unordered_set<uint32_t> visited;
  std::vector<const Node<T> *> aVisitOrder;
  if (shouldVisit(iGraphNode)) {
    stack.push(&iGraphNode);
    visited.insert(iGraphNode.getId());
  }

  while (not stack.empty()) {
    const Node<T> *aNode = stack.top();
    stack.pop();
    if (aNode) {
      visitPre(*aNode);
      aVisitOrder.push_back(aNode);
      for (const Node<T> *neighbor : aNode->getNeighbors()) {
        if (neighbor and (not isVisited(visited, neighbor->getId())) and
            shouldVisit(*neighbor)) {
          stack.push(neighbor);
          visited.insert(neighbor->getId());
        }
      }
    }
  }

  for (auto it = aVisitOrder.rbegin(); it != aVisitOrder.rend(); ++it) {
    visitPost(*(*it));
  }
}

}

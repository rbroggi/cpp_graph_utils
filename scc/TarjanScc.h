//
// Created by rbroggi on 10/18/20.
//

#pragma once

#include "../graph/Graph.h"
#include "../visit/DFS.h"
#include <unordered_map>
namespace scc {

template <typename T>
bool contains(const std::unordered_map<uint32_t, T> iMap, uint32_t iKey) {
  return iMap.find(iKey) != iMap.end();
}

uint32_t generateUniqueId() {
  static uint32_t uid = 0U;
  return ++uid;
}

template <typename T>
std::unordered_map<uint32_t, uint32_t> nodeIdToSSCTarjan(const Graph<T>& iGraph) {

  std::unordered_set<uint32_t> alreadyVisited;
  std::unordered_set<uint32_t> currentVisit;
  std::unordered_set<uint32_t> stackMembers;
  std::unordered_map<uint32_t, uint32_t> nodeIdToGeneratedId;
  std::unordered_map<uint32_t, uint32_t> nodeIdToLvv;

  std::function<void(const Node<T> &)> visitPre = [&stackMembers, &nodeIdToGeneratedId, &nodeIdToLvv] (const Node<T>& node) {
    uint32_t uId = generateUniqueId();
    stackMembers.insert(node.getId());
    nodeIdToGeneratedId.insert({node.getId(), uId});
    nodeIdToLvv.insert({node.getId(), uId});

    for (const Node<T>* neighbor : node.getNeighbors()) {
      if (dfs::isVisited(stackMembers, neighbor->getId())) {
        uint32_t currentLlv = nodeIdToLvv[node.getId()];
        uint32_t neighborLlv = nodeIdToLvv[neighbor->getId()];
        nodeIdToLvv.insert_or_assign(node.getId(), std::min(currentLlv, neighborLlv));
      }
    }
  };

  std::function<void(const Node<T> &)> visitPost = [&currentVisit, &stackMembers, &nodeIdToGeneratedId, &nodeIdToLvv] (const Node<T>& node) {
    uint32_t aMinLlv = nodeIdToLvv[node.getId()];
    for (const Node<T>* neighbor : node.getNeighbors())  {
      if (contains(nodeIdToLvv, neighbor->getId()) and dfs::isVisited(currentVisit, neighbor->getId())) {
        aMinLlv = std::min(aMinLlv, nodeIdToLvv[neighbor->getId()]);
      }
    }
    nodeIdToLvv.insert_or_assign(node.getId(), aMinLlv);
    stackMembers.erase(node.getId());
  };

  std::function<bool(const Node<T> &)> shouldVisit = [&currentVisit, &alreadyVisited] (const Node<T>& node) {
    if (not dfs::isVisited(alreadyVisited, node.getId())) {
      alreadyVisited.insert(node.getId());
      currentVisit.insert(node.getId());
      return true;
    }
    return false;
  };

  for (const auto &[nodeID, node] : iGraph) {
    dfs::PrePostVisit(node, visitPre, visitPost, shouldVisit);
    currentVisit.clear();
  }
  return nodeIdToLvv;
}

}

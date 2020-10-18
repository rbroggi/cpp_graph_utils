

#include "graph/Graph.h"
#include "scc/KosarajuScc.h"
#include "scc/TarjanScc.h"
#include "visit/BFS.h"
#include "visit/DFS.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {

  // build childless nodes
  std::vector<uint32_t> aListNodes = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U};

  // build children from list connections
  std::vector<std::pair<uint32_t, uint32_t>> aConnections = {
      {1U, 2U}, {2U, 3U}, {3U, 4U}, {4U, 5U}, {5U, 2U},  {5U, 6U},  {3U, 7U},
      {4U, 9U}, {7U, 8U}, {8U, 9U}, {9U, 7U}, {9U, 10U}, {10U, 9U},
  };

  Graph<std::string> aGraph = buildGraph(aListNodes, aConnections);
//  std::for_each(aGraph.cbegin(), aGraph.cend(), [](const auto &idNode) {
//    std::cout << idNode.second << std::endl;
//  });

  auto printNodeIdFunc = [](const Node<std::string> &iNode) {
    std::cout << iNode.getId() << std::endl;
  };
  std::cout << "DFS Pre: " << std::endl;
  Node<std::string> &aStartingNode = aGraph[1U];
  dfs::PreVisit<std::string>(aStartingNode, printNodeIdFunc);
  std::cout << "DFS Post: " << std::endl;
  dfs::PostVisit<std::string>(aStartingNode, printNodeIdFunc);
  std::cout << "BFS : " << std::endl;
  bfs::Visit<std::string>(aStartingNode, printNodeIdFunc);


  std::cout << "Kosaraju SCC: " << std::endl;
  auto nodeIdToSCCMap = scc::nodeIdToSCCId<std::string>(aGraph);
  std::for_each(nodeIdToSCCMap.begin(), nodeIdToSCCMap.end(),
                [](const auto &pair) {
                  std::cout << "id: " << pair.first << " sccId: " << pair.second
                            << std::endl;
                });

  std::cout << "Tarjan SCC: " << std::endl;
  auto nodeIdToSCCMapT = scc::nodeIdToSSCTarjan<std::string>(aGraph);
  std::for_each(nodeIdToSCCMapT.begin(), nodeIdToSCCMapT.end(),
                [](const auto &pair) {
                  std::cout << "id: " << pair.first << " sccId: " << pair.second
                            << std::endl;
                });

}

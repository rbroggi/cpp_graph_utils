cmake_minimum_required(VERSION 3.13)
project(cpp_graph_utils)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_graph_utils main.cpp scc/KosarajuScc.h graph/Graph.h visit/DFS.h visit/BFS.h scc/TarjanScc.h)

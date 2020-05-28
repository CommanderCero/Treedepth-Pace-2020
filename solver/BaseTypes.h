#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>

using namespace boost;

struct Vertex{int num;};
typedef adjacency_list<vecS, vecS, undirectedS, Vertex> UndirectedGraph;
typedef graph_traits<UndirectedGraph>::edge_iterator ud_edge_iterator;
typedef graph_traits<UndirectedGraph>::vertex_iterator ud_vertex_iterator;
typedef std::pair<int, int> Edge;
typedef property_map<UndirectedGraph, vertex_index_t>::type ud_IndexMap;
typedef graph_traits<UndirectedGraph>::vertex_descriptor VIndex;
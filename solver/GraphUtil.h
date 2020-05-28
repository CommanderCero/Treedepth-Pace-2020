#pragma once

#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>
#include <vector>

#include "BaseTypes.h"
#include "Tree.h"
#include "betweenness_centrality.hpp"

typedef shared_ptr<std::vector<unsigned long>> vertex_component_map;
typedef filtered_graph<UndirectedGraph, keep_all, function<bool(UndirectedGraph::vertex_descriptor)> > ComponentGraph;

std::vector<ComponentGraph> ConnectedComponents(const UndirectedGraph& graph);
VIndex ChooseNodeBrandes(const UndirectedGraph& graph, int pivotCount = 30);
int GenerateNaiveSolution(const UndirectedGraph& subgraph, int parentNum, Tree& targetTree, std::vector<int>& depthLookup);
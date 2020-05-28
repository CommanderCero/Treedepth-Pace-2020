#pragma once

#include <boost/graph/filtered_graph.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/make_shared.hpp>
#include <boost/graph/copy.hpp>

#include "BaseTypes.h"
#include "Tree.h"

std::vector<int> solve_naively(UndirectedGraph& graph);

class StrategicSolver
{
public:
    StrategicSolver(int betweenness_cutoff, int brandes_pivots);
    void Decompose(UndirectedGraph graph, Tree& bucket);
private:
    int betweenness_cutoff;
    int brandes_pivots;
    int decomp(UndirectedGraph* graph, Tree& bucket, Vertex parent);
    VIndex choose_node(UndirectedGraph& graph);
};
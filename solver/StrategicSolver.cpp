#include <signal.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <valarray>
#include <limits>
#include <cstdio>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/make_shared.hpp>
#include <boost/graph/copy.hpp>

#include "betweenness_centrality.hpp"
#include "BaseTypes.h"
#include "Tree.h"
#include "StrategicSolver.h"
#include "GraphUtil.h"

using namespace boost;


StrategicSolver::StrategicSolver(int betweenness_cutoff, int brandes_pivots)
    : betweenness_cutoff(betweenness_cutoff), 
      brandes_pivots(brandes_pivots)
{
}

std::vector<int> solve_naively(UndirectedGraph& graph){
    std::vector<int> output;

    int nNodes = num_vertices(graph); // nNodes is same as depth
    output.push_back(nNodes);

    // get an index map
    ud_IndexMap index = get(vertex_index, graph);

    // look up each vertex by index
    std::pair<ud_vertex_iterator, ud_vertex_iterator> vi = vertices(graph);
    for(ud_vertex_iterator vertex_iter = vi.first; vertex_iter != vi.second; ++vertex_iter){
        output.push_back(index[*vertex_iter]);
    }
    return output;
}

void StrategicSolver::Decompose(UndirectedGraph graph, Tree& bucket)
{
    int nNodes = num_vertices(graph);
    VIndex root_id = choose_node(graph);
    Vertex root = graph[root_id];
    clear_vertex(root_id, graph);
    remove_vertex(root_id, graph);
    bucket.ParentLookup[root.num] = -1;

    std::vector<ComponentGraph> cc = ConnectedComponents(graph);
    int depth = std::numeric_limits<int>::min();
    if (cc.size() > 1){
        for(ComponentGraph& c: cc){
            UndirectedGraph* new_subgraph = new UndirectedGraph();
            copy_graph(c, *new_subgraph);
            depth = std::max(decomp(new_subgraph, bucket, root), depth);
        }
    }
    else
    {
        UndirectedGraph* new_subgraph = new UndirectedGraph();
        copy_graph(graph, *new_subgraph);
        depth = decomp(new_subgraph, bucket, root);
    }
    bucket.Depth = depth + 1;
}

int StrategicSolver::decomp(UndirectedGraph* graph, Tree& bucket, Vertex parent)
{
    std::vector<ComponentGraph> cc;
    int depth = 0;
    do
    {
        VIndex child_id = choose_node(*graph);
        Vertex child = (*graph)[child_id];
        bucket.ParentLookup[child.num] = parent.num;

        clear_vertex(child_id, *graph);
        remove_vertex(child_id, *graph);
        depth++;

        if(num_vertices(*graph) == 0)
            return depth;

        cc = ConnectedComponents(*graph);
        parent = child;
    } while(cc.size() == 1);

    std::vector<UndirectedGraph*> subgraphs;
    for(ComponentGraph& c: cc)
    {
        UndirectedGraph* new_subgraph = new UndirectedGraph();
        copy_graph(c, *new_subgraph);
        subgraphs.push_back(new_subgraph);
    }
    delete graph;
    
    int largestDepth = std::numeric_limits<int>::min();
    for(UndirectedGraph* subgraph : subgraphs)
    {
        int branchDepth = decomp(subgraph, bucket, parent);
        largestDepth = std::max(branchDepth, largestDepth);
    }

    return largestDepth + depth;
}

VIndex StrategicSolver::choose_node(UndirectedGraph& graph)
{
    int child_id;
    if(true)
    {
        child_id = ChooseNodeBrandes(graph, brandes_pivots);
    }
    else
    {
        // For large graphs use degree centrality
        int maxDegree = std::numeric_limits<int>::min();
        for(auto v : make_iterator_range(vertices(graph)))
        {
            int deg = degree(v, graph);
            if(deg > maxDegree)
            {
                maxDegree = deg;
                child_id = v;
            }
        }
    }

    auto VIter = vertices(graph);
    return *(VIter.first + child_id);
}


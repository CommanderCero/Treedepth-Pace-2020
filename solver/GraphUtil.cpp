#include <random>
#include <iostream>

#include "GraphUtil.h"

std::vector<ComponentGraph> ConnectedComponents(const UndirectedGraph& graph)
{
    vertex_component_map mapping = boost::make_shared<std::vector<unsigned long>>(num_vertices(graph));
    size_t num = connected_components(graph, mapping->data());

    std::vector<ComponentGraph> component_graphs;

    for (size_t i = 0; i < num; i++)
        component_graphs.emplace_back(graph, keep_all{},
            [mapping,i](UndirectedGraph::vertex_descriptor v) {
                return mapping->at(v)==i;
            });

    return component_graphs;
}

std::mt19937 rng(0); // Set a static seed
VIndex ChooseNodeBrandes(const UndirectedGraph& graph, int pivotCount)
{
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, num_vertices(graph) - 1);
    std::vector<double> vertex_centrality(num_vertices(graph));
    std::vector<size_t> pivots;
    int s;
    for(int i=0; i < std::min((int)num_vertices(graph), pivotCount); ++i)
    {
        auto val = dist(rng);
        pivots.push_back(val);
    }

    brandes_betweenness_centrality(
        graph, pivots,
        make_iterator_property_map(
            vertex_centrality.begin(),
            get(vertex_index, graph),
            double()
        )
    );

    auto max_elem_iter = std::max_element(vertex_centrality.begin(), vertex_centrality.end());
    auto child_id = std::distance(vertex_centrality.begin(), max_elem_iter);
    return vertex(child_id, graph);
}

int GenerateNaiveSolution(const UndirectedGraph& subgraph, int parentNum, Tree& targetTree, std::vector<int>& depthLookup)
{
    int counter = 0;
    int depth = 0;
    for(VIndex v : make_iterator_range(vertices(subgraph)))
    {
        int childNum = subgraph[v].num;
        targetTree.ParentLookup[childNum] = parentNum;
        parentNum = childNum;

        counter++;
        depth = std::max(depth, counter + depthLookup[childNum]);
    }

    return depth;
}
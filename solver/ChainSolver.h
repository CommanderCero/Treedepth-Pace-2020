#pragma once

#include "BaseTypes.h"
#include "Tree.h"
#include "GraphUtil.h"

inline std::pair<std::vector<VIndex>, VIndex> CollectChain(VIndex v, const UndirectedGraph& graph)
{
    VIndex prev = v;
    VIndex curr = *adjacent_vertices(v, graph).first;
    std::vector<VIndex> chainMembers;
    chainMembers.push_back(v);
    while(degree(curr, graph) == 2)
    {
        auto neighbors = adjacent_vertices(curr, graph);
        auto nextMember = *neighbors.first;
        if(nextMember == prev)
        {
            nextMember = *(neighbors.first + 1);
        }

        chainMembers.push_back(curr);
        prev = curr;
        curr = nextMember;
    }

    return make_pair(chainMembers, curr);
}

inline void SolveChain(int parentNum, std::vector<VIndex>& chainMembers, const UndirectedGraph& graph, Tree& tree, int left, int right)
{
    if(left > right)
        return;

    // Take middle vertex
    int mid = left + (right - left) / 2;
    VIndex child = chainMembers[mid];
    int childNum = graph[child].num;
    tree.ParentLookup[childNum] = parentNum;

    // Solve remaining chain elements
    SolveChain(childNum, chainMembers, graph, tree, left, mid - 1);
    SolveChain(childNum, chainMembers, graph, tree, mid + 1, right);
}

template<typename VertexIter>
UndirectedGraph* RemoveChains(VertexIter start, VertexIter end, const UndirectedGraph& targetGraph, Tree& tree, std::vector<int>& depthLookup)
{
    std::vector<bool> removed(num_vertices(targetGraph));
    int count = 0;
    for(auto it = start; it != end; it = next(it))
    {
        VIndex v = *it;
        if(degree(v, targetGraph) != 1)
            continue;

        VIndex chainParent;
        std::vector<VIndex> chainMembers;
        std::tie(chainMembers, chainParent) = CollectChain(v, targetGraph);
        SolveChain(targetGraph[chainParent].num, chainMembers, targetGraph, tree, 0, chainMembers.size() - 1);
        for(auto v : chainMembers)
            removed[v] = true;

        depthLookup[targetGraph[chainParent].num] = std::max((int)log2(chainMembers.size()) + 1, depthLookup[targetGraph[chainParent].num]);
        count += chainMembers.size();
    }

    ComponentGraph c{targetGraph, keep_all{}, [&](VIndex v){return !removed[v];}};
    UndirectedGraph* new_subgraph = new UndirectedGraph();
    copy_graph(c, *new_subgraph);

    return new_subgraph;
}
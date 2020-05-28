#include <boost/graph/copy.hpp>
#include <iostream>

#include "PrioritizedStrategicSolver.h"
#include "GraphUtil.h"
#include "ChainSolver.h"


PrioritizedStrategicSolver::PrioritizedStrategicSolver()
    : queue()
{
}

void PrioritizedStrategicSolver::FinishPartialSolution(Tree& bucket, std::vector<int>& depthLookup)
{
    for(Node& n : queue)
    {
        int naiveDepth = GenerateNaiveSolution(*n.Subgraph, n.ParentNum, bucket, depthLookup);
        bucket.Depth = std::max(bucket.Depth, n.Depth + naiveDepth);
    }
}

void PrioritizedStrategicSolver::Decompose(const UndirectedGraph& graph, Tree& bucket)
{
    Init(new UndirectedGraph(graph));
    std::vector<int> depthLookup(num_vertices(graph));
    while(queue.size() != 0)
    {
        Step(bucket, depthLookup);
    }
    return;
}

void PrioritizedStrategicSolver::Init(UndirectedGraph* graph)
{
    for(int i = 0; i < queue.size(); i++)
    {
        delete queue[i].Subgraph;
    }

    queue.clear();
    queue.emplace_back(graph, 0, -1);
}

void PrioritizedStrategicSolver::Step(Tree& bucket, std::vector<int>& depthLookup)
{
    Node next = queue.front();
    std::pop_heap(queue.begin(), queue.end());
    queue.pop_back();

    // Prune branches where the naive solution cant make the overall solution worse
    int numVertices = num_vertices(*next.Subgraph);
    int naiveDepth = numVertices + next.Depth;
    if(numVertices == 1 || naiveDepth <= bucket.Depth)
    {
        naiveDepth = GenerateNaiveSolution(*next.Subgraph, next.ParentNum, bucket, depthLookup);
        bucket.Depth = std::max(bucket.Depth, next.Depth + naiveDepth);
        delete next.Subgraph;
        return;
    }

    // Remove a vertex
    VIndex child = ChooseNodeBrandes(*next.Subgraph, 30);
    int childNum = (*next.Subgraph)[child].num;
    clear_vertex(child, *next.Subgraph);
    remove_vertex(child, *next.Subgraph);
    next.Depth += 1;
    bucket.Depth = std::max(bucket.Depth, next.Depth + depthLookup[childNum]);

    // Update tree
    bucket.ParentLookup[childNum] = next.ParentNum;
    bucket.Depth = std::max(bucket.Depth, next.Depth + depthLookup[childNum]);

    // Search for connected components
    auto components = ConnectedComponents(*next.Subgraph);
    if(components.size() == 1)
    {
        // We didnt split the graph, keep the node
        next.ParentNum = childNum;
        queue.push_back(next);
        std::push_heap(queue.begin(), queue.end());
    }
    else
    {
        // We've split the graph, add all components to the queue
        for(ComponentGraph& c: components)
        {
            UndirectedGraph* new_subgraph = new UndirectedGraph();
            copy_graph(c, *new_subgraph);
            queue.emplace_back(new_subgraph, next.Depth, childNum);
            std::push_heap(queue.begin(), queue.end());
        }
        delete next.Subgraph;
    }
}

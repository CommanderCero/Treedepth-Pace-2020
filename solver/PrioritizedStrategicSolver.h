#pragma once

#include <queue>

#include "BaseTypes.h"
#include "Tree.h"

struct Node
{
    Node(UndirectedGraph* graph, int depth, int parentNum)
        : Subgraph(graph), Depth(depth), ParentNum(parentNum)
    {

    }

    friend bool operator< (const Node& left, const Node& right)
    {
        return (num_vertices(*left.Subgraph) + left.Depth) < (num_vertices(*right.Subgraph) + right.Depth);
    }

    UndirectedGraph* Subgraph;
    int Depth;
    int ParentNum;
};

class PrioritizedStrategicSolver
{
public:
    PrioritizedStrategicSolver();
    void Decompose(const UndirectedGraph& graph, Tree& bucket);
    void FinishPartialSolution(Tree& bucket, std::vector<int>& depthLookup);

    void Init(UndirectedGraph* graph);
    void Step(Tree& bucket, std::vector<int>& depthLookup);

    inline bool Done()
    {
        return queue.size() == 0;
    }

private:
    std::vector<Node> queue;
};
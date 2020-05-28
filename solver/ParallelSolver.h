#pragma once

#include <thread>
#include <mutex>
#include <limits>

#include "BaseTypes.h"
#include "Tree.h"

template<typename Solver>
class ParallelSolver
{
public:
    ParallelSolver(Solver& solver, UndirectedGraph& graph)
        : solver(&solver), graph(&graph), solverThread(), solutionMutex(), bestDepth(std::numeric_limits<int>::max()), bestTree(nullptr)
    {
    }

    void Start()
    {
        solverThread = std::thread(&ParallelSolver::FindSolutions, this);
    }

    std::shared_ptr<Tree> GetBestSolution()
    {
        std::lock_guard<std::mutex> lg(solutionMutex);
        return bestTree;
    }

private:
    Solver* solver;
    UndirectedGraph* graph;
    // This thread runs the solver in an infinite loop
    std::thread solverThread;
    // Threads have to lock this object, otherwise they are not allowed to access the soltion
    // This is done to prevent invalid states, when we want to overwrite the best solution
    std::mutex solutionMutex;

    // Best Solution data
    int bestDepth;
    std::shared_ptr<Tree> bestTree;

    void FindSolutions()
    {
        int newDepth;
        Tree treeBucket{num_vertices(*graph)};
        while(true)
        {
            // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            solver->Decompose(*graph, treeBucket);
            // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            solutionMutex.lock(); // Checks if any thread is currently busy with our recent best solution
            // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

            if(treeBucket.Depth < bestDepth)
            {
                std::cerr << "Found tree with depth " << treeBucket.Depth << std::endl;
                bestDepth = treeBucket.Depth;
                bestTree = std::make_shared<Tree>(treeBucket);
            }
            solutionMutex.unlock();

            // Reset depth
            treeBucket.Depth = 0;
        }
    }
};
#include <string>
#include <iostream>
#include <fstream>

#include "EntryPoint.h"
#include "PrioritizedStrategicSolver.h"
#include "ChainSolver.h"

#if SUBMISSION == 1
#include <signal.h>
//#include <unistd.h>
#include <cstring>
#include <cmath>

bool sigterm = false;
void sigtermHandler(int signum)
{
    sigterm = true;
}

int main()
{
    // // Register Sigterm
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigtermHandler;
    sigaction(SIGTERM, &action, NULL);

    // Read Graph
    UndirectedGraph graph = read_graph(std::cin);

    // Remove chains
    std::vector<int> depthLookup(num_vertices(graph));
    Tree treeBucket{num_vertices(graph)};

    auto allVertices = vertices(graph);
    UndirectedGraph* compressedGraph = RemoveChains(allVertices.first, allVertices.second, graph, treeBucket, depthLookup);
    
    // Initialize variables
    PrioritizedStrategicSolver solver;
    Tree bestTree;
    bestTree.Depth = std::numeric_limits<int>::max();

    // Solve
    solver.Init(new UndirectedGraph(*compressedGraph));
    while(!sigterm)
    {
        solver.Step(treeBucket, depthLookup);

        if(solver.Done() || treeBucket.Depth >= bestTree.Depth)
        {
            if(treeBucket.Depth < bestTree.Depth)
            {
                std::cerr << "Found tree with depth " << treeBucket.Depth << std::endl;
                bestTree = treeBucket;
            }

            // Reinitialize the solver
            treeBucket.Depth = 0;
            solver.Init(new UndirectedGraph(*compressedGraph));
        }
    }
    
    // The partial solution may be better than previously found solutions
    solver.FinishPartialSolution(treeBucket, depthLookup);
    if(treeBucket.Depth < bestTree.Depth)
    {
        write_stdout(treeBucket);
    }
    else
    {
        write_stdout(bestTree);
    }
    
    return 0;
}
#endif

#if SUBMISSION == 0
int main()
{
    // Read graph
    std::string inputPath = "../public_files/heur_005.gr";
    std::ifstream inputStream(inputPath);
    UndirectedGraph graph = read_graph(inputStream);
    
    // Create solver
    PrioritizedStrategicSolver solver;
    auto p = new ParallelSolver<PrioritizedStrategicSolver>{solver, graph};
    p->Start();

    std::this_thread::sleep_for(std::chrono::seconds(10));
    auto bestTree = p->GetBestSolution();
    if(bestTree == nullptr)
    {
        std::cout << "Couldn't find a solution in time" << std::endl;
    }
    else
    {
        write_file(inputPath, *bestTree);
    }

    exit(0);
    return 0;
}
#endif
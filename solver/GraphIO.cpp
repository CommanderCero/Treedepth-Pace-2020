#include <vector>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "BaseTypes.h"
#include "GraphIO.h"

UndirectedGraph read_graph(std::istream& stream)
{
    UndirectedGraph graph;
    std::vector<VIndex> vertices;
    for (std::string line; std::getline(stream, line);)
    {
        if (boost::starts_with(line, "c"))
            continue;
        
        if (boost::starts_with(line, "p"))
        {
            // // save node and edge number
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of(" "));

            int numNodes = std::stoi(strs[2]);
            for(int i = 0; i < numNodes; i++)
            {
                vertices.push_back(boost::add_vertex(Vertex{i}, graph));
            }
            // nEdges = std::stoi(strs[3]);
        }
        else {
            // split string
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of(" "));
            std::vector<int> edge(strs.size());
            // add new edge
            int fromNode = std::stoi(strs[0]) - 1;
            int toNode = std::stoi(strs[1]) - 1;

            boost::add_edge(vertices[fromNode], vertices[toNode], graph);
        }
    }
    
    return graph;
}

void write_file(std::string input_path, const Tree& solution){
    std::string output_path = input_path.substr(0, input_path.length()-2) + "tree";
    std::ofstream out;
    out.open(output_path);
    out << solution.Depth << std::endl;
    for (int i = 0; i < solution.ParentLookup.size(); ++i){
        out << solution.ParentLookup[i] + 1 << std::endl;
    }
    out.close();
}


void write_stdout(const Tree& solution){
    std::cout << solution.Depth << std::endl;
    int solution_size = solution.ParentLookup.size();
    for (int i = 0; i < solution_size; ++i){
        std::cout << solution.ParentLookup[i] + 1;
        if (i != solution_size-1){
           std::cout << std::endl;
        }
    }
}
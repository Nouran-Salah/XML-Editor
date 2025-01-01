#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "file_processing.cpp"


int visualize_graph(const std::map<int, std::vector<int>>& graph, std::string output_file) {
    std::stringstream structure;

    for(auto& g : graph) {
        int target_user = g.first;
        std::vector<int> followers = g.second;
        structure << target_user << "\n";
        for(int follower : followers) {
            structure << follower << " -> " << target_user << "\n";
        }
    }
    
    write_file("digraph {\n" + structure.str() + "}", "graph.gv");

    return system(("dot -Tpng graph.gv -o " + output_file + ".png").data());
}

int visualize_graph(const std::map<int, std::vector<int>>& graph) {
   return visualize_graph(graph, "graph");
}


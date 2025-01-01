
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "xml_tree.h"
#include "XML_to_JSON.h"
#include "XML_minifying.h"
#include "file_processing.h"
#include "validate_fix.h"
#include "Prettifying.h"
#include "compression_decompression.h"
#include "graph_h.h"

using namespace std;



int main(int argc, char* argv[]) {

    string command = argv[1];
    string inputFile = argv[command == "search" ? 5 : 3];

    string XML = read_file("input_file.xml");
    if (XML.empty()) {
        cerr << "Error: Failed to read input_file.xml or the file is empty." << endl;
        return 1;
    }

    string Jsonfile = "output_file.json";
    string OutputFile = "output_file.xml";
    string compressedFile = "outputfile.comp";
    unordered_map<string, string> dictionary;

    string xmlContent;

    try {
        write_file(corrected_xml(readFile(inputFile)), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        xmlContent = read_file("temp2.xml");
    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    size_t pos = 0;
    TreeNode* root = parseXML(xmlContent, pos);


    map<int, vector<int>> graph;
    map<int, string> idNameMap;
    buildGraph(root, graph);
    buildIdNameMap(root, idNameMap);

    // Handle the commands
     if (command  == "verify") {
        if (is_valid(XML)) {
            cout << "\nThe XML is valid.\n";
        } else {
            cout << "\nThe XML is not valid.\n";
        }
        write_file(corrected_xml(XML), OutputFile);
        cout << "Verified XML saved to OutputFile" << endl;

    } else if (command  == "mini") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", OutputFile);
        cout << "Minified XML saved to OutputFile" << endl;

    } else if (command  == "format") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        prettifyXML("temp2.xml", OutputFile);
        cout << "Prettifed XML saved to OutputFile" << endl;

    } else if (command  == "json") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        write_file(convertXMLToJSON(read_file("temp2.xml")), Jsonfile);
        cout << "Json file saved to OutputFile" << endl;

    } else if (command  == "compress") {
        string compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
        write_file(compressedXML, compressedFile);

    } else if (command == "decompress") {
        string compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
        string decompressedXML = Decompress(compressedXML, dictionary);
        write_file(decompressedXML, OutputFile);
    }
    else if (command == "most_influencer") {
        vector<pair<int, string>> mostInfluentialUsers = findMostInfluentialUsers(graph, idNameMap);
        cout << "Most Influential Users:\n";
        for (const auto& user : mostInfluentialUsers) {
            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
        }
    }
    else if (command == "most_active") {
        map<int, vector<int>> followingMap;
        buildFollowingMap(graph, followingMap);
        vector<pair<int, string>> mostActiveUsers = findMostActiveUsers(graph, idNameMap, followingMap);
        cout << "Most Active Users:\n";
        for (const auto& user : mostActiveUsers) {
            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
        }
    }
    else if (command == "search") {
    string searchType = argv[2];
    string query = argv[3];

    if (searchType == "-w") {
        cout << "The word '" << query << "' is found in:\n";
        vector<string> wordResults = searchByWord(root, query);
        for (const string& result : wordResults) {
            cout << result << endl;
        }
    }
    else if (searchType == "-t") {
        cout << "The topic '" << query << "' is found in:\n";
        vector<string> topicResults = searchByTopic(root, query);
        for (const string& result : topicResults) {
            cout << result << endl;
        }
    }
        else {
            cerr << "Invalid search type. Use '-w' for word search or '-t' for topic search.\n";
        }
    }
    else if (command == "suggest") {
        if (argc < 6 || string(argv[4]) != "-id") {
            cerr << "Error: Usage: " << argv[0] << " suggest -i <input_file.xml> -id <id>\n";
            return 1;
        }

        // Parse the user ID from the -id flag
        int userId = stoi(argv[5]);

        // Use the new function to suggest users
        suggestUsersToFollowForId(graph, userId);
    }
    else if (command == "mutual") {
        if (argc < 6 || string(argv[4]) != "-ids") {
            cerr << "Error: Usage: " << argv[0] << " mutual -i <input_file.xml> -ids <id1,id2,...> [-n <number>]\n";
            return 1;
        }

        // Parse the user IDs from the -ids flag
        string idsArg = argv[5];
        vector<int> userIds;
        stringstream ss(idsArg);
        string idStr;
        while (getline(ss, idStr, ',')) {
            userIds.push_back(stoi(idStr));
        }

        // Check for the optional -n flag
        size_t n = userIds.size(); // Default to using all user IDs
        if (argc >= 8 && string(argv[6]) == "-n") {
            n = stoi(argv[7]);
        }

        // Find mutual followers
        vector<int> mutualFollowers = findMutualFollowers(graph, userIds, n);

        if (mutualFollowers.empty()) {
            cout << "No mutual followers found.\n";
        }
        else {
            cout << "Mutual Followers:\n";
            for (int id : mutualFollowers) {
                cout << "ID: " << id << ", Name: " << idNameMap[id] << "\n";
            }
        }
    }
    else if (command == "draw") {
       if (visualize_graph(graph, argv[5])) {
            cout << "process failed: graphviz is either not installed or have missing configurations.\n";
        }

    }

    else {
        cerr << "Invalid command.\n";
        return 1;
    }

    cout << "Exiting program..." << endl;

    return 0;
}
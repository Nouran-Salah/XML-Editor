#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "visualization.cpp"
#include "XML_minifying.h"
#include "file_processing.h"
#include "validate_fix.h"
using namespace std;

// Structure to represent a node in the tree
struct TreeNode {
    string tagName;
    string value;
    vector<TreeNode*> children;

    TreeNode(string name, string val = "") : tagName(name), value(val) {}
};

// Function to parse XML and construct a tree
TreeNode* parseXML(const string& xml, size_t& pos) {
    size_t start = xml.find('<', pos);
    size_t end = xml.find('>', start);
    string tagName = xml.substr(start + 1, end - start - 1);

    TreeNode* node = new TreeNode(tagName);
    pos = end + 1;

    while (true) {
        if (xml[pos] == '<' && xml[pos + 1] == '/') {
            pos = xml.find('>', pos) + 1;
            break;
        }
        else if (xml[pos] == '<') {
            node->children.push_back(parseXML(xml, pos));
        }
        else {
            size_t valueEnd = xml.find('<', pos);
            node->value = xml.substr(pos, valueEnd - pos);
            pos = valueEnd;
        }
    }
    return node;
}

// Function to read a file into a string
string readFile(const string& fileName) {
    ifstream file(fileName);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to build a graph from the XML tree
void buildGraph(TreeNode* root, map<int, vector<int>>& graph) {
    if (root->tagName == "user") {
        int userId = -1;
        vector<int> followers;

        for (TreeNode* child : root->children) {
            if (child->tagName == "id") {
                userId = stoi(child->value);
            }
            else if (child->tagName == "followers") {
                for (TreeNode* follower : child->children) {
                    for (TreeNode* followerDetail : follower->children) {
                        if (followerDetail->tagName == "id") {
                            followers.push_back(stoi(followerDetail->value));
                        }
                    }
                }
            }
        }
        if (userId != -1) {
            graph[userId] = followers;
        }
    }

    for (TreeNode* child : root->children) {
        buildGraph(child, graph);
    }
}

// Function to build a map with user ID as the key and name as the value
void buildIdNameMap(TreeNode* root, map<int, string>& idNameMap) {
    if (root->tagName == "user") {
        int userId = -1;
        string userName;

        for (TreeNode* child : root->children) {
            if (child->tagName == "id") {
                userId = stoi(child->value);
            }
            else if (child->tagName == "name") {
                userName = child->value;
            }
        }
        if (userId != -1 && !userName.empty()) {
            idNameMap[userId] = userName;
        }
    }

    for (TreeNode* child : root->children) {
        buildIdNameMap(child, idNameMap);
    }
}

// Function to find the most influential users
vector<pair<int, string>> findMostInfluentialUsers(const map<int, vector<int>>& graph, const map<int, string>& idNameMap) {
    int maxFollowers = 0;
    vector<pair<int, string>> mostInfluentialUsers;

    for (const auto& [userId, followers] : graph) {
        if (followers.size() > maxFollowers) {
            maxFollowers = followers.size();
        }
    }

    for (const auto& [userId, followers] : graph) {
        if (followers.size() == maxFollowers) {
            mostInfluentialUsers.push_back({ userId, idNameMap.at(userId) });
        }
    }

    return mostInfluentialUsers;
}

// Function to build the following map
void buildFollowingMap(const map<int, vector<int>>& graph, map<int, vector<int>>& followingMap) {
    for (const auto& [userId, followers] : graph) {
        for (int followerId : followers) {
            followingMap[followerId].push_back(userId);
        }
    }
}

// Function to find the most active users
vector<pair<int, string>> findMostActiveUsers(const map<int, vector<int>>& graph, const map<int, string>& idNameMap, const map<int, vector<int>>& followingMap) {
    int maxConnections = 0;
    vector<pair<int, string>> mostActiveUsers;

    for (const auto& [userId, followers] : graph) {
        int totalConnections = followers.size();
        if (followingMap.count(userId) > 0) {
            totalConnections += followingMap.at(userId).size();
        }

        if (totalConnections > maxConnections) {
            maxConnections = totalConnections;
            mostActiveUsers = { {userId, idNameMap.at(userId)} };
        }
        else if (totalConnections == maxConnections) {
            mostActiveUsers.push_back({ userId, idNameMap.at(userId) });
        }
    }

    return mostActiveUsers;
}

// Function to suggest users to follow for a specific user
void suggestUsersToFollowForId(const map<int, vector<int>>& graph, int userId) {
    if (graph.find(userId) == graph.end()) {
        cout << "User " << userId << " does not exist in the graph.\n";
        return;
    }

    set<int> suggestions;
    set<int> alreadyFollowed(graph.at(userId).begin(), graph.at(userId).end());
    alreadyFollowed.insert(userId); // Avoid suggesting the user themselves

    for (int follower : graph.at(userId)) {
        if (graph.find(follower) != graph.end()) {
            for (int followerOfFollower : graph.at(follower)) {
                if (alreadyFollowed.find(followerOfFollower) == alreadyFollowed.end()) {
                    suggestions.insert(followerOfFollower);
                }
            }
        }
    }

    cout << "User " << userId << " might want to follow: ";
    if (suggestions.empty()) {
        cout << "No suggestions available.";
    }
    else {
        for (int suggestion : suggestions) {
            cout << suggestion << " ";
        }
    }
    cout << endl;
}

// mutual followers
vector<int> findMutualFollowers(const map<int, vector<int>>& graph, const vector<int>& users, size_t n) {
    // Ensure n does not exceed the size of the users vector
    n = min(n, users.size());

    if (n == 0) return {};

    // Initialize the set with the followers of the first user
    set<int> mutualFollowers(graph.at(users[0]).begin(), graph.at(users[0]).end());

    // Intersect with followers of the next n-1 users
    for (size_t i = 1; i < n; ++i) {
        set<int> currentFollowers(graph.at(users[i]).begin(), graph.at(users[i]).end());
        set<int> intersection;
        set_intersection(
            mutualFollowers.begin(), mutualFollowers.end(),
            currentFollowers.begin(), currentFollowers.end(),
            inserter(intersection, intersection.begin())
        );
        mutualFollowers = move(intersection); // Update mutualFollowers with the intersection
    }

    // Convert the result back to a vector for easier usage
    return vector<int>(mutualFollowers.begin(), mutualFollowers.end());
}


string toLower(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Function to search for a word in the body of posts and return results
void searchByWordHelper(TreeNode* root, const string& word, vector<string>& bodies) {
    string lowerWord = toLower(word);
    if (root->tagName == "post") {
        for (TreeNode* child : root->children) {
            if (child->tagName == "body" && toLower(child->value).find(lowerWord) != string::npos) {
                bodies.push_back(child->value);
            }
        }
    }

    for (TreeNode* child : root->children) {
        searchByWordHelper(child, word, bodies);
    }
}

vector<string> searchByWord(TreeNode* root, const string& word)
{
    vector<string> bodies;
    searchByWordHelper(root, word, bodies);
    return bodies;
}

// Function to search for a specific topic in the posts and return results
void searchByTopicHelper(TreeNode* root, const string& topic, vector<string>& bodies) {
    string lowerTopic = toLower(topic);

    if (root->tagName == "post") {
        bool topicFound = false;

        for (TreeNode* child : root->children) {
            if (child->tagName == "topics") {
                for (TreeNode* topicNode : child->children) {
                    if (topicNode->tagName == "topic" && toLower(topicNode->value) == lowerTopic) {
                        topicFound = true;
                        break;
                    }
                }
            }
        }
        if (topicFound) {
            for (TreeNode* child : root->children) {
                if (child->tagName == "body") {
                    bodies.push_back(child->value);
                }
            }
        }
    }

    for (TreeNode* child : root->children) {
        searchByTopicHelper(child, topic, bodies);
    }
}

vector<string> searchByTopic(TreeNode* root, const string& topic) {
    vector<string> bodies;
    searchByTopicHelper(root, topic, bodies);
    return bodies;
}

//int main(int argc, char argv[]) {
//    // Ensure there are at least 4 arguments (command, -i, and file)
//    if (argc < 4) {
//        cerr << "Usage: " << argv[0] << " [most_influencer|most_active|mutual|suggest|search] -i <input_file.xml> [-ids <id1,id2,...>|-id <id>|-w <word>|-t <topic>]\n";
//        return 1;
//    }
//
//    string command = argv[1];
//
//    // Ensure the second argument is -i
//    if (argc < 4 || string(argv[2]) != "-i") {
//        cerr << "Usage: " << argv[0] << " [most_influencer|most_active|mutual|suggest|search|draw] -i <input_file.xml> [-ids <id1,id2,...>|-id <id>|-w <word>|-t <topic>]\n";
//        return 1;
//    }
//
//    // Get the XML file path
//    string fileName = argv[3];
//
//
//    // Read the XML content from the file
//    string xmlContent = readFile(fileName);
//
//    // Parse the XML content into a tree structure
//    size_t pos = 0;
//
//    TreeNode* root = parseXML(xmlContent, pos);
//
//    // Build graph and ID -> name map
//    map<int, vector<int>> graph;
//    map<int, string> idNameMap;
//    buildGraph(root, graph);
//    buildIdNameMap(root, idNameMap);
//
//    // Handle the commands
//    if (command == "most_influencer") {
//        vector<pair<int, string>> mostInfluentialUsers = findMostInfluentialUsers(graph, idNameMap);
//        cout << "Most Influential Users:\n";
//        for (const auto& user : mostInfluentialUsers) {
//            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
//        }
//    }
//    else if (command == "most_active") {
//        map<int, vector<int>> followingMap;
//        buildFollowingMap(graph, followingMap);
//        vector<pair<int, string>> mostActiveUsers = findMostActiveUsers(graph, idNameMap, followingMap);
//        cout << "Most Active Users:\n";
//        for (const auto& user : mostActiveUsers) {
//            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
//        }
//    }
//    else if (command == "search") {
//        if (argc < 6) {
//            cerr << "Usage: " << argv[0] << " search -w <word> | -t <topic> -i <input_file.xml>\n";
//            return 1;
//        }
//
//        // Now check if we have the right flags for search
//        string searchType = argv[4];
//        string query = argv[5];
//
//        if (searchType == "-w") {
//            cout << "Searching for word: " << query << "\n";
//            searchByWord(root, query);
//        }
//        else if (searchType == "-t") {
//            cout << "Searching for topic: " << query << "\n";
//            vector<string> bodies;  // To store body content of posts matching the topic
//            searchByTopic(root, query, bodies);
//
//            if (bodies.empty()) {
//                cout << "No posts found for topic: " << query << "\n";
//            }
//            else {
//                cout << "Posts found for topic " << query << ":\n";
//                for (const string& body : bodies) {
//                    cout << body << "\n";
//                }
//            }
//        }
//        else {
//            cerr << "Invalid search type. Use -w for word or -t for topic.\n";
//            return 1;
//        }
//    }
//
//    else if (command == "suggest") {
//        if (argc < 6 || string(argv[4]) != "-id") {
//            cerr << "Error: Usage: " << argv[0] << " suggest -i <input_file.xml> -id <id>\n";
//            return 1;
//        }
//
//        // Parse the user ID from the -id flag
//        int userId = stoi(argv[5]);
//
//        // Use the new function to suggest users
//        suggestUsersToFollowForId(graph, userId);
//
//    }
//    else if (command == "mutual") {
//        if (argc < 6 || string(argv[4]) != "-ids") {
//            cerr << "Error: Usage: " << argv[0] << " mutual -i <input_file.xml> -ids <id1,id2,...> [-n <number>]\n";
//            return 1;
//        }
//
//        // Parse the user IDs from the -ids flag
//        string idsArg = argv[5];
//        vector<int> userIds;
//        stringstream ss(idsArg);
//        string idStr;
//        while (getline(ss, idStr, ',')) {
//            userIds.push_back(stoi(idStr));
//        }
//
//        // Check for the optional -n flag
//        size_t n = userIds.size(); // Default to using all user IDs
//        if (argc >= 8 && string(argv[6]) == "-n") {
//            n = stoi(argv[7]);
//        }
//
//        // Find mutual followers
//        vector<int> mutualFollowers = findMutualFollowers(graph, userIds, n);
//
//        if (mutualFollowers.empty()) {
//            cout << "No mutual followers found.\n";
//        }
//        else {
//            cout << "Mutual Followers:\n";
//            for (int id : mutualFollowers) {
//                cout << "ID: " << id << ", Name: " << idNameMap[id] << "\n";
//            }
//        }
//    }
//    else if (command == "draw") {
//        if (visualize_graph(graph, argv[5])) {
//            cout << "process failed: graphviz is either not installed or have missing configurations.\n";
//        }
//
//    }
//    else {
//        cerr << "Invalid command. Supported commands are [most_influencer|most_active|mutual|suggest|search|draw].\n";
//        return 1;
//    }
//
//    return 0;
//}



int main(int argc, char* argv[]) {

    string command = argv[1];
    string inputFile = argv[command == "search" ? 5 : 3];

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
    if (command == "most_influencer") {
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
            searchByWord(root, query);
        }
        else if (searchType == "-t") {
            cout << "The topic '" << query << "' is found in:\n";
            searchByTopic(root, query);
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
    else {
        cerr << "Invalid command. Supported commands are [most_influencer|most_active|mutual|suggest|search].\n";
        return 1;
    }

    return 0;
}

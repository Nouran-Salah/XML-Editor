#include <iostream>
#include <string>
#include <vector>
#include <map>
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
    // Parse opening tag
    size_t start = xml.find('<', pos);
    size_t end = xml.find('>', start);
    string tagName = xml.substr(start + 1, end - start - 1);

    TreeNode* node = new TreeNode(tagName);
    pos = end + 1;

    // Parse content or nested tags
    while (true) {
        if (xml[pos] == '<' && xml[pos + 1] == '/') { // Closing tag
            pos = xml.find('>', pos) + 1;
            break;
        } else if (xml[pos] == '<') { // Nested tag
            node->children.push_back(parseXML(xml, pos));
        } else { // Value
            size_t valueEnd = xml.find('<', pos);
            node->value = xml.substr(pos, valueEnd - pos);
            pos = valueEnd;
        }
    }
    return node;
}

// Function to traverse tree and extract graph data
void buildGraph(TreeNode* root, map<int, vector<int>>& graph) {
    if (root->tagName == "user") {
        int userId = -1;
        vector<int> followers;

        // Traverse user node
        for (TreeNode* child : root->children) {
            if (child->tagName == "id") {
                userId = stoi(child->value);
            } else if (child->tagName == "followers") {
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

    // Recursively traverse other users
    for (TreeNode* child : root->children) {
        buildGraph(child, graph);
    }
}

// Function to print the graph
/*void printGraph(const map<int, vector<int>>& graph) {
    for (const auto& [user, followers] : graph) {
        cout << "User " << user << " is followed by: ";
        for (int follower : followers) {
            cout << follower << " ";
        }
        cout << endl;
    }
}*/

// Function to build a map with user ID as the key and name as the value
void buildIdNameMap(TreeNode* root, map<int, string>& idNameMap) {
    if (root->tagName == "user") {
        int userId = -1;
        string userName;

        for (TreeNode* child : root->children) {
            if (child->tagName == "id") {
                userId = stoi(child->value);
            } else if (child->tagName == "name") {
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
            mostInfluentialUsers.push_back({userId, idNameMap.at(userId)});
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
            mostActiveUsers = {{userId, idNameMap.at(userId)}};
        } else if (totalConnections == maxConnections) {
            mostActiveUsers.push_back({userId, idNameMap.at(userId)});
        }
    }

    return mostActiveUsers;
}

int main(int argc, char* argv[]) {
    if (argc != 4 || string(argv[2]) != "-i") {
        cerr << "Usage: " << argv[0] << " [most_influencer|most_active] -i <input_file.xml>\n";
        return 1;
    }

    string command = argv[1];
    string fileName = argv[3];

    // Read the XML file into a string
    string xmlContent = readFile(fileName);

    size_t pos = 0;
    TreeNode* root = parseXML(xmlContent, pos);

    map<int, vector<int>> graph;
    map<int, string> idNameMap;

    buildGraph(root, graph);
    buildIdNameMap(root, idNameMap);

    if (command == "most_influencer") {
        vector<pair<int, string>> mostInfluentialUsers = findMostInfluentialUsers(graph, idNameMap);
        cout << "Most Influential Users:\n";
        for (const auto& user : mostInfluentialUsers) {
            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
        }
    } else if (command == "most_active") {
        map<int, vector<int>> followingMap;
        buildFollowingMap(graph, followingMap);

        vector<pair<int, string>> mostActiveUsers = findMostActiveUsers(graph, idNameMap, followingMap);
        cout << "Most Active Users:\n";
        for (const auto& user : mostActiveUsers) {
            cout << "ID: " << user.first << ", Name: " << user.second << "\n";
        }
    } else {
        cerr << "Invalid command. Use 'most_influencer' or 'most_active'.\n";
        return 1;
    }

    return 0;
}

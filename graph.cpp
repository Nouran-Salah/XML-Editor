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
void printGraph(const map<int, vector<int>>& graph) {
    for (const auto& [user, followers] : graph) {
        cout << "User " << user << " is followed by: ";
        for (int follower : followers) {
            cout << follower << " ";
        }
        cout << endl;
    }
}

int main() {
    string xml = "<users><user><id>1</id><name>Ahmed Ali</name><posts>...</posts><followers><follower><id>2</id></follower><follower><id>3</id></follower></followers></user><user><id>2</id><name>Yasser Ahmed</name><posts>...</posts><followers><follower><id>1</id></follower></followers></user><user><id>3</id><name>Mohamed Sherif</name><posts>...</posts><followers><follower><id>1</id></follower></followers></user></users>";
    size_t pos = 0;

    TreeNode* root = parseXML(xml, pos);

    map<int, vector<int>> graph;
    buildGraph(root, graph);

    printGraph(graph);

    return 0;
}

#include "graph.h"
#include <QString>
#include <QDebug>

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
        } else if (xml[pos] == '<') {
            node->children.push_back(parseXML(xml, pos));
        } else {
            size_t valueEnd = xml.find('<', pos);
            node->value = xml.substr(pos, valueEnd - pos);
            pos = valueEnd;
        }
    }
    return node;
}

string readFile(const string& fileName) {
    ifstream file(fileName);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void buildGraph(TreeNode* root, map<int, vector<int>>& graph) {
    if (root->tagName == "user") {
        int userId = -1;
        vector<int> followers;

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

void buildFollowingMap(const map<int, vector<int>>& graph, map<int, vector<int>>& followingMap) {
    for (const auto& [userId, followers] : graph) {
        for (int followerId : followers) {
            followingMap[followerId].push_back(userId);
        }
    }
}

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

vector<int> suggestUsersToFollowForId(const map<int, vector<int>>& graph, int userId, const map<int, string>& idNameMap) {
    if (graph.find(userId) == graph.end()) {
        cout << "User " << userId << " does not exist in the graph.\n";
        return {};
    }

    set<int> suggestions;
    set<int> alreadyFollowed(graph.at(userId).begin(), graph.at(userId).end());
    alreadyFollowed.insert(userId); // Avoid suggesting the user themselves

    // Find second-degree connections
    for (int follower : graph.at(userId)) {
        if (graph.find(follower) != graph.end()) {
            for (int followerOfFollower : graph.at(follower)) {
                if (alreadyFollowed.find(followerOfFollower) == alreadyFollowed.end()) {
                    suggestions.insert(followerOfFollower);
                }
            }
        }
    }

    vector<int> suggestedUsers;
    if (!suggestions.empty()) {
        cout << "User " << userId << " (" << idNameMap.at(userId) << ") might want to follow:\n";
        for (int suggestion : suggestions) {
            suggestedUsers.push_back(suggestion);
            cout << "ID: " << suggestion << ", Name: " << idNameMap.at(suggestion) << "\n";
        }
    } else {
        cout << "No suggestions for user " << userId << " (" << idNameMap.at(userId) << ").\n";
    }

    return suggestedUsers;
}

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



vector<string> searchByWordHelper(TreeNode* root, const string& word)
{
    vector<string> bodies;
    searchByWordHelper(root, word, bodies);
    return bodies;
    if (bodies.empty()) {
        cout << "The word '" << word << "' was not found in any post." << endl;
    } else {
        cout << "Posts mentioning word '" << word << "':\n";
        for (size_t i = 0; i < bodies.size(); ++i) {
            cout << "Post " << (i + 1) << ": " << bodies[i] << endl;
        }
    }
}

vector<string> searchByWord(TreeNode* root, const string& word)
{
    vector<string> bodies;
    searchByWordHelper(root, word, bodies);
    return bodies;
}

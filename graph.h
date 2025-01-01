#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
//#include "visualization.cpp"

using namespace std;

// Structure to represent a node in the tree
struct TreeNode {
    string tagName;
    string value;
    vector<TreeNode*> children;

    TreeNode(string name, string val = "") : tagName(name), value(val) {}
};

// Function declarations
TreeNode* parseXML(const string& xml, size_t& pos);
string readFile(const string& fileName);
void buildGraph(TreeNode* root, map<int, vector<int>>& graph);
void buildIdNameMap(TreeNode* root, map<int, string>& idNameMap);
vector<pair<int, string>> findMostInfluentialUsers(const map<int, vector<int>>& graph, const map<int, string>& idNameMap);
void buildFollowingMap(const map<int, vector<int>>& graph, map<int, vector<int>>& followingMap);
vector<pair<int, string>> findMostActiveUsers(const map<int, vector<int>>& graph, const map<int, string>& idNameMap, const map<int, vector<int>>& followingMap);
vector<int> suggestUsersToFollowForId(const map<int, vector<int>>& graph, int userId, const map<int, string>& idNameMap);
vector<int> findMutualFollowers(const map<int, vector<int>>& graph, const vector<int>& users, size_t n);
std::string toLower(const std::string& str);

// Search posts by a specific word in the body
std::vector<std::string> searchByWord(TreeNode* root, const std::string& word);

// Search posts by a specific topic
std::vector<std::string> searchByTopic(TreeNode* root, const std::string& topic);

#endif // HEADER_H#pragma once
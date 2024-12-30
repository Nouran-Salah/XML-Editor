#ifndef xmlTree_H
#define xmlTree_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Node {
public:
    std::string tagName;
    std::string tagValue;
    std::vector<Node*> children;
    Node* parent;

    Node(const std::string& tagName);

    void addChild(Node* child);

    std::string getTagName() const;
    std::string getTagValue() const;
};

std::string extractTagName(const std::string& line);
std::string extractTagValue(const std::string& line, const std::string& tagName);
Node* parseXML(const std::string& xml);

#endif 

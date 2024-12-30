#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>
#include "xml_tree.h"


Node::Node(const std::string& tagName) : tagName(tagName), parent(nullptr) {}

void Node::addChild(Node* child) {
    children.push_back(child);
    child->parent = this;
}

std::string Node::getTagName() const { return tagName; }
std::string Node::getTagValue() const { return tagValue; }

std::string extractTagName(const std::string& line) {
    std::regex pattern("<(.*?)>");
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
        return matches[1].str();
    }
    return "";
}

std::string extractTagValue(const std::string& line, const std::string& tagName) {
    std::string pattern = "<" + tagName + ">(.*?)</" + tagName + ">";
    std::regex reg(pattern);
    std::smatch matches;
    if (std::regex_search(line, matches, reg)) {
        return matches[1].str();
    }
    return "";
}

Node* parseXML(const std::string& xml) {
    Node* root = nullptr;
    Node* currentNode = nullptr;
    std::string currentText;

    std::istringstream iss(xml);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.find('<') != std::string::npos) {
            std::string tagName = extractTagName(line);

            if (tagName.empty()) {
                continue;
            }

            if (tagName.front() == '/') {
                if (currentNode) {
                    currentNode->tagValue = currentText;
                    currentNode = currentNode->parent;
                }
            }
            else {
                if (line.find('>') != line.rfind('>')) {
                    std::string tagValue = extractTagValue(line, tagName);
                    Node* newNode = new Node(tagName);
                    newNode->tagValue = tagValue;
                    if (currentNode) {
                        currentNode->addChild(newNode);
                    }
                    else {
                        root = newNode;
                    }
                }
                else {
                    Node* newNode = new Node(tagName);
                    if (currentNode) {
                        currentNode->addChild(newNode);
                    }
                    else {
                        root = newNode;
                    }
                    currentNode = newNode;
                }
            }
            currentText = "";
        }
        else {
            currentText += line + "\n";
        }
    }

    return root;
}

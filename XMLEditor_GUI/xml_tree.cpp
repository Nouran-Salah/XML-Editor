#include <iostream>
#include <string>
#include <vector>
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

//updated function
std::string extractTagName(const std::string& line) {
    size_t start = line.find('<');
    size_t end = line.find('>');
    if (start != std::string::npos && end != std::string::npos && end > start + 1) {
        return line.substr(start + 1, end - start - 1);
    }
    return "";
}

//updated function
std::string extractTagValue(const std::string& line, const std::string& tagName) {
    std::string startTag = "<" + tagName + ">";
    std::string endTag = "</" + tagName + ">";
    size_t start = line.find(startTag);
    size_t end = line.find(endTag);
    if (start != std::string::npos && end != std::string::npos) {
        return line.substr(start + startTag.length(), end - start - startTag.length());
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

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <fstream>

std::string xmlString(std::string xml) {
    std::string xmlWithLines = xml;
    int n = (int)xml.length();
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            xmlWithLines[j] = xml[i];
            xmlWithLines.resize(xmlWithLines.length() + 1);
            xmlWithLines[j++] = '\n';
        }
        else if (xml[i] == '>' && xml[i + 1] == '<') {
            xmlWithLines[j++] = xml[i];
            xmlWithLines.resize(xmlWithLines.length() + 1);
            xmlWithLines[j++] = '\n';
        }
        else {
            xmlWithLines[j++] = xml[i];
        }
    }
    return xmlWithLines;
}

class Node {
public:
    std::string tagName;
    std::string tagValue;
    std::vector<Node*> children;
    Node* parent;

    Node(const std::string& tagName) : tagName(tagName), parent(nullptr) {}

    void addChild(Node* child) {
        children.push_back(child);
        child->parent = this;
    }

    std::string getTagName() const { return tagName; }
    std::string getTagValue() const { return tagValue; }
};

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


void printJsonTree(const Node* node, int level, std::string& jsonBuilder, bool is_multilevel) {
    std::string indentation = "    ";
    std::string indent = std::string(level, indentation[0]);

    jsonBuilder += indent;

    if (!is_multilevel) {
        jsonBuilder += "\"" + node->getTagName() + "\": ";
    }

    if (node->children.empty()) {
        jsonBuilder += "\"" + node->getTagValue() + "\"";
        return;
    }

    bool many_children_and_same_name = (node->children.size() > 1) &&
        (node->children[0]->getTagName() == node->children[1]->getTagName());

    if (!many_children_and_same_name) {
        jsonBuilder += "{\n";
        for (size_t i = 0; i < node->children.size(); ++i) {
            printJsonTree(node->children[i], level + 1, jsonBuilder, false);
            if (i < node->children.size() - 1) {
                jsonBuilder += ",";
            }
            jsonBuilder += "\n";
        }
        jsonBuilder += indent + "}";
        return;
    }

    jsonBuilder += "{\n" + indent + indentation + "\"" + node->children[0]->getTagName() + "\": [\n";
    for (size_t i = 0; i < node->children.size(); ++i) {
        printJsonTree(node->children[i], level + 2, jsonBuilder, true);
        if (i < node->children.size() - 1) {
            jsonBuilder += ",";
        }
        jsonBuilder += "\n";
    }
    jsonBuilder += indent + indentation + "]\n" + indent + "}";
}

void convertXMLToJSON(const std::string& xml) {
    std::string xml_string = xmlString(xml);
    Node* root = parseXML(xml_string);

    std::string jsonString;
    printJsonTree(root, 1, jsonString, false);

    std::cout << "{\n";
    std::cout << jsonString << std::endl;
    std::cout << "}\n";
}



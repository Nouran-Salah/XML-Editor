#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

string xmlString(string xml) {
    string xmlWithLines = xml;
    int n = (int)xml.length();
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            xmlWithLines[j++] = xml[i];
            xmlWithLines.resize(xmlWithLines.length() + 1);
            xmlWithLines[j] = '\n';
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

string extractTagName(const string& line) {
    size_t start = line.find('<');
    size_t end = line.find('>');
    if (start != string::npos && end != string::npos && end > start + 1) {
        return line.substr(start + 1, end - start - 1);
    }
    return "";
}

string extractTagValue(const string& line, const string& tagName) {
    string startTag = "<" + tagName + ">";
    string endTag = "</" + tagName + ">";
    size_t start = line.find(startTag);
    size_t end = line.find(endTag);
    if (start != string::npos && end != string::npos) {
        return line.substr(start + startTag.length(), end - start - startTag.length());
    }
    return "";
}

Node* parseXML(const string& xml) {
    Node* root = nullptr;
    Node* currentNode = nullptr;
    string currentText;

    istringstream iss(xml);
    string line;

    while (getline(iss, line)) {
        if (line.find('<') != string::npos) {
            string tagName = extractTagName(line);

            if (tagName.front() == '/') {
                if (currentNode) {
                    currentNode->tagValue = currentText;
                    currentNode = currentNode->parent;
                }
            }
            else {
                if (line.find('>') != line.rfind('>')) { // Opening and closing tag on the same line
                    string tagValue = extractTagValue(line, tagName);
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

void printJsonTree(const Node* node, int level, string& jsonBuilder, bool is_multilevel) {
    string indentation = "    ";
    string indent = string(level, indentation[0]);

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



//the function to be called in main
void convertXMLToJSON(const std::string& xml) {
    std::string xml_string = xmlString(xml);
    Node* root = parseXML(xml_string);

    std::string jsonString;
    printJsonTree(root, 1, jsonString, false);

    std::cout << "{\n";
    std::cout << jsonString << std::endl;
    std::cout << "}\n";
}


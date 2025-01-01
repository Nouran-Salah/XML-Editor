#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include "xml_tree.h"
#include "XML_To_JSON.h"

using namespace std;


std::string xmlString(const std::string& xml) {
    std::string xmlWithLines = xml;
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
    jsonBuilder = "{\n" + jsonBuilder + "\n}\n";
}

void convertXMLToJSON(const std::string& xml, std::string& jsonString) {
    std::string xml_string = xmlString(xml);
    Node* root = parseXML(xml_string);

    printJsonTree(root, 1, jsonString, false);

}

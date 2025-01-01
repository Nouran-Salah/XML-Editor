#ifndef XML_TO_JSON_H
#define XML_TO_JSON_H

#include "xml_tree.h"
#include <string>


// Converts a flat XML string into a formatted string with each tag on a new line
std::string xmlString(const std::string& xml);

// Recursively builds a JSON string representation of an XML tree
void printJsonTree(const Node* node, int level, std::string& jsonBuilder, bool is_multilevel);

// Converts an XML string to a JSON string and prints it to the console
void convertXMLToJSON(const std::string& xml, std::string& jsonString);

#endif // XML_TO_JSON_H
#pragma once


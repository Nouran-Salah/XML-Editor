#ifndef xml2json_H
#define xml2json_H

#include <string>


// Converts a flat XML string into a formatted string with each tag on a new line
std::string xmlString(const std::string& xml);

// Recursively builds a JSON string representation of an XML tree
void printJsonTree(const Node* node, int level, std::string& jsonBuilder, bool is_multilevel);

// Converts an XML string to a JSON string and prints it to the console
void convertXMLToJSON(const std::string& xml);

#endif // XML_TO_JSON_H
#pragma once

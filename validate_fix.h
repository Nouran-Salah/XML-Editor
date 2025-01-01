#ifndef XML_UTILS_H
#define XML_UTILS_H

#include <string>
#include <stack>

// Function to check if an XML string is valid
bool is_valid(std::string xml);

// Function to create an opening XML tag with optional attributes
std::string makeXML_open_tag(std::string name, std::string attributes = "");

// Function to create a closing XML tag
std::string makeXML_close_tag(std::string name);

// Function to correct invalid XML strings by fixing or adding necessary tags
std::string corrected_xml(std::string xml);

#endif // XML_UTILS_H

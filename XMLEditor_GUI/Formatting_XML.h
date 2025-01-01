#ifndef FORMATTING_XML_H
#define FORMATTING_XML_H
#include <iostream>
#include <fstream>  // For std::ifstream and std::ofstream

using namespace std;

void minifyXML(std::string inputFile, std::string outputFile);
void prettifyXML(const std::string& inputFile, const std::string& outputFile);

#endif // FORMATTING_XML_H

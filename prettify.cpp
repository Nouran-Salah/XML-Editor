#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

void prettifyXML(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }
    if (!output.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    std::string line, currentTag;
    int indentLevel = 0;
    bool insideTag = false;

    while (std::getline(input, line)) {
        // Remove leading/trailing spaces
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];

            if (c == '<') {
                if (!currentTag.empty()) {
                    // Write current text content before starting a new tag
                    output << std::string(indentLevel * 2, ' ') << currentTag << '\n';
                    currentTag.clear();
                }

                // Check if it's a closing tag or self-closing tag
                if (line[i + 1] == '/') {
                    // Closing tag: decrease indent
                    indentLevel = std::max(0, indentLevel - 1);
                }

                insideTag = true;
                currentTag += c;
            } else if (c == '>') {
                currentTag += c;

                // Write tag and adjust indentation if it's an opening tag
                output << std::string(indentLevel * 2, ' ') << currentTag << '\n';
                if (line[i - 1] != '/' && line[i - 1] != '?') {
                    // Opening tag: increase indent
                    if (currentTag[1] != '/')
                        indentLevel++;
                }
                currentTag.clear();
                insideTag = false;
            } else {
                currentTag += c;
            }
        }

        // If there's any remaining content outside of tags, write it
        if (!currentTag.empty() && !insideTag) {
            output << std::string(indentLevel * 2, ' ') << currentTag << '\n';
            currentTag.clear();
        }
    }

    input.close();
    output.close();

    std::cout << "Formatted XML saved to: " << outputFile << std::endl;
}


int main() {
    std::string inputFile = "C:\\Users\\20109\\Desktop\\data_structura_project\\xmlinonestring.xml";   // Input XML file path
    std::string outputFile = "C:\\Users\\20109\\Desktop\\data_structura_project\\afterBeutifying.xml"; // Output XML file path

    prettifyXML(inputFile, outputFile);

    return 0;
}
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function to minify XML, remove comments, and trim spaces in text nodes
void minifyXML(string inputFile, string outputFile) {
    ifstream input(inputFile);
    ofstream output(outputFile);

    if (!input || !output) {
        cout << "Error opening input or output file!" << endl;
        return;
    }

    char c;
    bool insideTag = false;
    bool insideText = false;
    string textBuffer = "";

    while (input.get(c)) {
            if (insideText) {
                // Trim leading and trailing spaces and output text
                while (!textBuffer.empty() && isspace(textBuffer.front())) {
                    textBuffer.erase(textBuffer.begin());
                }
                while (!textBuffer.empty() && isspace(textBuffer.back())) {
                    textBuffer.pop_back();
                }
                if (!textBuffer.empty()) {
                    output << textBuffer;
                }
                textBuffer.clear();
                insideText = false;
            }

            insideTag = true;
            output << c;
        } else if (c == '>') {
            insideTag = false;
            output << c;
        } else if (insideTag) {
            output << c;
        } else if (isspace(c)) {
            if (!textBuffer.empty()) {
                textBuffer += ' '; // Add a single space to buffer
            }
        } else {
            insideText = true;
            textBuffer += c;
        }
    }

    input.close();
    output.close();
    cout << "Minified XML saved to " << outputFile << endl;
}


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
    bool insideComment = false;
    string textBuffer = "";

    while (input.get(c)) {
          if (insideComment) {
            // Look for the end of a comment (-->)
            if (c == '-' && input.peek() == '-') {
                input.get(c); // Consume second '-'
                if (input.peek() == '>') {
                    input.get(c); // Consume '>'
                    insideComment = false; // Exit comment mode
                }
            }
        } else if (c == '<') {
            if (input.peek() == '!') {
                // Detect start of a comment (<!--)
                input.get(c); // Consume '!'
                if (input.peek() == '-') {
                    input.get(c); // Consume '-'
                    if (input.peek() == '-') {
                        input.get(c); // Consume second '-'
                        insideComment = true; // Enter comment mode
                        continue; // Skip writing comments to output
                    }
               }
} 
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
// Handle any remaining text
    while (!textBuffer.empty() && isspace(textBuffer.front())) {
        textBuffer.erase(textBuffer.begin());
    }
    while (!textBuffer.empty() && isspace(textBuffer.back())) {
        textBuffer.pop_back();
    }
    if (!textBuffer.empty()) {
        output << textBuffer;
}
    input.close();
    output.close();
    cout << "Minified XML saved to " << outputFile << endl;
}
   
// Main function for command-line arguments
/*int main(int argc, char* argv[]) {
    if (argc < 6) {
        cout << "Usage: xml_editor mini -i input_file.xml -o output_file.xml" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputOption = argv[2];
    string inputFile = argv[3];
    string outputOption = argv[4];
    string outputFile = argv[5];

    if (mode == "mini" && inputOption == "-i" && outputOption == "-o") {
        minifyXML(inputFile, outputFile);
    } else {
        cout << "Invalid command! Use: xml_editor mini -i input_file.xml -o output_file.xml" << endl;
        return 1;
    }

return0;
} 
}*/


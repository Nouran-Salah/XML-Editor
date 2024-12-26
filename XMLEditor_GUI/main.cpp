#include "mainwindow.h"
#include <QFileDialog>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>  // For std::ifstream and std::ofstream
#include <string>   // For std::string
#include <cctype>   // For isspace
#include <iostream> // For std::cout (optional)



std::pair<std::string, int> FindMostFrequentPair(const std::string& XMLContent) // Find the most frequent pair in the XML File
{
    std::unordered_map<std::string, int> FrequencyOfPairs;
    for (unsigned int i = 0; i < XMLContent.size() - 1; i++)
    {
        std::string OnePair = XMLContent.substr(i, 2);
        FrequencyOfPairs[OnePair]++;
    }

    std::pair<std::string, int> MostFrequent{"", 0};
    for (auto i : FrequencyOfPairs)
    {
        if (i.second > MostFrequent.second)
        {
            MostFrequent = i;
        }
    }
    return MostFrequent;
}

std::string ApplyBPE(const std::string& XMLContent, std::unordered_map<std::string, std::string>& Dictionary)
{
    std::string CompressedXML = XMLContent;
    int symbol = 65;
    while (1)
    {
        std::pair<std::string, int> MostFrequentPair = FindMostFrequentPair(CompressedXML);
        if (MostFrequentPair.second < 2)
            break;
        std::string key(1, symbol++);
        std::string Pair = MostFrequentPair.first;
        Dictionary[key] = Pair;

        auto pos = 0;
        while ((pos = CompressedXML.find(Pair, pos)) != std::string::npos)
        {
            CompressedXML.replace(pos, Pair.size(), key);
        }
    }

    std::string NewCompressedXML = "";
    for (int i = 0; i < CompressedXML.size() - 1; i++)
    {
        if (CompressedXML[i] == CompressedXML[i + 1])
        {
            NewCompressedXML.push_back(symbol);
            std::string key(1, symbol);
            std::string str(2, CompressedXML[i]);
            Dictionary[key] = str;
            symbol++;
            i++;
        }
        else
        {
            NewCompressedXML.push_back(CompressedXML[i]);
        }
    }
    if (CompressedXML[CompressedXML.size() - 1] != CompressedXML[CompressedXML.size() - 2])
        NewCompressedXML.push_back(CompressedXML[CompressedXML.size() - 1]);

    return NewCompressedXML;
}

std::string Decompress(const std::string& NewCompressedXML, std::unordered_map<std::string, std::string>& Dictionary)
{
    std::string DecompressedXML = "";
    std::string encoded_value;
    for(unsigned int i = 0 ; i < NewCompressedXML.length(); i++)
    {
        std::string encoded_value(1,NewCompressedXML[i]);
        auto it = Dictionary.find(encoded_value);
        //cout << "Decoded value:" << decoded_value << endl;

        if (it != Dictionary.end())
        {
            std::string decoded_value = Dictionary[encoded_value];
            while (true) {
                bool fullyDecoded = true;
                std::string new_decoded_value;
                for (char c : decoded_value)
                {
                    std::string key(1, c); // Convert character to string
                    if (Dictionary.count(key))
                    {
                        new_decoded_value += Dictionary[key];
                        fullyDecoded = false; // Found an encoded value, need further decoding
                    }
                    else
                    {
                        new_decoded_value += c; // Append as-is if not encoded
                    }
                }
                decoded_value = new_decoded_value;

                if (fullyDecoded)
                {
                    break; // Exit loop when no further replacements are possible
                }
            }
            DecompressedXML += decoded_value; // Append the fully decoded value
        }

        else
        {
            DecompressedXML += NewCompressedXML[i];
        }
    }
    return DecompressedXML;
}


// Function to minify XML, remove comments, and trim spaces in text nodes
void minifyXML(std::string inputFile, std::string outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input || !output) {
        return;
    }

    char c;
    bool insideTag = false;
    bool insideComment = false;
    bool insideText = false;
    std::string textBuffer = "";

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
}


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



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

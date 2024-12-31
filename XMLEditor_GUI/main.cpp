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
#include <QStack>
#include "xml_tree.h"
#include <QString>
#include<iostream>
#include<string>
#include<unordered_map>

/*
std::pair<std::string, int> FindMostFrequentPair(const std::string& XMLContent) {
    std::unordered_map<std::string, int> FrequencyOfPairs;
    for (unsigned int i = 0; i < XMLContent.size() - 1; i++) {
        std::string OnePair = XMLContent.substr(i, 2);
        FrequencyOfPairs[OnePair]++;
    }

    std::pair<std::string, int> MostFrequent{"", 0};
    for (auto i : FrequencyOfPairs) {
        if (i.second > MostFrequent.second) {
            MostFrequent = i;
        }
    }
    return MostFrequent;
}

std::string ApplyBPE(const std::string& filePath, std::unordered_map<std::string, std::string>& Dictionary) {
    // Read content from file
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::ostringstream buffer;
    buffer << inputFile.rdbuf();
    std::string XMLContent = buffer.str();

    std::string CompressedXML = XMLContent;
    int symbol = 128;
    while (1) {
        std::pair<std::string, int> MostFrequentPair = FindMostFrequentPair(CompressedXML);
        if (MostFrequentPair.second < 2)
            break;
        std::string key(1, symbol++);
        std::string Pair = MostFrequentPair.first;
        Dictionary[key] = Pair;

        auto pos = 0;
        while ((pos = CompressedXML.find(Pair, pos)) != std::string::npos) {
            CompressedXML.replace(pos, Pair.size(), key);
        }
    }

    std::string NewCompressedXML = "";
    for (int i = 0; i < CompressedXML.size() - 1; i++) {
        if (CompressedXML[i] == CompressedXML[i + 1]) {
            NewCompressedXML.push_back(symbol);
            std::string key(1, symbol);
            std::string str(2, CompressedXML[i]);
            Dictionary[key] = str;
            symbol++;
            i++;
        } else {
            NewCompressedXML.push_back(CompressedXML[i]);
        }
    }
    if (CompressedXML[CompressedXML.size() - 1] != CompressedXML[CompressedXML.size() - 2])
        NewCompressedXML.push_back(CompressedXML[CompressedXML.size() - 1]);

    return NewCompressedXML;
}

std::string Decompress(const std::string& filePath, std::unordered_map<std::string, std::string>& Dictionary) {
    // Read content from file
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::ostringstream buffer;
    buffer << inputFile.rdbuf();
    std::string NewCompressedXML = buffer.str();

    std::string DecompressedXML = "";
    for (unsigned int i = 0; i < NewCompressedXML.length(); i++) {
        std::string encoded_value(1, NewCompressedXML[i]);
        auto it = Dictionary.find(encoded_value);

        if (it != Dictionary.end()) {
            std::string decoded_value = Dictionary[encoded_value];
            while (true) {
                bool fullyDecoded = true;
                std::string new_decoded_value;
                for (char c : decoded_value) {
                    std::string key(1, c); // Convert character to string
                    if (Dictionary.count(key)) {
                        new_decoded_value += Dictionary[key];
                        fullyDecoded = false; // Found an encoded value, need further decoding
                    } else {
                        new_decoded_value += c; // Append as-is if not encoded
                    }
                }
                decoded_value = new_decoded_value;

                if (fullyDecoded) {
                    break; // Exit loop when no further replacements are possible
                }
            }
            DecompressedXML += decoded_value; // Append the fully decoded value
        } else {
            DecompressedXML += NewCompressedXML[i];
        }
    }
    return DecompressedXML;
}*/



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
    int symbol = 128;
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



bool is_valid(QString xml) {
    QStack<QString> opening_tags;
    int i = 0;

    while (i < xml.size()) {
        bool closing_tag = false;
        QString tag;
        QString attributes;

        // Look for the start of a tag
        if (xml[i] == '<') {
            i++; // Move past '<'

            // Check if it's a closing tag
            if (xml[i] == '/') {
                closing_tag = true;
                i++; // Move past '/'
            }

            // Extract the tag name
            while (i < xml.size() && xml[i] != ' ' && xml[i] != '>' && xml[i] != '/') {
                tag += xml[i++];
            }

            // Skip over attributes if any
            while (i < xml.size() && (xml[i] == ' ' || xml[i] == '=' || xml[i] == '"' || xml[i] == '/')) {
                if (xml[i] == ' ' || xml[i] == '=') {
                    attributes += xml[i]; // You can store the attribute names here if needed
                }
                i++;
            }

            // If it's a closing tag, check if it matches the last opened tag
            if (closing_tag) {
                if (opening_tags.empty()) {
                    return false; // No opening tag to match
                }

                if (opening_tags.top() == tag) {
                    opening_tags.pop(); // Valid closing tag, pop from stack
                } else {
                    return false; // Mismatched tag
                }
            } else {
                opening_tags.push(tag); // Push opening tag onto stack
            }

            // If it's a self-closing tag, don't push it onto the stack
            if (xml[i-1] == '/') {
                i++; // Skip past the self-closing '/'
                // Do not push to stack for self-closing tags
            }

        } else {
            i++; // If it's not a tag, just move to the next character
        }
    }

    return opening_tags.empty(); // Valid if no unclosed tags remain
}

QString makeXML_open_tag(QString name, QString attributes = "") {
    return "<" + name + attributes + ">";
}

QString makeXML_close_tag(QString name) {
    return "</" + name + ">";
}

QString corrected_xml(QString xml) {
    QStack<QString> opening_tags;
    QString fixed_xml;
    int i = 0;

    while (i < xml.size()) {
        bool closing_tag = false;
        QString tag;
        QString attributes;

        if (xml[i] == '<') {
            i++; // Move past '<'

            if (xml[i] == '/') {
                closing_tag = true;
                i++; // Move past '/'
            }

            // Extract the tag name
            while (i < xml.size() && xml[i] != ' ' && xml[i] != '>' && xml[i] != '/') {
                tag += xml[i++];
            }

            // Skip over attributes
            while (i < xml.size() && (xml[i] == ' ' || xml[i] == '=' || xml[i] == '"' || xml[i] == '/')) {
                if (xml[i] == ' ' || xml[i] == '=') {
                    attributes += xml[i]; // Store the attributes, if needed
                }
                i++;
            }

            if (closing_tag) {
                if (opening_tags.empty()) {
                    // If there's no matching opening tag, fix it by inserting an opening tag
                    fixed_xml += makeXML_open_tag(tag) + makeXML_close_tag(tag);
                } else if (opening_tags.top() == tag) {
                    // If tags match, close it properly
                    fixed_xml += makeXML_close_tag(tag);
                    opening_tags.pop();
                } else {
                    // If mismatched, add an opening tag
                    fixed_xml += makeXML_open_tag(tag) + makeXML_close_tag(tag);
                }
            } else {
                // For opening tags, push onto the stack and append to the fixed XML
                opening_tags.push(tag);
                fixed_xml += makeXML_open_tag(tag, attributes);
            }

            // Handle self-closing tags (e.g., <tag/>)
            if (xml[i-1] == '/') {
                fixed_xml += makeXML_close_tag(tag); // Add the closing tag to the fixed XML
                opening_tags.pop(); // No need to push to stack for self-closing tags
            }

        } else {
            fixed_xml += xml[i]; // For non-tag characters, just add them to the result
        }

        i++;
    }

    // Close any remaining tags in the stack
    while (!opening_tags.empty()) {
        QString tag = opening_tags.top();
        fixed_xml += makeXML_close_tag(tag);
        opening_tags.pop();
    }

    return fixed_xml;
}


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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

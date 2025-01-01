#include <iostream>
#include <QString>
#include <QStack>

using namespace std;

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

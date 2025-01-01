#ifndef VALIDATE_XML_H
#define VALIDATE_XML_H

#include <iostream>
#include <QString>
#include <QStack>

using namespace std;

// Function to check if an XML string is valid
bool is_valid(QString xml);

// Function to generate an opening XML tag
QString makeXML_open_tag(QString name, QString attributes = "");

// Function to generate a closing XML tag
QString makeXML_close_tag(QString name);

// Function to correct an invalid XML string by fixing mismatched or missing tags
QString corrected_xml(QString xml);

#endif // VALIDATE_XML_H

/*#include <iostream>
#include <string>
#include<unordered_map>
#include "xml_tree.h"
#include "XML_to_JSON.h"
#include "XML_minifying.h"
#include "file_processing.h"
#include "validate_fix.h"
#include "Prettifying.h"
#include "compression_decompression.h"
using namespace std;
int main(int argc ,char*argv[]) {

    string XML = read_file("input_file.xml");
    string Jsonfile = "output_file.json";
    string OutputFile = "output_file.xml";
    string output;
    string command;
    string corrected, compressedXML;
    string compressedFile = "outputfile.comp";
    unordered_map<string, string> dictionary;
   /* while(true)
    {
        cout << "Enter Command (or 'exit' to quit): ";*/

      // getline(cin, command);

        // Allow the user to exit the loop
      /*  if (command == "exit" || command == "quit") {
            break;
        }*/

     /*   istringstream iss(command);
        string mode, ProgramName, inputFile, outputFile;
        iss >> ProgramName >> mode >> inputFile >> outputFile;*/
       /* string mode = argv[2];
    if (mode == "") {
        cout << "Mode is empty!" << endl;
    }
    else if (mode == "verify") {

        if (is_valid(XML)) {
            cout << "\nThe XML is valid.\n";
        }
        else {
            cout << "\nThe XML is not valid.\n";
            
        }
        write_file(corrected_xml(XML), OutputFile);
        cout << "Verified XML saved to OutputFile" << endl;

       
    }
    else if (mode == "mini") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", OutputFile);
        cout << "Minified XML saved to OutputFile" << endl;

    }
    else if (mode == "format") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        prettifyXML("temp2.xml", OutputFile);
        cout << "Prettifed XML saved to OutputFile" << endl;


    }
    else if (mode == "json") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        write_file(convertXMLToJSON(read_file("temp2.xml")), Jsonfile);
        cout << " Json file saved to OutputFile" << endl;

    }
    else if (mode == "compress") {
      //  write_file(, "temp.xml");
         compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
         write_file(compressedXML, compressedFile);


    }
    else if (mode == "decompress") {
        compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
        string decompressedXML = Decompress(compressedXML, dictionary);
        write_file(decompressedXML, OutputFile);
    }
    else if (mode == "search") {

    }

    else {
        cout << "Unknown command or invalid format!" <<endl;
    }

    cout << "Exiting program..." << endl;
    return 0;
}*/


#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "visualization.h"
#include "xml_tree.h"
#include "XML_to_JSON.h"
#include "XML_minifying.h"
#include "file_processing.h"
#include "validate_fix.h"
#include "Prettifying.h"
#include "compression_decompression.h"

using namespace std;

int main(int argc, char* argv[]) {


    if (argc < 3) {
        cerr << "Error: Not enough arguments provided. Usage: <program> <mode> <input_file>" << endl;
        return 1;
    }

    string mode = argv[1];
    string XML = read_file("input_file.xml");
    if (XML.empty()) {
        cerr << "Error: Failed to read input_file.xml or the file is empty." << endl;
        return 1;
    }

    string Jsonfile = "output_file.json";
    string OutputFile = "output_file.xml";
    string compressedFile = "outputfile.comp";
    unordered_map<string, string> dictionary;
     

    if (mode == "verify") {
        if (is_valid(XML)) {
            cout << "\nThe XML is valid.\n";
        } else {
            cout << "\nThe XML is not valid.\n";
        }
        write_file(corrected_xml(XML), OutputFile);
        cout << "Verified XML saved to OutputFile" << endl;

    } else if (mode == "mini") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", OutputFile);
        cout << "Minified XML saved to OutputFile" << endl;

    } else if (mode == "format") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        prettifyXML("temp2.xml", OutputFile);
        cout << "Prettifed XML saved to OutputFile" << endl;

    } else if (mode == "json") {
        write_file(corrected_xml(XML), "temp.xml");
        minifyXML("temp.xml", "temp2.xml");
        write_file(convertXMLToJSON(read_file("temp2.xml")), Jsonfile);
        cout << "Json file saved to OutputFile" << endl;

    } else if (mode == "compress") {
        string compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
        write_file(compressedXML, compressedFile);

    } else if (mode == "decompress") {
        string compressedXML = ApplyBPE(corrected_xml(XML), dictionary);
        string decompressedXML = Decompress(compressedXML, dictionary);
        write_file(decompressedXML, OutputFile);

    } else if (mode == "search") {
        // Implement search functionality
    } else {
        cout << "Unknown command or invalid format!" << endl;
    }

    cout << "Exiting program..." << endl;
    return 0;
}

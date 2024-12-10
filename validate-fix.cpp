#include <string>
#include <stack>

using namespace std;

bool is_valid(string xml) {

    stack<string> opening_tags;

    for(int i=0;i < xml.size();i++) {

        bool closing_tag = false;
        string tag;

        if(xml[i] == '<') { // if thats a tag

            for(i++;xml[i] != '>' && xml[i] != ' ';i++) { // extract tag name

                if(xml[i] == '/') { // if closing tag
                    closing_tag = true;
                }else {

                    tag += xml[i];
                }
            }
        }

        if(closing_tag) {
            if(opening_tags.empty()) return false;

            if(opening_tags.top() == tag) {
                opening_tags.pop();
            }else {
                return false;
            }
        }else if(tag.size()) opening_tags.push(tag);
    }

    return opening_tags.empty();
}

string makeXML_open_tag(string name, string attributes="") {
    return "<"+name+attributes+">";
}

string makeXML_close_tag(string name) {
    return "</"+name+">";
}

string corrected_xml(string xml) { 

    stack<string> opening_tags;
    string fixed_xml;

    for(int i=0;i < xml.size();i++) {

        bool closing_tag = false;
        string tag;
        string attributes;

        if(xml[i] == '<') { // if thats a tag

            for(i++;xml[i] != '>';i++) { // extract tag name

                if(xml[i] == '/') { // if closing tag
                    closing_tag = true;
                }else {
                    
                    if(attributes.size() || xml[i] == ' ') {
                        attributes += xml[i];
                    }else {
                        tag += xml[i];
                    }
                }
            }
        }else {
            fixed_xml += xml[i];
        }

        if(closing_tag) {
            if(opening_tags.empty()) {
                fixed_xml += makeXML_open_tag(tag) + makeXML_close_tag(tag);

            }else if(opening_tags.top() == tag) {

                fixed_xml += makeXML_close_tag(tag);
                opening_tags.pop();

            }else {
                fixed_xml += makeXML_open_tag(tag) + makeXML_close_tag(tag);
            }
        }else if(tag.size()) {
            opening_tags.push(tag);
            fixed_xml += makeXML_open_tag(tag, attributes);
        }
    }

    while(!opening_tags.empty()) {

        string tag = opening_tags.top();
        fixed_xml += makeXML_close_tag(tag);
        opening_tags.pop();
    }

    return fixed_xml;
}
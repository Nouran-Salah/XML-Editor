#include <string>
#include <stack>


bool is_valid(std::string xml) {

    std::stack<std::string> opening_tags;

    for(int i=0;i < xml.size();i++) {

        bool closing_tag = false;
        std::string tag;

        if(xml[i] == '<') { // if thats a tag
            if(xml[i+1] == '?' || xml[i+1] == '!') continue;
                
            closing_tag = xml[i+1] == '/';

            for(i += 1 + closing_tag;xml[i] != '>' && !isspace(xml[i]);i++) { // extract tag name

                tag += xml[i];                
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

std::string makeXML_open_tag(std::string name, std::string attributes="") {
    return "<"+name+attributes+">";
}

std::string makeXML_close_tag(std::string name) {
    return "</"+name+">";
}

std::string corrected_xml(std::string xml) { 

    std::stack<std::string> opening_tags;
    std::string fixed_xml;

    for(int i=0;i < xml.size();i++) {

        bool closing_tag = false;
        std::string tag;
        std::string attributes;

        if(xml[i] == '<') { // if thats a tag

            if(xml[i+1] == '?' || xml[i+1] == '!') {
                fixed_xml += xml[i];
                continue;
            }

            closing_tag = xml[i+1] == '/';
            
            for(i += 1 + closing_tag;xml[i] != '>';i++) { // extract tag name

                if(attributes.size() || xml[i] == ' ') {
                    attributes += xml[i];
                }else {
                    tag += xml[i];
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

        std::string tag = opening_tags.top();
        fixed_xml += makeXML_close_tag(tag);
        opening_tags.pop();
    }

    return fixed_xml;
}
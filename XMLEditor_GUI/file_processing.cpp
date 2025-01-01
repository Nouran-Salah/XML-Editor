#include <string>
#include <sstream>
#include <fstream>

std::string read_file(std::string file_name) {

    std::ifstream file(file_name);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();

}

void write_file(std::string content, std::string file_name) {

    std::ofstream file(file_name);
    file << content;
    file.close();
}

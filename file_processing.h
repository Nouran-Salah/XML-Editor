#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

// Function to read the contents of a file into a string
std::string read_file(std::string file_name);

// Function to write a string to a file
void write_file(std::string content, std::string file_name);

#endif // FILE_UTILS_H

#ifndef XML_COMPRESSION_H
#define XML_COMPRESSION_H

#include <string>
#include <unordered_map>

// Function to find the most frequent pair in the XML content
std::pair<std::string, int> FindMostFrequentPair(const std::string& XMLContent);

// Function to apply Byte Pair Encoding (BPE) to compress the XML content
std::string ApplyBPE(const std::string& XMLContent, std::unordered_map<std::string, std::string>& Dictionary);

// Function to decompress the compressed XML content using the dictionary
std::string Decompress(const std::string& NewCompressedXML, std::unordered_map<std::string, std::string>& Dictionary);

#endif // XML_COMPRESSION_H
#pragma once

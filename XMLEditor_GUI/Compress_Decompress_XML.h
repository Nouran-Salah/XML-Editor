#ifndef COMPRESS_DECOMPRESS_XML_H
#define COMPRESS_DECOMPRESS_XML_H

std::pair<std::string, int> FindMostFrequentPair(const std::string& XMLContent); // Find the most frequent pair in the XML File
//std::pair<std::string, int> FindMostFrequentPair(const std::string& XMLContent);
//std::string ApplyBPE(const std::string& filePath, std::unordered_map<std::string, std::string>& Dictionary);
//std::string Decompress(const std::string& filePath, std::unordered_map<std::string, std::string>& Dictionary);
std::string ApplyBPE(const std::string& XMLContent, std::unordered_map<std::string, std::string>& Dictionary);
std::string Decompress(const std::string& NewCompressedXML, std::unordered_map<std::string, std::string>& Dictionary);


#endif // COMPRESS_DECOMPRESS_XML_H

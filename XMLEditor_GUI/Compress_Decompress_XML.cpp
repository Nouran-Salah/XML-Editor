#include <iostream>
#include <unordered_map>

using namespace std;

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

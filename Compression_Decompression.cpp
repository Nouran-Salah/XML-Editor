#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

pair<string, int> FindMostFrequentPair(const string& XMLContent) // Find the most frequent pair in the XML File
{
	unordered_map<string, int> FrequencyOfPairs;
	for (unsigned int i = 0; i < XMLContent.size() - 1; i++)
	{
		string OnePair = XMLContent.substr(i, 2);
		FrequencyOfPairs[OnePair]++;
	}

	pair<string, int> MostFrequent{"", 0};
	for (auto i : FrequencyOfPairs)
	{
		if (i.second > MostFrequent.second)
		{
			MostFrequent = i;
		}
	}
	return MostFrequent;
}

string ApplyBPE(const string& XMLContent, unordered_map<string, string>& Dictionary)
{
	string CompressedXML = XMLContent;
	int symbol = 128;
	while (1)
	{
		pair<string, int> MostFrequentPair = FindMostFrequentPair(CompressedXML);
		if (MostFrequentPair.second < 2)
			break;
		string key(1, symbol++);
		string Pair = MostFrequentPair.first;
		Dictionary[key] = Pair;

		auto pos = 0;
		while ((pos = CompressedXML.find(Pair, pos)) != string::npos)
		{
			CompressedXML.replace(pos, Pair.size(), key);
		}
	}

	string NewCompressedXML = "";
	for (int i = 0; i < CompressedXML.size() - 1; i++)
	{
		if (CompressedXML[i] == CompressedXML[i + 1])
		{
			NewCompressedXML.push_back(symbol);
			string key(1, symbol);
			string str(2, CompressedXML[i]);
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

	cout << NewCompressedXML.size() << "\nDictionary:\n";

	for (auto i : Dictionary)
	{
		cout << i.first << " " << i.second << "\n";
	}
	cout << "Compressed: " << NewCompressedXML << endl;
	return NewCompressedXML;
}

string Decompress(const string& NewCompressedXML, unordered_map<string, string>& Dictionary)
{
	string DecompressedXML = "";
	string encoded_value;
	for(unsigned int i = 0 ; i < NewCompressedXML.length(); i++)
	{
		string encoded_value(1,NewCompressedXML[i]);
		auto it = Dictionary.find(encoded_value);
		//cout << "Decoded value:" << decoded_value << endl;

		if (it != Dictionary.end())
		{
		    string decoded_value = Dictionary[encoded_value];
		    while (true) {
		        bool fullyDecoded = true;
		        string new_decoded_value;
		        for (char c : decoded_value)
		        {
		            string key(1, c); // Convert character to string
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

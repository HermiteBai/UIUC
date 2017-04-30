/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/** 
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */ 
AnagramDict::AnagramDict(const string& filename)
{
    /* Your code goes here! */
	vector<string> words;
	ifstream wordsFile(filename);
	string word;
	if (wordsFile.is_open())
	{
		while (getline(wordsFile, word))
		{
			words.push_back(word);
		}
	}

	for (auto itr : words)
	{
		auto lookup = dict.find(itr);
		if (lookup == dict.end())
		{
			vector<char> chars;
			for (unsigned long i = 0; i < itr.length(); i++)
			{
				chars.push_back(itr.at(i));
			}
			std::sort(chars.begin(), chars.end());
			dict.insert(std::pair<string, vector<char>>(itr, chars));
		}

	}

}

/** 
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */ 
AnagramDict::AnagramDict(const vector< string >& words)
{
    /* Your code goes here! */
	for (auto word : words)
	{
		auto lookup = dict.find(word);
		if (lookup == dict.end())
		{
			vector<char> chars;
			for (unsigned long i = 0; i < word.length(); i++)
			{
				chars.push_back(word.at(i));
			}
			std::sort(chars.begin(), chars.end());
			dict.insert(std::pair<string, vector<char>>(word, chars));
		}
	}			
}

/**
 * @param word The word to find anagrams of. 
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the 
 * word list.
 */
vector< string > AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
	vector<string> anagrams;
	anagrams = helper_get_anagrams(word);
	if (anagrams.size() > 1)
		return anagrams;
	else
		return vector<string>();
}       

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector< vector< string > > AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
	vector<vector<string>> all_anagrams;
	for (auto itr : dict)
	{
		string temp = itr.first;
		vector<string> tempVector = helper_get_anagrams(temp);
		if (tempVector.size() > 1)
			all_anagrams.push_back(tempVector);
	}
    return all_anagrams;
}

//helper function
vector<string> AnagramDict::helper_get_anagrams(const string& word) const
{
	vector<string> out;
	auto lookup = dict.find(word);
	if (lookup != dict.end())
	{
		vector<char> chars = lookup->second;
		for (auto pair : dict)
		{
			if (pair.second == chars)
				 out.push_back(pair.first);
		}
	}
	return out;
}

/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str) 
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), 
                        std::back_inserter(ret),
                        std::ptr_fun< int, int >(&std::ispunct));
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

/**
 * Initializes file_word_maps.
 * @param filenames The vector of names of the files that will be used
 */
void CommonWords::init_file_word_maps(const vector<string>& filenames) 
{
    // make the length of file_word_maps the same as the length of filenames
    file_word_maps.resize(filenames.size());

    // go through all files
    for(size_t i = 0; i < filenames.size(); i++) {
        // get the corresponding vector of words that represents the current file
        vector<string> words = file_to_vector(filenames[i]);
        /* Your code goes here! */
		for (auto word : words)
		{
			auto lookup = file_word_maps[i].find(word);
			if (lookup != file_word_maps[i].end())
				file_word_maps[i][word]++;
			else
				file_word_maps[i][word] = 1;
		}
    }
}

/**
 * Initializes common.
 */
void CommonWords::init_common() 
{
    /* Your code goes here! */
	for (auto wordMap : file_word_maps)
	{
		for (auto itr = wordMap.begin(); itr != wordMap.end(); itr++)
		{
			auto lookup = common.find(itr->first);
			if (lookup != common.end())
				common[itr->first]++;
			else
				common[itr->first] = 1;
		}
	}
}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector< string > CommonWords::get_common_words(unsigned int n) const
{
    /* Your code goes here! */
    vector<string> out;
	///*
	map<string, unsigned long> temp;
	for (auto wordMaps : file_word_maps)
	{ 
		for (auto itr : wordMaps)
		{
			
			if (itr.second >= n)
			{
		//		out.push_back(itr.first);
				auto lookup = temp.find(itr.first);
				if (lookup != temp.end())	
					temp[itr.first]++;
				else
					temp[itr.first] = 1;
			}
		}
	}
	///*
	for (auto itr : temp)
	{
		if (itr.second == file_word_maps.size())
			out.push_back(itr.first);
	}
	//*/
	//*/
	/*
	for (auto word : common)
	{
		if (word.second == file_word_maps.size())
		{
			unsigned long count = 0;
			for (auto wordMap : file_word_maps)
			{
				auto lookup = wordMap.find(word.first);
				if (word.second >= n)
					count++;
			}
			if (count == file_word_maps.size())
				out.push_back(word.first);
		}
	}
	*/
    return out;
}

	

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector< string > CommonWords::file_to_vector(const string& filename) const {
    ifstream words(filename);
    vector<string> out;

    if(words.is_open()) {
        std::istream_iterator<std::string> word_iter(words);
        while(!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}

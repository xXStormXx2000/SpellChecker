#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>


int word_distance(const std::string&, const std::string&);
bool rough_filter(const std::string&, const std::string&);
bool rough_char_filter(const std::string&, const std::string&, int);

std::map<int, std::vector<std::string>> find_suggestions(std::string);
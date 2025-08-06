#include <iostream>
#include "Spell_Checker.h"
#include <set>


int main() {
	std::string word;
	std::cin >> word;
	std::map<int, std::vector<std::string>> ws = find_suggestions(word);
	if (ws.size() == 1 && ws.find(0) != ws.end()) return 0;
	if (ws.size() == 0) std::cout << "Can't find any similar words\n";
	for (auto i: ws) 
	{
		std::cout << "Suggestions " << i.first << ": ";
		for(auto j: i.second) std::cout << j << ' ';
		std::cout << '\n';
	}
	return 0;
}
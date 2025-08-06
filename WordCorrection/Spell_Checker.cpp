#include "Spell_Checker.h"

int word_distance(const std::string& word_1, const std::string& word_2) 
{
	if (word_1.size() == 0 || word_2.size() == 0) return word_1.size() + word_2.size();
	int y_size = word_1.size() + 1;
	int x_size = word_2.size() + 1;
	std::vector<std::vector<int>> distance_map(y_size, std::vector<int>(x_size, INT32_MAX));
	distance_map[0][0] = 0;
	for (int y = 0; y < y_size; y++) 
	{
		for (int x = 0; x < x_size; x++) 
		{
			int num = distance_map[y][x];
			if (y + 1 < y_size && 
				num + 1 < distance_map[y + 1][x]) distance_map[y + 1][x] = num + 1;

			if (x + 1 < x_size &&
				num + 1 < distance_map[y][x + 1]) distance_map[y][x + 1] = num + 1;

			if (y + 1 < y_size &&
				x + 1 < x_size &&
				num < distance_map[y + 1][x + 1]) 
			{
				distance_map[y + 1][x + 1] = num;
				if (word_1[y] != word_2[x])
					distance_map[y + 1][x + 1]++;
			}
		}
	}
	return distance_map.back().back();
}

bool rough_size_filter(const std::string& word_1, const std::string& word_2) 
{
	if (word_1.size() == word_2.size()) return true;
	int min = std::min(word_1.size(), word_2.size());
	int max = std::max(word_1.size(), word_2.size());
	return max <= min*1.5;
}

bool rough_char_filter(const std::string& word_1, const std::string& word_2, int max_dif)
{
	std::vector<short> char_list(256, 0);
	for (char c : word_1) char_list[c]--;
	for (char c : word_2) char_list[c]++;
	int dif = 0;
	for (short num : char_list) dif += abs(num);
	return dif <= max_dif;
}

bool rough_filter(const std::string& word_1, const std::string& word_2)
{
	const int max_dif = (word_1.size() + word_2.size())>>2;
	return (rough_size_filter(word_1, word_2) && rough_char_filter(word_1, word_2, max_dif));
}

std::map<int, std::vector<std::string>> find_suggestions(std::string word_1)
{
	for (char& c : word_1) if (c < 'a') c += 'a' - 'A';
	std::string word_2 = "";
	std::map<int, std::vector<std::string>> word_suggestions;
	int bias = 0;
	auto healper = [&word_suggestions, &word_1, &word_2, &bias](std::string file_path) -> bool{
		std::ifstream file(file_path);
		while (file >> word_2)
		{
			if (word_1 == word_2)
			{
				word_suggestions = { { 0, { word_1 }} };
				return true;
			}
			if (!rough_filter(word_1, word_2)) continue;
			int distance = word_distance(word_1, word_2);
			if (distance > word_1.size() >> 1) continue;
			distance += distance + bias;
			word_suggestions[distance].push_back(word_2);
		}

		file.close();
		return false;
	};
	if(healper("google_10000_english.txt")) return word_suggestions;
	bias++;
	if(healper("words_alpha.txt")) return word_suggestions;
	return word_suggestions;
}
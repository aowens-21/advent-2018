/*
	Day 2 - Advent of Code 2018
	
	Part 1: Given a list of strings, compute the number of strings with at least one
	character that appears exactly two times and compute the number with at least one
	character that appears exactly three times. Multiply these together to get a simple
	checksum.

	Part 2: Given a list of strings, find the two strings that differ by only 1 character and
	return the characters they have in common.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

int compute_difference(string s1, string s2)
{
	int diff = 0;
	int s1_length = s1.size();
	int s2_length = s2.size();
	int min_length = min(s1_length, s2_length);
	
	if (s1_length != s2_length)
		diff += abs(s2_length - s1_length);

	for (int i = 0; i < min_length; ++i)
	{
		if (s1[i] != s2[i])
			diff++;
	}	

	return diff;

}

string get_common_characters_from_correct_ids(string in_file_name)
{
	ifstream box_ids_file { in_file_name };
	vector<string> ids;
	string line;

	while (getline(box_ids_file, line))
		ids.push_back(line);
	
	// once the list is sorted, we know that the two correct ids will be adjacent
	sort(ids.begin(), ids.end());

	for (int i = 0; i < ids.size() - 1; ++i)
	{
		if (compute_difference(ids[i], ids[i+1]) == 1)
		{
			string s1 = ids[i];
			string s2 = ids[i+1];
			int diff_char_index = 0;
			while (s1[diff_char_index] == s2[diff_char_index])
				diff_char_index++;

			return s1.substr(0, diff_char_index) + s1.substr(diff_char_index+1);
		}
	}
}

int get_checksum(string in_file_name)
{
	ifstream box_ids_file { in_file_name };
	string line;
	int num_found_two = 0;
	int num_found_three = 0;
	bool found_two;
	bool found_three;
	
	while (getline(box_ids_file, line))
	{
		map<char, int> char_counts;
		
		for (auto it : line)
		{
			if (char_counts.count(it))
				char_counts[it]++;
			else
				char_counts.emplace(it, 1);
		}

		for (auto it = char_counts.begin(); it != char_counts.end() && (!found_two || !found_three); ++it)
		{
			if (it->second == 2)
				found_two = true;
			else if (it->second == 3)
				found_three = true;
		}

		if (found_two)
			num_found_two++;
		if (found_three)
			num_found_three++;
		
		found_two = false;
		found_three = false;	
	}

	return num_found_two * num_found_three;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage: ./a.out input.txt" << endl;
		return -1;
	}

	cout << "Checksum: " << get_checksum(argv[1]) << endl;
	cout << "Correct Ids Common Chars: " << get_common_characters_from_correct_ids(argv[1]) << endl;
	return 0;
}

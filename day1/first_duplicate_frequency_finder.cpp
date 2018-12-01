/*
	Day 1 - Advent of Code 2018
	
	Given a list of frequency changes of the form (+x) or (-x) and a starting frequency
	of 0, go through the list of changes keeping track of the current one and return the first
	frequency that is reached twice
*/

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

int parse_frequency_change(string change)
{
	char sign = change[0];
	int value = stoi(change.substr(1));
	
	if (sign == '+')
		return value;
	else
		return -value;	
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage: ./a.out input.txt" << endl;
		return -1;
	}
	
	ifstream freq_change_file {argv[1]};

	string line;
	int current_frequency = 0;
	
	unordered_set<int> visited_frequencies { 0 };
	vector<int> changes;
		
	while (getline(freq_change_file, line))
	{
		int change = parse_frequency_change(line);

		changes.push_back(change);
		current_frequency += change;

		if (visited_frequencies.count(current_frequency) == 0)
		{
			visited_frequencies.emplace(current_frequency);
		}
		else
		{
			cout << "First repeated frequency: " << current_frequency << endl;
			return 0;
		}					
	}
	
	bool found_repeated_frequency = false;
	int change_index = 0;
	int length_of_changes = changes.size();
	
	while (!found_repeated_frequency)
	{	
		int change = changes[change_index];
		current_frequency += change; 

		if (visited_frequencies.count(current_frequency) == 0)
		{
			visited_frequencies.emplace(current_frequency);
		}
		else
		{
			found_repeated_frequency = true;
		}

		if (change_index + 1 < length_of_changes)
			change_index++;
		else
			change_index = 0;	
	}

	cout << "First repeated frequency: " << current_frequency << endl;
	return 0;
}

/*
	Day 1 - Advent of Code 2018
	
	Part 1: Given a list of frequency changes of the form (+x) or (-x) and a starting frequency,
	go through the list of changes and compute the final frequency.
	
	Part 2: Given a list of frequency changes of the form (+x) or (-x) and a starting frequency,
	go through the list of changes keeping track of the current one and return the first
	frequency that is reached twice.
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

int compute_final_frequency(int start_freq, string input_file_name)
{
	ifstream freq_change_file { input_file_name };

	string line;
	int current_frequency = start_freq;
	
	while (getline(freq_change_file, line))
	{
		current_frequency += parse_frequency_change(line);	
	}
	
	freq_change_file.close();

	return current_frequency;	
}

int compute_first_repeated_frequency(int start_freq, string input_file_name)
{
	ifstream freq_change_file { input_file_name };
	string line;
	int current_frequency = start_freq;
	
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
			return current_frequency;
		}					
	}
	
	freq_change_file.close();
	
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
	
	return current_frequency;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage: ./a.out input.txt" << endl;
		return -1;
	}

	cout << "Final freqency after 1 iteration: " << compute_final_frequency(0, argv[1]) << endl;
	cout << "First repeated frequency: " << compute_first_repeated_frequency(0, argv[1]) << endl;

	return 0;	
}

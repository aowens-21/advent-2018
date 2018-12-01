/*
	Day 1 - Advent of Code 2018
	
	Given a list of frequency changes of the form (+x) or (-x) and a starting frequency
	of 0, calculate the resulting frequency after all changes have been applied.
*/

#include <iostream>
#include <fstream>
#include <string>

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
	
	while (getline(freq_change_file, line))
	{
		current_frequency += parse_frequency_change(line);	
	}

	cout << "Final Frequency: " << current_frequency << endl;

	return 0;	
}

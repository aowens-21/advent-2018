#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Claim {
public:
	Claim(int id, int l_off, int t_off, int w, int h) :
		id(id), left_offset(l_off), top_offset(t_off), width(w), height(h) {};
	const int id;
	const int left_offset;
	const int top_offset;
	const int width;
	const int height;
};

Claim parse_line_into_claim(const string& line)
{
	int atsign_pos, comma_pos, colon_pos, x_pos;

	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == '@')
			atsign_pos = i;
		else if (line[i] == ',')
			comma_pos = i;
		else if (line[i] == ':')
			colon_pos = i;
		else if (line[i] == 'x')
			x_pos = i;
	}

	// extract from correct spot in string
	int id = stoi(line.substr(1, atsign_pos - 2));
	int l_off = stoi(line.substr(atsign_pos + 2, comma_pos - (atsign_pos + 2)));
	int t_off = stoi(line.substr(comma_pos + 1, colon_pos - (comma_pos + 1)));
	int w = stoi(line.substr(colon_pos + 2, x_pos - (colon_pos + 2)));
	int h = stoi(line.substr(x_pos + 1));

	return Claim { id, l_off, t_off, w, h };
}

int compute_overlapping_claims(const vector<Claim>& claims)
{
	int fabric[1000][1000] {};
	int overlapping = 0;
	
	for (auto claim : claims)
	{
		for (int row = claim.left_offset; row < claim.left_offset + claim.width; ++row)
		{
			for (int col = claim.top_offset; col < claim.top_offset + claim.height; ++col)
			{
				fabric[row][col]++;
			}
		}
	}

	for (int row = 0; row < 1000; ++row)
	{
		for (int col = 0; col < 1000; ++col)
		{
			if (fabric[row][col] > 1)
				overlapping++;
		}
	}
	
	return overlapping;
}

int find_non_overlapping_claim(const vector<Claim>& claims)
{
	int fabric[1000][1000] {};
	int overlapping = 0;
	
	for (auto claim : claims)
	{
		for (int row = claim.left_offset; row < claim.left_offset + claim.width; ++row)
		{
			for (int col = claim.top_offset; col < claim.top_offset + claim.height; ++col)
			{
				fabric[row][col]++;
			}
		}
	}

	for (auto claim : claims)
	{
		bool overlapped = false;
		for (int row = claim.left_offset; row < claim.left_offset + claim.width && !overlapped; ++row)
		{
			for (int col = claim.top_offset; col < claim.top_offset + claim.height && !overlapped; ++col)
			{
				if (fabric[row][col] != 1)
					overlapped = true;
			}
		}

		if (!overlapped)
			return claim.id;

	}

	return -1;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Usage: ./a.out input.txt" << endl;
		return -1;
	}

	ifstream claims_file { argv[1] };
	string line;
	vector<Claim> claims {};

	while (getline(claims_file, line))
	{
		claims.emplace_back(parse_line_into_claim(line));
	}

	claims_file.close();

	cout << "Overlapping area: " << compute_overlapping_claims(claims) << endl;
	cout << "Non-overlapping claim: " << find_non_overlapping_claim(claims) << endl;
	
	return 0;
}


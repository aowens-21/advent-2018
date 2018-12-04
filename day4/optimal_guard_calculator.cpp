#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;

enum class SleepEvent { Wake, Sleep };

struct Event {
	long timestamp;
	variant<int, SleepEvent> info;
};

string remove_all_from_string(const unordered_set<char>& chars_to_remove, const string& s)
{
	string string_with_chars_removed {};
	
	for (auto c : s)
	{
		if (chars_to_remove.count(c) == 0)
			string_with_chars_removed += c;	
	}	

	return string_with_chars_removed;
}

Event parse_line_into_event(string line)
{
	int timestamp_start = line.find('[') + 1;
	int timestamp_end = line.find(']');
	
	string timestamp_string = remove_all_from_string(unordered_set<char> { '-', ':', ' '}, line.substr(timestamp_start, timestamp_end - timestamp_start));
	long timestamp = stol(timestamp_string);

	string info_string = line.substr(timestamp_end + 1);

	auto found_in_string = info_string.find("wake");
	
	if (found_in_string != string::npos)
		return Event { timestamp, SleepEvent::Wake };
	
	found_in_string = info_string.find("sleep");
	if (found_in_string != string::npos)
		return Event { timestamp, SleepEvent::Sleep };
		 
	found_in_string = info_string.find("#");
	string id_string {};
	for (int i = found_in_string + 1; info_string[i] != ' '; ++i)
		id_string += info_string[i];		

	int id = stoi(id_string);
	
	return Event { timestamp,  id };
}

int compute_id_with_most_minutes_slept(const vector<Event>& events)
{
	map<int, int> id_to_minutes_slept;
	int current_most_minutes = 0;
	int current_most_minutes_id = 0;

	int event_index = 0;
	Event current_event;
	
	while (event_index < events.size())
	{
		int minutes = 0;
		// Get current, which will tell us the id
		current_event = events[event_index];

		int next_sleep_index = event_index + 1;
		int next_wake_index = event_index + 2;
		Event next_sleep = events[next_sleep_index];	
		Event next_wake = events[next_wake_index];
		
		if (holds_alternative<int>(next_sleep.info))
			event_index++;
		else
		{	
				while (holds_alternative<SleepEvent>(next_wake.info) && get<SleepEvent>(next_wake.info) == SleepEvent::Wake)
				{
					minutes += (next_wake.timestamp - next_sleep.timestamp - 1);	
					event_index = next_wake_index + 1;
					next_sleep_index = next_wake_index + 1;
					next_wake_index = next_wake_index + 2;
					next_sleep = events[next_sleep_index];
					next_wake = events[next_wake_index];
				}
			
				if (id_to_minutes_slept.count(get<int>(current_event.info)) != 0)
					id_to_minutes_slept[get<int>(current_event.info)] += minutes;
				else
					id_to_minutes_slept.emplace(get<int>(current_event.info), minutes);

				if (id_to_minutes_slept.at(get<int>(current_event.info)) > current_most_minutes)
				{
					current_most_minutes = id_to_minutes_slept.at(get<int>(current_event.info));	
					current_most_minutes_id = get<int>(current_event.info);
				}		
		}
	}
	
	return current_most_minutes_id;
}

pair<int, int> determine_most_common_minute_slept_for_id(int id, const vector<Event>& events)
{
	int hour[59] {};
	
	int event_index = 0;
	Event current_event;
	
	while (event_index < events.size())
	{
		current_event = events[event_index];
	
		if (holds_alternative<int>(current_event.info) && get<int>(current_event.info) == id)
		{
				int next_sleep_index = event_index + 1;
				int next_wake_index = event_index + 2;
				Event next_sleep = events[next_sleep_index];	
				Event next_wake = events[next_wake_index];
				
				while (holds_alternative<SleepEvent>(next_wake.info) && get<SleepEvent>(next_wake.info) == SleepEvent::Wake)
				{
					string next_sleep_string = to_string(next_sleep.timestamp);
					string next_wake_string = to_string(next_wake.timestamp);	
					int lower_bound = stoi(next_sleep_string.substr(next_sleep_string.size() - 2));
					int upper_bound = stoi(next_wake_string.substr(next_wake_string.size() - 2));
					for (int i = lower_bound; i < upper_bound; ++i)
						hour[i]++;
					event_index = next_wake_index + 1;
					next_sleep_index = next_wake_index + 1;
					next_wake_index = next_wake_index + 2;
					next_sleep = events[next_sleep_index];
					next_wake = events[next_wake_index];
				}
		}
		event_index++;
	}
	
	int current_max = 0;
	int current_max_index = 0;
	
	for (int i = 0; i < 59; ++i)
	{
		if (hour[i] > current_max)
		{
			current_max = hour[i];
			current_max_index = i;	
		}
	}

	return make_pair(current_max_index, current_max);
}

pair<int, int> compute_id_with_most_sleeps_on_a_minute(const vector<Event>& events)
{
	unordered_set<int> processed_ids {};
	int current_highest_sleeps = 0;
	int minute_with_highest_sleeps = 0;
	int current_highest_sleeps_id = 0;
	
	for (auto e : events)
	{
		if (holds_alternative<int>(e.info) && processed_ids.count(get<int>(e.info)) == 0)
		{
			int id = get<int>(e.info);
			processed_ids.emplace(id);
		
			pair<int, int> minute_and_id = determine_most_common_minute_slept_for_id(id, events);
			int highest_sleeps_for_id = minute_and_id.second; 
			if (current_highest_sleeps < highest_sleeps_for_id)
			{
				current_highest_sleeps = highest_sleeps_for_id;
				minute_with_highest_sleeps = minute_and_id.first;
				current_highest_sleeps_id = id;
			}
		}	
	}
	
	return make_pair(current_highest_sleeps_id, minute_with_highest_sleeps);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		cout << "Usage: ./a.out input.txt" << endl;

	ifstream events_file { argv[1] };
	string line;
	vector<Event> events {};
	
	while (getline(events_file, line))
	{
		events.push_back(parse_line_into_event(line));	
	}

	// sort based on timestamp
	sort(events.begin(), events.end(), [](Event a, Event b) {
		return a.timestamp < b.timestamp;
	});	
	
	int sleepiest_id = compute_id_with_most_minutes_slept(events);	
	cout << "Sleepy Id: " << sleepiest_id << " Sleepiest minute: " << determine_most_common_minute_slept_for_id(sleepiest_id, events).first << endl; 

	pair<int, int> id_and_highest = compute_id_with_most_sleeps_on_a_minute(events);
	cout << "Id: " << id_and_highest.first << " Minute: " << id_and_highest.second << endl; 

	return 0;
}


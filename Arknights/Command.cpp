#include "Command.h"

void Command::execute(Character::Type& tp, int& i, int& j)const
{
	tp = character_num;
	i = map_row;
	j = map_column;
}

void Command::time_update(int time, bool& timeup)
{
	if (time >= game_running_time)
	{
		is_excute = true;
		timeup = is_excute;
	}
}

void Command::write_in_file(std::ofstream& file)
{
	file << (int)character_num << " "
		 << map_row << " "
		 << map_column << " "
		 << game_running_time << std::endl;
}

void Command::read_from_file(std::string input)
{
	std::stringstream ss(input);
	int num = 0;
	ss >> num >> map_row >> map_column >> game_running_time;

	character_num = static_cast<Character::Type>(num);
}

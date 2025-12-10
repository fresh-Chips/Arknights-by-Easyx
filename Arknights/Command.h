#pragma once

#include <iostream>
#include <fstream>

#include "Vector2.h"
#include "Character.h"

class Command
{
public:
	Command() = default;
	Command(Character::Type tp, int i, int j, int time) 
	{
		character_num = tp;
		map_row = i;
		map_column = j;
		game_running_time = time;
	};
	~Command() = default;
	void execute(Character::Type& tp, int& i, int& j)const;
	void time_update(int time, bool& timeup);
	void write_in_file(std::ofstream& file);
	void read_from_file(std::string input);
	bool get_is_excute()const { return is_excute; }
	//仅提供操作的细节，不负责执行具体如何操作
private:
	int game_running_time = 0;
	int map_row;		//部署位置
	int map_column;
	Character::Type character_num = Character::Type::invalid;//干员编号，用类枚举

	bool is_excute = false;	//到时间后才执行
};


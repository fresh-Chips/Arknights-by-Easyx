#pragma once

#include <iostream>
#include <fstream>

#include "Enemy.h"

class EnemyWave
{
public:
	EnemyWave() = default;
	~EnemyWave() = default;
	EnemyWave(Enemy::EnemyType tp, int n, int time)
	{
		enemy_type = tp;
		enter_num = n;
		game_time = time;
	}

	void time_update(int time, bool& timeup);
	void read_from_file(std::string input);
	bool get_is_excute()const { return is_excute; }
	void execute(Enemy::EnemyType& tp, int& enter) const;

private:
	bool is_excute = false;
	int game_time = 0;
	int enter_num = 0;
	Enemy::EnemyType enemy_type;

};


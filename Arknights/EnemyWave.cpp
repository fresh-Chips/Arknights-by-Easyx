#include "EnemyWave.h"

void EnemyWave::time_update(int time, bool& timeup)
{
	if (time >= game_time)
	{
		is_excute = true;
		timeup = is_excute;
	}
}

void EnemyWave::read_from_file(std::string input)
{
	std::stringstream ss(input);
	int tp = 0;
	ss >> tp >> enter_num >> game_time;

	enemy_type = static_cast<Enemy::EnemyType>(tp);
}

void EnemyWave::execute(Enemy::EnemyType& tp, int& enter) const
{
	tp = enemy_type;
	enter = enter_num;
}

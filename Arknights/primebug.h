#pragma once

#include "Enemy.h"

class Primebug;
extern Atlas atlas_primebug_move_left;
extern Atlas atlas_primebug_move_right;
extern Atlas atlas_primebug_atk_left;
extern Atlas atlas_primebug_atk_right;
extern Primebug primebug_templete;

class Primebug :public Enemy
{
public:
	Primebug() = default;
	~Primebug() = default;
	Primebug(const Primebug&) = default;

	void on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint) override;
	void set_animation() override;
	void copy_templete() override;
	void skill()override{}			//这个敌人没有技能
private:

};

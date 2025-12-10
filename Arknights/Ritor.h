#pragma once

#include "Enemy.h"

class Ritor;
extern Atlas atlas_ritor_move_left;
extern Atlas atlas_ritor_move_right;
extern Atlas atlas_ritor_atk_left;
extern Atlas atlas_ritor_atk_right;
extern Ritor ritor_templete;

class Ritor :public Enemy
{
public:
	Ritor() = default;
	~Ritor() = default;

	void on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint) override;
	void set_animation() override;
	void copy_templete() override;
	void skill()override {}			//这个敌人没有技能
private:

};


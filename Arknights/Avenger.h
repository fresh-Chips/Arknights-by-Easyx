#pragma once

#include "Enemy.h"

class Avenger;
extern Atlas atlas_avenger_move_left;
extern Atlas atlas_avenger_move_right;
extern Atlas atlas_avenger_atk_left;
extern Atlas atlas_avenger_atk_right;
extern Avenger avenger_templete;

class Avenger :public Enemy
{
public:
	Avenger() = default;
	~Avenger() = default;

	void on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint) override;
	void set_animation() override;
	void copy_templete() override;
	void skill()override {}			//这个敌人没有技能
private:
};


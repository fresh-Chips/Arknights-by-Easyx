#pragma once

#include "Enemy.h"

class Shield;
extern Atlas atlas_shield_move_left;
extern Atlas atlas_shield_move_right;
extern Atlas atlas_shield_atk_left;
extern Atlas atlas_shield_atk_right;
extern Shield shield_templete;

class Shield : public Enemy
{
public:
	Shield() = default;
	~Shield() = default;
	
	void on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint) override;
	void set_animation() override;
	void copy_templete() override;
	void skill()override {}			//这个敌人没有技能
private:
};


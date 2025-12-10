#pragma once

#include "Character.h"
#include "Daoguang.h"

extern Atlas atlas_frostleaf_idle_right;
extern Atlas atlas_frostleaf_idle_left;
extern Atlas atlas_frostleaf_atk_right;
extern Atlas atlas_frostleaf_atk_left;

class Frostleaf: public Character
{
public:
	Frostleaf();
	~Frostleaf() = default;
	Frostleaf(const Frostleaf& p);

	void on_update(const std::vector<Enemy*>& active_enemy, int delta)override;
	void on_darw(const Camera& camera)override;

	void set_animation() override;
	void on_attack()override;
	void skill(int delta)override;

private:
	char snd_alias[64];
	char snd_cmd[128];

	void speed_cut();	//减速，被动技能，不需要冷却

	Daoguang daoguang;	//按照攻速这个角色最多只能同时打出一个子弹
};


#pragma once

#include "Character.h"

//能天使
extern Atlas atlas_exusiai_idle_right;			//向右的默认动画
extern Atlas atlas_exusiai_idle_left;			//向左的默认动画
extern Atlas atlas_exusiai_atk_right;			//向右的攻击动画
extern Atlas atlas_exusiai_atk_left;			//向左的攻击动画
extern IMAGE img_exusiai_skill;

class Exusiai: public Character
{
public:
	Exusiai();
	~Exusiai() = default;
	Exusiai(const Exusiai& p);

	void on_update(const std::vector<Enemy*>& active_enemy, int delta) override;
	void set_animation() override;
	void on_attack()override;
	void skill(int delta)override;
	void on_darw(const Camera& camera)override;

private:
	char snd_alias[64];
	char snd_cmd[128];
	void repeat_fire();
};


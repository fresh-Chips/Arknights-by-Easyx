#include "Muelsyse.h"

Muelsyse::Muelsyse()
{
	skill_lasting = 0;
}

void Muelsyse::set_animation()
{
	animation_idle_right.set_atlas(&atlas_muelsyse_idle_right);
	animation_idle_left.set_atlas(&atlas_muelsyse_idle_left);
	animation_atk_left.set_atlas(&atlas_muelsyse_idle_left);
	animation_atk_right.set_atlas(&atlas_muelsyse_idle_left);
	//安全起见，动画类未提供安全的访问图片功能，这里没有攻击图片
	//也要搭载上

	IMAGE* temp = atlas_muelsyse_idle_right.get_image(0);
	size.x = temp->getwidth();
	size.y = temp->getheight();
}

void Muelsyse::skill(int delta)
{
	Character::skill(delta);

	if (is_can_skill && !is_on_skill)
	{
		is_on_skill = true;
		skill_cd = skill_lasting;
	}

	if (is_on_skill)
	{
		skill_stop();
		produce_cost();
	}
}

void Muelsyse::produce_cost()
{
	cost_total += 6;
}

#include "Frostleaf.h"

Frostleaf::Frostleaf()
{
	dam_tp = Damage_type::magic;	//这个角色设定为法伤
}

Frostleaf::Frostleaf(const Frostleaf& p)
{
	*this = p;
	_stprintf_s(snd_alias, "frostleaf_atk%d", snd_count++);
}

void Frostleaf::on_update(const std::vector<Enemy*>& active_enemy, int delta)
{
	Character::on_update(active_enemy, delta);

	if (is_attacking && current_target != nullptr
		&& current_target->get_is_active())		
	{	
		if (current_animation->get_index_frame() == 33)
		{
			daoguang.set_valid(true);
			daoguang.set_position(position.x, position.y - 40);
			daoguang.set_destination(current_target->get_position());
			//为了避免不可预知的bug，先固定子弹终点
			daoguang.on_init();
			daoguang.set_img();
		}
		else if (current_animation->get_index_frame() == 55
			&& daoguang.get_valid())
			daoguang.set_valid(false);	
		//这里说明子弹飞出去了要清理
	}

	if (daoguang.get_valid())
	{
		daoguang.on_update(delta);
	}
	//播放音效
	if (is_attacking && current_animation->get_index_frame() == hurtframe - 12)
	{
		_stprintf_s(snd_cmd, "open res/sound/frostleaf_atk.WAV alias %s", snd_alias);
		mciSendString(snd_cmd, NULL, 0, NULL);

		_stprintf_s(snd_cmd, "play %s from 0", snd_alias);
		mciSendString(snd_cmd, NULL, 0, NULL);
	}
	else if (is_attacking && current_animation->get_index_frame() == hurtframe - 15)
	{
		_stprintf_s(snd_cmd, "close %s", snd_alias);
		mciSendString(snd_cmd, NULL, 0, NULL);
	}

}

void Frostleaf::on_darw(const Camera& camera)
{
	Character::on_darw(camera);

	if (daoguang.get_valid())
		daoguang.on_draw(camera);
}

void Frostleaf::set_animation()
{
	animation_idle_right.set_atlas(&atlas_frostleaf_idle_right);
	animation_idle_left.set_atlas(&atlas_frostleaf_idle_left);
	animation_atk_right.set_atlas(&atlas_frostleaf_atk_right);
	animation_atk_left.set_atlas(&atlas_frostleaf_atk_left);

	IMAGE* temp = atlas_frostleaf_idle_right.get_image(0);
	size.x = temp->getwidth();
	size.y = temp->getheight();

}

void Frostleaf::on_attack()
{
	Character::on_attack();
	speed_cut();
}

void Frostleaf::skill(int delta)
{
	Character::skill(delta);
}

void Frostleaf::speed_cut()
{
	if (current_target != nullptr && current_target->get_is_active())
	{
		current_target->set_speed(0.5);
	}
}

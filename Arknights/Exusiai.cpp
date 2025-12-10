#include "Exusiai.h"

Exusiai::Exusiai()
{
	skill_lasting = 15000;
}

Exusiai::Exusiai(const Exusiai& p)
{
	*this = p;
	_stprintf_s(snd_alias, "exusiai_atk%d", snd_count++);
}

void Exusiai::skill(int delta)		//开启后进行5连击
{
	Character::skill(delta);

	if (is_can_skill && !is_on_skill)
	{
		is_on_skill = true;
		skill_cd = skill_lasting;
		mciSendString("play atkskill_on from 0", NULL, 0, NULL);
	}
	if (is_on_skill)
	{
		skill_cd -= delta;
		//上面这句虽然十分不规范，但好在简洁，直接偷懒了
		if (skill_cd < 0) skill_stop();

		if(is_get_target && is_attacking
			&& current_target->get_is_active())
		repeat_fire();	//可以造成5连击	
	}
}

void Exusiai::on_darw(const Camera& camera)
{
	if (is_on_skill)
	{
		const Vector2& cam = camera.get_position();
		putimagePNG(img_position_idle.x, img_position_idle.y, &img_exusiai_skill);
	}

	Character::on_darw(camera);
}

void Exusiai::repeat_fire()
{
	if (current_animation->get_index_frame() == hurtframe + 1)
	{
		Character::on_attack();
	}
	else if (current_animation->get_index_frame() == hurtframe + 2)
	{
		Character::on_attack();
	}
	else if (current_animation->get_index_frame() == hurtframe + 3)
	{
		Character::on_attack();
	}
	else if (current_animation->get_index_frame() == hurtframe + 4)
	{
		Character::on_attack();
	}
}

void Exusiai::on_update(const std::vector<Enemy*>& active_enemy,int delta)
{
	Character::on_update(active_enemy, delta);
	//实现异步播放音效，不然就只播放一个，可能会卡没
	if (is_attacking && current_animation->get_index_frame() == hurtframe - 12)
	{
		_stprintf_s(snd_cmd, "open res/sound/exusiai_atk.WAV alias %s", snd_alias);
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

void Exusiai::set_animation()
{
	animation_idle_right.set_atlas(&atlas_exusiai_idle_right);
	animation_idle_left.set_atlas(&atlas_exusiai_idle_left);
	animation_atk_right.set_atlas(&atlas_exusiai_atk_right);
	animation_atk_left.set_atlas(&atlas_exusiai_atk_left);

	IMAGE* temp = atlas_exusiai_idle_right.get_image(0);
	size.x = temp->getwidth();
	size.y = temp->getheight();

}

void Exusiai::on_attack()
{
	Character::on_attack();
}

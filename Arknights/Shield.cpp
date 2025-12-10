#include "Shield.h"

void Shield::on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint)
{
	Enemy::on_update(delta, checkpoint);
}

void Shield::set_animation()
{
	animation_move_left.set_atlas(&atlas_shield_move_left);
	animation_move_right.set_atlas(&atlas_shield_move_right);
	animation_atk_left.set_atlas(&atlas_shield_atk_left);
	animation_atk_right.set_atlas(&atlas_shield_atk_right);
}

void Shield::copy_templete()
{
	*this = shield_templete;
}

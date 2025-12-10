#include "primebug.h"

void Primebug::on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint)
{
	Enemy::on_update(delta, checkpoint);
}

void Primebug::set_animation()
{
	animation_move_left.set_atlas(&atlas_primebug_move_left);
	animation_move_right.set_atlas(&atlas_primebug_move_right);
	animation_atk_left.set_atlas(&atlas_primebug_atk_left);
	animation_atk_right.set_atlas(&atlas_primebug_atk_right);
}

void Primebug::copy_templete()
{
	*this = primebug_templete;
}
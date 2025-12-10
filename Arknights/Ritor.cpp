#include "Ritor.h"

void Ritor::on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint)
{
	Enemy::on_update(delta, checkpoint);
}

void Ritor::set_animation()
{
	animation_move_left.set_atlas(&atlas_ritor_move_left);
	animation_move_right.set_atlas(&atlas_ritor_move_right);
	animation_atk_left.set_atlas(&atlas_ritor_atk_left);
	animation_atk_right.set_atlas(&atlas_ritor_atk_right);
}

void Ritor::copy_templete()
{
	*this = ritor_templete;
}

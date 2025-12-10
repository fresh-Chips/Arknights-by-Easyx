#include "Avenger.h"

void Avenger::on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint)
{
	Enemy::on_update(delta, checkpoint);
}

void Avenger::set_animation()
{
	animation_move_left.set_atlas(&atlas_avenger_move_left);
	animation_move_right.set_atlas(&atlas_avenger_move_right);
	animation_atk_left.set_atlas(&atlas_avenger_atk_left);
	animation_atk_right.set_atlas(&atlas_avenger_atk_right);
}

void Avenger::copy_templete()
{
	*this = avenger_templete;
}

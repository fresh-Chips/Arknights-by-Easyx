#include "Enemy.h"

void Enemy::on_init(std::string input)
{
	std::stringstream ss(input);
	ss >> atk >> def >> max_hp
		>> magic_def >> is_remote
		>> atkspeed >> hurtframe
		>> original_speed >> target_value;

	original_speed *= rate_speed;
	health = max_hp;
	speed = original_speed;
	magic_def /= 100.0f;
	set_animation();
}

void Enemy::on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint)
{
	//移动更新
	move_timer += delta;
	if (is_slow_down)
		slow_count_down(delta);
	if (is_move && move_timer >= 16)	//这里也按时间更新
	{
		move_timer = 0;
		if (speed < 1)	//使用慢速移动的重载
			on_move(checkpoint, true);
		else
			on_move(checkpoint);

		if (is_pass_last_cp) //最后一个检查点之后就要进门了
		{
			switch (direction)
			{
			case Direction::left:
				if (position.x <= end_position.x)
				{
					is_active = false;			//进点了就要消失
					is_agress = true;
				}
				break;

			case Direction::up:
				if (position.y >= end_position.y)
				{
					is_active = false;
					is_agress = true;
				}
				break;

			case Direction::right:
				if (position.x >= end_position.x)
				{
					is_active = false;			//进点了就要消失
					is_agress = true;
				}
				break;

			case Direction::down:
				if (position.y <= end_position.y)
				{
					is_active = false;			//进点了就要消失
					is_agress = true;
				}
				break;

			default:
				break;
			}
		}
	}

	//攻击干员
	else if (current_block_ch != nullptr && is_blocked 
		&& current_animation->get_index_frame() == hurtframe)	
		//目前没有可以远程攻击的敌人
	{
		on_attack();
		//强制跳帧并重置计时器间隔
		current_animation->restart();
		current_animation->set_index_frame(hurtframe + 1);
		delta = -16;		//下次更新要空过一帧
	}
	//动画更新
	current_animation->on_update(delta);
}

void Enemy::on_darw(const Camera& camera)
{
	if (is_move)
	{
		current_animation->on_draw(camera,
			position.x + img_position_move.x,
			position.y + img_position_move.y);
		if (health < max_hp)
			draw_health(camera);
	}
	else
	{
		current_animation->on_draw(camera,
			position.x + img_position_atk.x,
			position.y + img_position_atk.y);
		if (health < max_hp)
			draw_health(camera);
	}
}

void Enemy::on_attack()		
{	//默认造成物理伤害
	float damage = atk - current_block_ch->get_defence();
	if (damage < atk * 0.05) 
		damage = atk * 0.05;
	current_block_ch->get_damage(damage);

	if (!current_block_ch->get_is_active())
		cancel_block();
}

void Enemy::on_move(std::vector<Enemy::CheckPoint>& checkpoint)
{
	switch (direction)
	{
	case Direction::left:
		position.x -= speed;
		if (position.x <= current_cp.x && !is_pass_last_cp)
			renew_direction(checkpoint);
		break;

	case Direction::up:
		position.y -= speed;
		if (position.y <= current_cp.y && !is_pass_last_cp)
			renew_direction(checkpoint);
		break;

	case Direction::right:
		position.x += speed;
		if (position.x >= current_cp.x && !is_pass_last_cp)
			renew_direction(checkpoint);
		break;

	case Direction::down:
		position.y += speed;
		if (position.y >= current_cp.y && !is_pass_last_cp)
			renew_direction(checkpoint);
		break;

	default:
		break;
	}
	distance -= speed;	//用于排序
}

void Enemy::on_move(std::vector<Enemy::CheckPoint>& checkpoint, bool lowspeed)
{
	lowspeed_distance += speed;		//突破每次必须至少移动1像素的限制
	if (lowspeed_distance >= 1) 
	{
		lowspeed_distance -= 1;
		distance--;			//用于索敌
		switch (direction)
		{
		case Direction::left:
			position.x--;
			if (position.x <= current_cp.x && !is_pass_last_cp)
				renew_direction(checkpoint);
			break;

		case Direction::up:
			position.y--;
			if (position.y <= current_cp.y && !is_pass_last_cp)
				renew_direction(checkpoint);
			break;

		case Direction::right:
			position.x++;
			if (position.x >= current_cp.x && !is_pass_last_cp)
				renew_direction(checkpoint);
			break;

		case Direction::down:
			position.y++;
			if (position.y >= current_cp.y && !is_pass_last_cp)
				renew_direction(checkpoint);
			break;

		default:
			break;
		}
	}
}

void Enemy::renew_direction(const std::vector<Enemy::CheckPoint>& checkpoint)
{
	direction = checkpoint[cp_number].forward;	//获取这个检查点的方向
	
	if (cp_number < last_cp_num)		//要防止越界
	{
		cp_number++;
		current_cp = checkpoint[cp_number];		//安全
	}
	else if (cp_number == last_cp_num)
		is_pass_last_cp = true;
/*这里这样写的原因是，敌人的下一个检查点是最后一个检查点，
需要知道敌人是否经过了最后一个检查点。*/
}

void Enemy::draw_health(const Camera& camera) const
{
	const Vector2& cam = camera.get_position();
	float len = health / max_hp * 100;

	setfillcolor(RGB(240, 134, 80));
	solidrectangle(position.x - 50 - cam.x
		, position.y - 60 + 100 - cam.y
		, position.x - 50 + (int)len - cam.x
		, position.y - 60 + 105 - cam.y);
}

void Enemy::set_img_position(std::string input)
{
	std::stringstream ss(input);
	ss >> img_position_move.x >> img_position_move.y
		>> img_position_atk.y >> img_position_atk.y;
}

void Enemy::init_position(Vector2 val)
{
	position = val;
	//用动画图集的第一张图片进行设置
	IMAGE* temp = animation_move_left.get_frame();
	img_position_move.x -= temp->getwidth() / 2;
	img_position_move.y -= temp->getheight() / 2;

	temp = animation_atk_left.get_frame();
	img_position_atk.x -= temp->getwidth() / 2;
	img_position_atk.y -= temp->getheight() / 2;
}

void Enemy::set_block_ch(Character* p)
{
	current_block_ch = p;
	is_blocked = true;
	is_move = false;

	//切换动画
	current_animation->restart();
	if (current_animation == &animation_move_left)
		current_animation = &animation_atk_left;

	//else if (current_animation == &animation_move_right)
	//	current_animation = &animation_atk_right;

	else
	{
		std::cout << "动画错误" << std::endl;
		system("pause");
		exit(1);
	}
}

void Enemy::cancel_block()
{
	current_block_ch = nullptr;
	is_blocked = false;
	is_move = true;
	
	//切换动画——由于动画有限，故不做敌人朝右的动画了
	current_animation->restart();
	if (current_animation == &animation_atk_left)
		current_animation = &animation_move_left;

	//else if (current_animation == &animation_atk_right)
	//	current_animation = &animation_move_right;

	else
	{
		std::cout << "动画错误" << std::endl;
		system("pause");
		exit(1);
	}
}


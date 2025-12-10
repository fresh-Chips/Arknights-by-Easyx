#include "Character.h"

void Character::on_init(std::string input)
{
	std::stringstream ss(input);
	ss  >> atk >> def 
		>> health >> cost 
		>> type >> sight_wid 
		>> sight_hig >> atkspeed 
		>> hurtframe >> mp >> can_attack;
	
	if (mp < 0) mp = max_mp;
	set_animation();
	max_hp = health;

}

void Character::set_img_position(std::string input)
{
	std::stringstream ss(input);
	ss >> img_position_idle.x >> img_position_idle.y
	   >> img_position_atk.x >> img_position_atk.y;
}

void Character::set_position(Vector2 val)
{
	position = val;
	img_position_idle += position;
	img_position_atk += position;

	IMAGE* temp = img_drag[num];
	img_position_idle.x -= temp->getwidth() / 2;
	img_position_idle.y -= temp->getheight() / 2;

	temp = animation_atk_right.get_frame();
	img_position_atk.x -= temp->getwidth() / 2;
	img_position_atk.y -= temp->getheight() / 2;
	//设置索敌范围——矩形碰撞箱
	aim_l_t = (Vector2(position.x - sight_wid, position.y - sight_hig));
	aim_r_b = (Vector2(position.x + sight_wid, position.y + sight_hig));
}

void Character::show_aimrange(const Camera& camera)
{
	const Vector2& pos_camera = camera.get_position();
	setlinecolor(RED);
	rectangle(aim_l_t.x - pos_camera.x, aim_l_t.y - pos_camera.y,
			  aim_r_b.x - pos_camera.x, aim_r_b.y - pos_camera.y);
}

void Character::on_update(const std::vector<Enemy*>& active_enemy, int delta)
{
	skill(delta);	//技能更新

	if (!active_enemy.empty() && can_attack && !is_block)//阻挡时不索敌
		is_get_target = findaim(active_enemy);
	
	if (is_get_target && !is_block && can_block)//已索敌且未阻挡且可阻挡
		find_block(active_enemy);

	if (is_block && !current_target->get_is_active())//取消阻挡
	{
		is_block = false;
		is_get_target = false;
		is_get_target = findaim(active_enemy);	//立即重新索敌
	}

	if (is_get_target && is_attacking
		&& current_animation->get_index_frame() == hurtframe)
	{
		on_attack();

		//强制跳帧并重置计时器间隔
		current_animation->restart();
		current_animation->set_index_frame(hurtframe + 1);
		delta = -16;		
	}
	if (current_target != nullptr)
		switch_animation();
	
	current_animation->on_update(delta);
}

void Character::on_darw(const Camera& camera)
{
	if (is_idle)
		current_animation->on_draw(camera, img_position_idle.x, img_position_idle.y);
	else
		current_animation->on_draw(camera, img_position_atk.x, img_position_atk.y);

	draw_health_mp(camera);
	if (is_debug && is_show_aimrange)
		show_aimrange(camera);
}

void Character::draw_health_mp(const Camera& camera)
{
	const Vector2& cam = camera.get_position();
	float len = health / max_hp * 100;

	setfillcolor(RGB(159, 252, 253));
	solidrectangle(img_position_idle.x - cam.x
		, img_position_idle.y + 100 - cam.y
		, img_position_idle.x + (int)len - cam.x
		, img_position_idle.y + 105 - cam.y);

	if (mp == max_mp) return;	//不用冷却的不画技能条
	
	if (!is_on_skill) 
	{
		len = 1.0 * skill_cd / mp * 100;
		setfillcolor(RGB(179, 199, 90));
	}
	else 
	{
		len = 1.0 * skill_cd / skill_lasting * 100;
		setfillcolor(RGB(217, 145, 17));
	}
	solidrectangle(img_position_idle.x - cam.x
		, img_position_idle.y + 106 - cam.y
		, img_position_idle.x + (int)len - cam.x
		, img_position_idle.y + 111 - cam.y);
}

void Character::on_attack()
{
	if (current_target == nullptr || !current_target->get_is_active())
		return;
	float damage = 0;
	switch (dam_tp)
	{
	case Damage_type::physics:
		damage = atk - current_target->get_defence();
		break;
	case Damage_type::magic:
		damage = atk * (1 - current_target->get_defence_magic());
		break;
	case Damage_type::real:
		damage = atk;
		break;
	default:
		break;
	}
	if (damage < atk * 0.05) damage = atk * 0.05;
	current_target->get_damage(damage);
}

bool Character::findaim(const std::vector<Enemy*>& active_enemy)
{
	for (int i = 0; i < active_enemy.size(); i++)
	{
		if (!active_enemy[i]->get_is_active())
			continue;

		Vector2 temp = active_enemy[i]->get_position();
		if (temp.x > aim_l_t.x && temp.y > aim_l_t.y
			&& aim_r_b.x > temp.x && aim_r_b.y > temp.y)
		{
			current_target = active_enemy[i];
			is_attacking = true;
			return true;
		}
	}	
	return false;
}

void Character::skill(int delta)//这里只负责冷却
{
	if (mp == max_mp) return;	//无需冷却就不用管，单独在子类写
	
	if (skill_cd < mp && !is_on_skill && !is_can_skill)
	{
		skill_cd += delta;
		if (skill_cd >= mp) skill_cd = mp;
	}
	else is_can_skill = true;
}

void Character::skill_stop()
{
	is_on_skill = false;
	skill_cd = 0;
	is_can_skill = false;
}

void Character::switch_animation()
{
	if (current_animation == &animation_idle_right && is_attacking)
	{
		current_animation = &animation_atk_right;
		is_idle = false;
	}
	else if (current_animation == &animation_idle_left && is_attacking)
	{
		current_animation = &animation_atk_left;
		is_idle = false;
	}

	else if (current_target->get_position().x < position.x - 0 && is_facing_right)
	{
		is_facing_right = false;
		current_animation = &animation_atk_left;
		animation_atk_left.set_index_frame(animation_atk_right.get_index_frame());
	}
	else if (current_target->get_position().x > position.x + 0 && !is_facing_right)
	{
		is_facing_right = true;
		current_animation = &animation_atk_right;
		animation_atk_right.set_index_frame(animation_atk_left.get_index_frame());
	}
	else if ((is_attacking && !is_get_target)
		|| !current_target->get_is_active())//这说明敌人被消灭且没找到新的敌人
	{
		is_attacking = false;	//切换为普通状态
		is_idle = true;
		current_target = nullptr;	//置空
		if (current_animation == &animation_atk_left)
		{
			animation_atk_left.restart();
			current_animation = &animation_idle_left;
			current_animation->restart();
		}
		else if (current_animation == &animation_atk_right)
		{
			animation_atk_right.restart();
			current_animation = &animation_idle_right;
			current_animation->restart();
		}
		else
		{
			std::cout << "攻击动画结束错误" << std::endl;
			system("pause");
			exit(1);
		}
	}
}

void Character::find_block(const std::vector<Enemy*>& active_enemy)
{
	int dx, dy;
	dx = dy = 0;
	Vector2 temp;

	for (int i = 0; i < active_enemy.size(); i++)
	{
		if (active_enemy[i]->get_is_blocked())//不能阻挡已经被阻挡的敌人
			continue;

		temp = active_enemy[i]->get_position();
		dx = abs(position.x - temp.x);
		dy = position.y - temp.y;

		if (dx <= block_range
			&& dy <= block_range + 20
			&& dy >= 20 - block_range)//通过细调数值完善视觉效果
		{
			is_block = true;
			active_enemy[i]->set_block_ch(this);
			Enemy* a = active_enemy[i];
			current_target = a;
			return;
		}
	}
}

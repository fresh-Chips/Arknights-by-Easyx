#pragma once

#include<sstream>
#include<iostream>
#include<vector>

#include "Camera.h"
#include "graphics.h"
#include "Animation.h"
#include "Character.h"

extern float rate_speed;
extern bool is_debug;

class Character;

class Enemy
{
public:
	enum class EnemyType
	{
		primebug,
		ritor,
		shield,
		avenger
	};
	enum class Direction
	{
		zero,		//0不好辨认，占一位
		left,
		right,
		up,			
		down,
		invalid		//表示不转向
	};
	typedef struct CheckPoint //敌人移动的检查点
	{
		int num;
		int x;
		int y;
		Enemy::Direction forward;
	};
public:
	Enemy() = default;
	virtual ~Enemy() = default;

	virtual void on_update(int delta, std::vector<Enemy::CheckPoint>& checkpoint);
	void on_init(std::string input);
	virtual void set_animation() = 0;
	void on_darw(const Camera& camera);
	void draw_health(const Camera& camera) const;
	virtual void skill() = 0;
	virtual void copy_templete() = 0;
	virtual void get_damage(float damage)
	{
		health -= damage;
		if (health <= 0)
			health = 0;
	}
	virtual void on_attack();

	void on_move(std::vector<Enemy::CheckPoint>& checkpoint);
	void on_move(std::vector<Enemy::CheckPoint>& checkpoint, bool lowspeed);
	void renew_direction(const std::vector<Enemy::CheckPoint>& checkpoint);

	bool operator<(const Enemy& p)
	{
		return this->distance < p.distance;
	}

	//接口
	void set_img_position(std::string input);
	void init_position(Vector2 val);
	void set_end_position(Vector2 val)
	{
		end_position = val;
	}
	void set_target_distance(float dis) 
	{
		distance = dis;
	}
	void set_speed(float val) 
	{ 
		speed = original_speed * val;
		if (speed < original_speed)
		{
			is_slow_down = true;
			slow_time = 0;		//刷新持续时间
		}

		else is_slow_down = false;
	}
	void renew_speed() 
	{
		speed = original_speed;
	}
	void set_block_ch(Character* p);
	void cancel_block();
	void slow_count_down(int time)
	{
		slow_time += time;
		if (slow_time >= 5000)
		{
			renew_speed();
			slow_time = 0;
		}
	}

	void set_enter_num(int n, int end, int start) 
	{
		enter_num = n;
		last_cp_num = end;
		cp_number = start;
	}
	void init_current_animation()	//深拷贝，不然指针都接到模版上了
	{
		current_animation = &animation_move_left;
	}
	bool get_is_active()const { return is_active; }
	bool get_is_agress()const { return is_agress; }
	bool get_is_blocked()const { return is_blocked; }
	int get_target_value()const 
	{
		return target_value;
	}
	Vector2 get_position()const
	{
		return position;
	}
	float get_defence()const 
	{
		return def;
	}
	float get_defence_magic()const 
	{
		return magic_def;
	}
	void set_is_active(bool f)
	{
		is_active = f;
	}
	float get_health() const
	{
		return health;
	}
	int get_distance() const
	{
		return distance;
	}

protected:
	float atk;
	float def;
	float health;
	float max_hp;			//最大血量
	float magic_def;		//法抗
	int atkspeed;
	int hurtframe;
	int target_value;	//目标价值，进入蓝门后扣血
	int percentheal = 100;
	int cp_number = 0;	//指向checkpoint的下标，用于检查点转向
	int enter_num = 0;  //进入的入口编号，用于数组内索引
	int last_cp_num;	//最后一个检查点的下标
	bool is_pass_last_cp = false;	//是否经过最后一个检查点
	
	int move_timer = 0;	//用于更新移动
	float distance;	//到家门的距离，用于索敌
	bool blocked;	//被阻挡的状态
	float speed;
	float original_speed;			//初始速度
	float lowspeed_distance = 0;	//极慢速度下的移动累计
	bool is_active = false;			//是否存活
	bool is_facing_right = false;	//是否朝右，否自然就是朝左
	bool is_move = true;			//是否是移动状态，否为静止状态
	bool is_blocked = false;		//是否被阻挡
	bool is_can_atk = true;			//是否可以攻击
	bool is_remote;					//是否可以远程攻击
	bool is_slow_down = false;		//是否被减速
	int slow_time = 0;				//减速持续倒计时
	Character* current_block_ch = nullptr;	//正在阻挡自己的干员
	
	Direction direction = Direction::left;	//默认向左
	CheckPoint current_cp;				//当前前往的检查点
	Vector2 end_position;				//最后要进点，用于判断
	bool is_agress = false;     		//是否成功入侵

	Vector2 position;
	Vector2 img_position_move;			//这里对应着偏移量
	Vector2 img_position_atk;

	Animation animation_move_right;  //向右的默认动画
	Animation animation_move_left;   //向左的默认动画
	Animation animation_atk_right;   //向右的攻击动画
	Animation animation_atk_left;    //向左的攻击动画
	Animation* current_animation = nullptr; //正在播放的动画
};

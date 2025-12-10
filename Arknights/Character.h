#pragma once

#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "Camera.h"
#include "graphics.h"
#include "Animation.h"
#include "Enemy.h"

class Character;
class Enemy;
extern std::vector<Character*> character_templete;
extern std::vector<IMAGE*> img_drag;
extern bool is_debug;
extern int cost_total;
extern bool is_show_aimrange;

const int block_range = 50;		//使用矩形范围
const int max_mp = 0x3f3f3f3f;

class Character
{
public:
	enum class Type
	{
		exusiai,
		muelsyse,
		frostleaf,
		invalid
	};
	enum class Damage_type
	{
		physics,		//物理
		magic,			//法术
		real			//真实
	};

public:
	Character() = default;
	virtual ~Character() = default;

	virtual void on_update(const std::vector<Enemy*>& active_enemy, int delta);
	void on_init(std::string input);
	virtual void set_animation() = 0;
	virtual void on_darw(const Camera& camera);
	void draw_health_mp(const Camera& camera);
	
	virtual void on_attack();
	bool findaim(const std::vector<Enemy*>&);	
	virtual void skill(int delta);
	void skill_stop();
	void switch_animation();
	void find_block(const std::vector<Enemy*>&);
	static void reset_snd_count()
	{
		snd_count = 0;
	}

	//接口
	void set_img_position(std::string input);
	void set_position(Vector2 val);
	void set_can_block(bool f) 
	{ 
		can_block = f; 
	}
	void set_gridcell_pos(int i, int j) 
	{
		grid_row = i;
		grid_col = j;
	}
	int get_cost()const { return cost; }
	void init_current_animation()	//深拷贝，不然指针都接到模版上了
	{ current_animation = &animation_idle_right; }
	void get_damage(float damage)
	{
		health -= damage;
		if (health <= 0) {
			health = 0;
			is_active = false;
		}
	}
	float get_defence() { return def; }
	bool get_is_active() { return is_active; }
	void get_gridcell_pos(int& i, int& j)
	{
		i = grid_row;
		j = grid_col;
	}

	//调试工具
	void show_aimrange(const Camera& camera);

protected:
	int num;    //编号类型
	float atk;
	float def;
	float health;
	float atkspeed;
	float max_hp;
	int mp;		//能量，或者技力回转，按时间增长只能是整数

	bool is_active = true;
	int cost;
	int type;	//1为高台，2为近战
	int sight_wid;	//索敌范围，用矩形
	int sight_hig;
	int grid_row;	//所在的格子数组坐标，便于干员退场时使用
	int grid_col;

	Vector2 aim_l_t;	//索敌矩形的左上角
	Vector2 aim_r_b;	//索敌矩形的右下角

	int hurtframe;	//出伤帧，有的干员并非在完成一套攻击动作后才造成伤害
	//int maxblock;	//阻挡数，干员只能阻挡有限个敌人
	bool is_facing_right = true;	//是否朝右，否自然就是朝左
	bool is_idle = true;			//是否是默认状态，否为攻击状态

	Enemy* current_target = nullptr;//索敌后攻击时使用
	bool is_get_target = false;		//是否有索敌目标
	bool can_block = false;			//是否允许阻挡敌人
	bool is_block = false;			//是否阻挡敌人

	int attack_cd = 0;				//攻击间隔
	bool can_attack = true;			//是否可以攻击
	bool is_attacking = false;		//是否正在攻击
	int skill_cd = 0;				//技能冷却
	bool is_can_skill = false;		//是否可以释放技能
	bool is_on_skill = false;		//是否释放技能
	int skill_lasting = 0;			//技能持续时间
	inline static int snd_count = 0;//播放音效时使用

	//Timer timer_skill_cd;			//技能冷却计时器

	Damage_type dam_tp = Damage_type::physics;//伤害类型，以后改

	Vector2 position;
	Vector2 img_position_idle;
	Vector2 img_position_atk;
	Vector2 size;					 //图片大小

	Animation animation_idle_right;  //向右的默认动画
	Animation animation_idle_left;   //向左的默认动画
	Animation animation_atk_right;   //向右的攻击动画
	Animation animation_atk_left;    //向左的攻击动画
	Animation* current_animation = nullptr; //正在播放的动画
};


#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "LevelSelector.h"
#include "Timer.h"
#include "Gridcell.h"
#include "Command.h"
#include "primebug.h"
#include "Ritor.h"
#include "Shield.h"
#include "Avenger.h"
#include "Button.h"
#include "EnemyWave.h"
		 
#include <graphics.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <queue>
using namespace std;

extern SceneManager scene_manager;
extern Camera main_camera;
extern vector<vector<Gridcell>> map_loc;
extern vector<IMAGE*> img_drag;
extern int cost_total;
extern Primebug primebug_templete;
#pragma region{引入场景图片资源全局变量}
extern IMAGE img_level1_map;
extern IMAGE img_level2_map;
extern IMAGE img_level3_map;
extern IMAGE img_entry_block;
extern IMAGE img_cost_bg; 
extern IMAGE img_button_continue;
extern IMAGE img_button_pause;
extern IMAGE img_button_quit;
extern IMAGE img_button_speed1;
extern IMAGE img_button_speed2;
extern IMAGE img_sucessboard;
extern IMAGE img_autotips;
extern IMAGE img_autotips_board;
extern IMAGE img_quitgame;
extern IMAGE img_scoreboard;
extern IMAGE img_entry_block;
extern IMAGE img_failed;
extern vector<IMAGE*> img_head;
#pragma endregion

const int ch_count = 3;	//角色数量

class GameScene: public Scene
{
public:
	GameScene();
	~GameScene() = default;
	void on_enter()override; 	//初始化进入
	void on_update(int delta)override;	//处理数据
	void on_draw(const Camera& camera)override; 	//渲染
	void on_input(const ExMessage& msg)override; 	//处理玩家输入
	void on_exit()override; 	//退出时卸载

	void get_level_num();	//获取关卡编号
	void get_is_auto(bool is)
	{
		is_auto_operate = is;
	}
	void init_enemy_pool();
	void create_enemy(Enemy::EnemyType, int enter_num);

	void clear_character();
	void clear_enemy();
	void clear_map_grid();
	void delete_enemy(vector<Enemy*>::iterator&);		//在敌人的is_active为否时触发
	void delete_ch(vector<Character*>::iterator&);
	void check_enemy_pool();

private:
	int level_num = 0;
	int player_health = 0;		//玩家生命，为0时失败
	int enemy_total = 0;		//敌人总数
	int enemy_rest = 0;			//剩余的敌人数量
	int row = 0;				//行
	int column = 0;				//列
	int entrance_cnt = 0;		//敌人进入的入口数量
	bool is_pause = false;		//是否暂停
	bool is_fast = false;		//是否二倍速
	bool is_quit = false;		//是否退出
	bool can_create_enemy = false;	//是否可以创建敌人
	bool is_auto_operate = false;	//自动作战
	vector<Vector2> defence_target;	//蓝门的坐标，可能有多个
	vector<float> def_target_distance;//到蓝门的距离，以像素为单位

	bool choose = false;			//选择状态
	Character::Type choose_tp = Character::Type::invalid;//选择干员的类型
	int now_x = 0;					//用于绘制拖动状态下的图片
	int now_y = 0;
	IMAGE* img_bg = nullptr;
	int img_success_x = -850;
	bool is_success = false;
	bool is_failed = false;
	IMAGE* img_success = &img_sucessboard;

	int cost_increase = 0;
	Timer cost_debug;
	Timer test_emy;
	Timer enemy_debug;
	LOGFONT tc;		//渲染费用数字的字体
		
	void initmap_1();				//关卡初始化函数
	void initmap_2();
	void initmap_3();
	void set_command();
	void set_enemy_wave();

	void switch_pause()		//切换是否暂停
	{
		is_pause = !is_pause;
		pause_button.on_switch();
	}
	void switch_speed()		//切换倍速
	{
		is_fast = !is_fast;
		speed_button.on_switch();
	}
	void switch_quit()
	{
		is_pause = !is_pause;
		is_quit = !is_quit;
		quit_button.on_switch();
		//这里如果按过暂停按钮了，就让暂停按钮关上
		if (pause_button.get_is_turn()) switch_pause();
	}

	vector<function<void()>> mapInital;	//用数组访问
	vector<Character*> active_ch;
	vector<Enemy*> active_enemy;
	vector<Enemy*> enemypool;		//敌人池
	vector<Enemy::CheckPoint> checkpoint;

	vector<int> cp_start;
	vector<int> cp_index;		//指向该入口最后一个检查点的下标
	queue<EnemyWave> enemy_wave;

	int game_time = 0;			//用于自动作战
	bool can_excute = false;
	queue<Command> player_command;	//这个用于输出
	vector<Command> input_command;	//这个用于输入
	
	vector<Button*> button_list;	//统一管理按钮，务必使用指针
	Button pause_button;
	Button speed_button;
	Button quit_button;
	Button cost_button;
	Button score_button;
};


#include <iostream>
#include <graphics.h>
#include <fstream>
#include <sstream>

#include "MenuScene.h"
#include "GameScene.h"
#include "LevelSelector.h"
#include "TujianScene.h"
#include "LoginScene.h"
#include "SceneManager.h"
#include "Character.h"
#include "Gridcell.h"
#include "tools.h"
#pragma comment(lib,"winmm.lib")

using namespace std;
//添加新场景时记得开个new
Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* levelselector_scene = nullptr;
Scene* introduction_scene = nullptr;
Scene* login_scene = nullptr;

Camera main_camera;
SceneManager scene_manager;		//使用场景管理器间接访问各场景
vector<vector<Gridcell>> map_loc;

vector<Character*> character_templete;	//角色模版，游戏内创建角色时直接拷贝
Exusiai exusiai_templete;
Muelsyse muelsyse_templete;
Frostleaf frostleaf_templete;

vector<Enemy*> enemy_templete;			//敌人模版
Primebug primebug_templete;
Ritor ritor_templete;
Shield shield_templete;
Avenger avenger_templete;
#pragma region{加载图片资源的全局变量}
IMAGE img_menu_background;		        //主菜单背景图片
IMAGE img_levelselector_background;		//选择关卡背景图片
IMAGE img_levelselector_confirm;		//确认进入关卡图片
IMAGE img_introduction;					//干员图鉴图片
IMAGE img_login;						//登录确定图片

IMAGE img_level1_map;					//关卡地图背景图片
IMAGE img_level2_map;					//关卡地图背景图片
IMAGE img_level3_map;					//关卡地图背景图片
IMAGE img_entry_block;					//敌人出口遮挡图片
IMAGE img_cost_bg;						//费用条背景图片

IMAGE img_button_continue;				//游戏界面按钮图片
IMAGE img_button_pause;
IMAGE img_button_quit;
IMAGE img_button_speed1;
IMAGE img_button_speed2;
IMAGE* img_start;

IMAGE img_sucessboard;					//成功后的滚动图
IMAGE img_failed;						//失败后的图片
IMAGE img_quitgame;						//退出游戏关卡界面
IMAGE img_scoreboard;					//计分板
IMAGE img_autotips;						//自动作战提示
IMAGE img_autotips_board;				

vector<IMAGE*> img_head;
IMAGE img_exusiai_head;					//角色头像图片
IMAGE img_frostleaf_head;
IMAGE img_muelsyse_head;

IMAGE img_exusiai_skill;				//技能期间的背景图片
#pragma endregion
#pragma region{加载图集资源的全局变量}
vector<IMAGE*> img_drag;				//角色头像指针，用于访问拖动图片
//能天使
Atlas atlas_exusiai_idle_right;			//向右的默认动画
Atlas atlas_exusiai_idle_left;			//向左的默认动画
Atlas atlas_exusiai_atk_right;			//向右的攻击动画
Atlas atlas_exusiai_atk_left;			//向左的攻击动画
//霜叶
Atlas atlas_frostleaf_idle_right;
Atlas atlas_frostleaf_idle_left;
Atlas atlas_frostleaf_atk_right;
Atlas atlas_frostleaf_atk_left; 
Atlas atlas_frostleaf_danoguang;
//缪缪
Atlas atlas_muelsyse_idle_right;
Atlas atlas_muelsyse_idle_left;
Atlas atlas_muelsyse_atk_right;	
Atlas atlas_muelsyse_atk_left;

//敌人的动画
//源石虫
Atlas atlas_primebug_move_left;
Atlas atlas_primebug_move_right;
Atlas atlas_primebug_atk_left;
Atlas atlas_primebug_atk_right;
//小兵
Atlas atlas_ritor_move_left;	//这个名字的意思是暴徒
Atlas atlas_ritor_move_right;
Atlas atlas_ritor_atk_left;
Atlas atlas_ritor_atk_right;
//盾牌哥
Atlas atlas_shield_move_left;
Atlas atlas_shield_move_right;
Atlas atlas_shield_atk_left;
Atlas atlas_shield_atk_right;
//红刀哥
Atlas atlas_avenger_move_left;
Atlas atlas_avenger_move_right;
Atlas atlas_avenger_atk_left;
Atlas atlas_avenger_atk_right;
#pragma endregion			
#pragma region{其他类型的全局变量}
int cost_total = 0;
bool is_debug = true;
bool is_show_aimrange = false;	//是否可以展示角色攻击范围
float rate_speed = 1.0f;		//敌人的公共移动倍率，用于整体调整
const float pi = 3.1415926535;
const float bullet_speed = 0.5;
#pragma endregion			

void flip_atlas(Atlas& src, Atlas& dst);
void load_game_resources();
void create_character_templete();
void create_enemy_templete();
void draw_start()
{
	img_start = new IMAGE;
	loadimage(img_start, "res/startscene.png");
	putimage(0, 0, img_start);
}

int main()
{
	ExMessage msg;
	const int fps = 120;

	initgraph(1280, 720);	//默认窗口大小，最后删除1

	draw_start();
	load_game_resources();
	create_character_templete();
	create_enemy_templete();

	cleardevice();
	delete img_start;

	BeginBatchDraw();

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	levelselector_scene = new LevelSelector();
	introduction_scene = new TujianScene();
	login_scene = new LoginScene();

	scene_manager.set_current_scene(login_scene);//初始化为菜单场景

	while(true)
	{
		DWORD frame_start_time = GetTickCount();
		//用DWORD是因为他返回值设这个类型的，与unsigned int等效，但只用int有溢出风险
		while (peekmessage(&msg))	//使得用户输入被优先处理
		{
			scene_manager.on_input(msg);
		}
		//这里又添加了一个计时器，为的是减小外部循环计时器到执行更新时的时间误差
		//这样向各个类对象传入的时间就是最精准的
		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick_time);
		main_camera.on_update(delta_tick_time);
		last_tick_time = current_tick_time;
		
		cleardevice();
		scene_manager.on_draw(main_camera);
		FlushBatchDraw();

		//在一帧的末尾计时，如果时间没到就休眠掉剩余的时间
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / fps)
		{
			Sleep(1000 / fps - frame_delta_time);			
		}
		//这里使用的是120帧的帧间隔，缓解了跳帧导致的卡顿问题
		//不过电脑实际上最高只能按60多帧运行
	}

	EndBatchDraw();

	return 0;
}

void flip_atlas(Atlas& src, Atlas& dst)
{
	dst.clear();
	for (int i = 0; i < src.get_size(); i++) 
	{
		IMAGE imgtemp;

		flip_image(src.get_image(i), &imgtemp);
		dst.add_image(imgtemp);
	}
}

void load_game_resources()
{
	//场景资源
	loadimage(&img_menu_background, "res/menu.png");
	loadimage(&img_levelselector_background, "res/levelmap.png");
	loadimage(&img_level1_map, "res/map-1.png");
	loadimage(&img_level2_map, "res/map-2.png");
	loadimage(&img_level3_map, "res/map-3.png");
	loadimage(&img_entry_block, "res/main_00-01_entry.png");
	loadimage(&img_levelselector_confirm, "res/levelconfirm.png");
	loadimage(&img_cost_bg, "res/cost.png");
	loadimage(&img_button_continue, "res/mapset/continue.png");
	loadimage(&img_button_pause, "res/mapset/pause.png");
	loadimage(&img_button_quit, "res/mapset/quit.png");
	loadimage(&img_button_speed1, "res/mapset/speed1.png");
	loadimage(&img_button_speed2, "res/mapset/speed2.png");
	loadimage(&img_sucessboard, "res/mapset/successboard.png");
	loadimage(&img_autotips, "res/mapset/autotips.png");
	loadimage(&img_autotips_board, "res/mapset/autotips_board.png");
	loadimage(&img_quitgame, "res/mapset/quitgame.png");
	loadimage(&img_scoreboard, "res/mapset/scoreboard.png");
	loadimage(&img_failed, "res/mapset/missonfailed.png");
	loadimage(&img_introduction, "res/introduction.jpg");
	loadimage(&img_login, "res/log_in.png");
	
	//角色资源
	loadimage(&img_exusiai_head, "res/character/head/1.png");
	loadimage(&img_frostleaf_head, "res/character/head/3.png");
	loadimage(&img_muelsyse_head, "res/character/head/2.png");
	img_head.push_back(&img_exusiai_head);
	img_head.push_back(&img_muelsyse_head);
	img_head.push_back(&img_frostleaf_head);
	
	atlas_exusiai_idle_right.load_from_file("res/animation/1/%d.png", 120);
	flip_atlas(atlas_exusiai_idle_right, atlas_exusiai_idle_left);
	atlas_exusiai_atk_right.load_from_file("res/animation/atk-1/%d.png", 59);
	flip_atlas(atlas_exusiai_atk_right, atlas_exusiai_atk_left);
	loadimage(&img_exusiai_skill, "res/character/angel.png");

	atlas_frostleaf_idle_right.load_from_file("res/animation/3/%d.png", 59);
	flip_atlas(atlas_frostleaf_idle_right, atlas_frostleaf_idle_left);
	atlas_frostleaf_atk_right.load_from_file("res/animation/atk-3/%d.png", 78);
	flip_atlas(atlas_frostleaf_atk_right, atlas_frostleaf_atk_left);
	atlas_frostleaf_danoguang.load_from_file("res/bullet/daoguang%d.png", 8);

	atlas_muelsyse_idle_right.load_from_file("res/animation/2/%d.png", 156);
	flip_atlas(atlas_muelsyse_idle_right, atlas_muelsyse_idle_left);
	
	//敌人资源
	atlas_primebug_move_left.load_from_file("res/enemy/1idle/%d.png", 119);
	//flip_atlas(atlas_primebug_move_left, atlas_primebug_move_right);
	atlas_primebug_atk_left.load_from_file("res/enemy/1atk/%d.png", 46);
	//flip_atlas(atlas_primebug_atk_left, atlas_primebug_atk_right);

	atlas_ritor_move_left.load_from_file("res/enemy/2idle/%d.png", 55);
	//flip_atlas(atlas_ritor_move_left, atlas_ritor_move_right);
	atlas_ritor_atk_left.load_from_file("res/enemy/2atk/%d.png", 59);
	//flip_atlas(atlas_ritor_atk_left, atlas_ritor_atk_right);
	
	atlas_avenger_move_left.load_from_file("res/enemy/4idle/%d.png", 80);
	//flip_atlas(atlas_avenger_move_left, atlas_avenger_move_right);
	atlas_avenger_atk_left.load_from_file("res/enemy/4atk/%d.png", 80);
	//flip_atlas(atlas_avenger_atk_left, atlas_avenger_atk_right);

	atlas_shield_move_left.load_from_file("res/enemy/3idle/%d.png", 71);
	//flip_atlas(atlas_shield_move_left, atlas_shield_move_right);
	atlas_shield_atk_left.load_from_file("res/enemy/3atk/%d.png", 71);
	//flip_atlas(atlas_shield_atk_left, atlas_shield_atk_right);
	

	//拖动图片加载
	img_drag.push_back(atlas_exusiai_idle_right.get_image(0));
	img_drag.push_back(atlas_muelsyse_idle_right.get_image(0));
	img_drag.push_back(atlas_frostleaf_idle_right.get_image(0));

	//音乐文件加载
	mciSendString("open res/sound/m_bat_indust_combine.mp3 alias bgm_level_1", NULL, 0, NULL);
	mciSendString("open res/sound/m_bat_kaltsitteam_combine.mp3 alias bgm_level_2", NULL, 0, NULL);
	mciSendString("open res/sound/ultimate_battle.mp3 alias bgm_level_3", NULL, 0, NULL);
	mciSendString("open res/sound/m_sys_void_combine.mp3 alias bgm_menu", NULL, 0, NULL);
	mciSendString("open res/sound/m_sys_title_combine.mp3 alias bgm_login", NULL, 0, NULL);
	mciSendString("open res/sound/exusiai_atk.WAV alias exusiai_atk", NULL, 0, NULL);
	mciSendString("open res/sound/skill_on.WAV alias atkskill_on", NULL, 0, NULL);
	mciSendString("open res/sound/frostleaf_attack.mp3 alias frostleaf_atk", NULL, 0, NULL);

	mciSendString("setaudio bgm_level_1 volume to 280", NULL, 0, NULL);
	mciSendString("setaudio bgm_level_2 volume to 280", NULL, 0, NULL);
	mciSendString("setaudio bgm_menu volume to 500", NULL, 0, NULL);
	mciSendString("setaudio bgm_login volume to 500", NULL, 0, NULL);
}

void create_character_templete()
{
	character_templete.push_back(&exusiai_templete);
	character_templete.push_back(&muelsyse_templete);
	character_templete.push_back(&frostleaf_templete);

	int len = character_templete.size();
	ifstream file("res/animation/data.txt");
	if (!file.is_open()) {
		cerr << "错误：无法打开文件" << endl;
		system("pause");
		exit(1);
	}
	string input;
	getline(file, input);	//先放出第一行的提示信息
	for (int i = 0; i < len; i++)
	{
		getline(file, input);
		if (input.empty()) continue;
		
		character_templete[i]->on_init(input);
	}
	file.close();
	//设置渲染偏移量
	file.open("res/animation/offset.txt");
	for (int i = 0; i < len; i++)
	{
		getline(file, input);
		if (input.empty()) continue;

		character_templete[i]->set_img_position(input);
	}
}

void create_enemy_templete()
{
	enemy_templete.push_back(&primebug_templete);
	enemy_templete.push_back(&ritor_templete);
	enemy_templete.push_back(&shield_templete);
	enemy_templete.push_back(&avenger_templete);

	int len = enemy_templete.size();
	ifstream file("res/enemy/data_enemy.txt");
	if (!file.is_open()) {
		cerr << "错误：无法打开文件" << endl;
		system("pause");
		exit(1);
	}
	string input;
	getline(file, input);	//先放出第一行的提示信息
	for (int i = 0; i < len; i++)
	{
		getline(file, input);
		if (input.empty()) continue;

		enemy_templete[i]->on_init(input);
	}
	file.close();
	//设置渲染偏移量
	file.open("res/enemy/offset_enemy.txt");
	for (int i = 0; i < len; i++)
	{
		getline(file, input);
		if (input.empty()) continue;

		enemy_templete[i]->set_img_position(input);
	}
	file.close();
}
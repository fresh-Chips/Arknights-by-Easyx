#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "Timer.h"
		 
extern SceneManager scene_manager;
//extern Atlas;//可用于测试
extern IMAGE img_menu_background;
extern bool is_debug;

class MenuScene: public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;
	void on_enter()override; 	//初始化进入
	void on_update(int delta)override;	//处理数据
	void on_draw(const Camera& camera)override; 	//渲染
	void on_input(const ExMessage& msg)override; 	//处理玩家输入
	void on_exit()override; 	//退出时卸载
private:
	Timer timer;
	Animation animation_run_test;
};


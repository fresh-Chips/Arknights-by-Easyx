#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "Timer.h"

extern SceneManager scene_manager;
extern IMAGE img_login;

class LoginScene: public Scene
{
public:
	LoginScene() = default;
	~LoginScene() = default;

	void on_enter()override; 	//初始化进入
	void on_update(int delta)override;	//处理数据
	void on_draw(const Camera& camera)override; 	//渲染
	void on_input(const ExMessage& msg)override; 	//处理玩家输入
	void on_exit()override; 	//退出时卸载

private:
	bool is_exit = false;
	int img_idx = 0;
	IMAGE* img = new IMAGE;
	Timer switch_frame;
};


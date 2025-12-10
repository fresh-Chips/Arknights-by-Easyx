#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "Timer.h"

extern SceneManager scene_manager;
extern IMAGE img_introduction;

class TujianScene: public Scene
{
public:
	TujianScene() = default;
	~TujianScene() = default;

	void on_enter()override; 	//初始化进入
	void on_update(int delta)override;	//处理数据
	void on_draw(const Camera& camera)override; 	//渲染
	void on_input(const ExMessage& msg)override; 	//处理玩家输入
	void on_exit()override; 	//退出时卸载

private:

};


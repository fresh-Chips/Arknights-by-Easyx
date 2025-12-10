#pragma once

#include <graphics.h>
#include <iostream>
#include "Camera.h"
#include "Animation.h"

extern Camera main_camera;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	//虚函数用于继承
	virtual void on_enter(){}	//初始化进入
	virtual void on_update(int delta){}	//处理数据
	virtual void on_draw(const Camera& camera){}	//渲染
	virtual void on_input(const ExMessage& msg){}	//处理玩家输入
	virtual void on_exit(){}	//退出卸载
protected:
	inline static int level_num = 0;
	inline static bool is_auto = false;
};


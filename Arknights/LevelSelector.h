#pragma once

#include "Scene.h"
#include "SceneManager.h"

extern SceneManager scene_manager;
extern Scene* game_scene;
extern IMAGE img_levelselector_background;
extern IMAGE img_levelselector_confirm;

const int level_count = 5;

class LevelSelector: public Scene
{
public:
	LevelSelector() = default;
	~LevelSelector() = default;
	void on_enter()override; 	//初始化进入
	void on_update(int delta)override;	//处理数据
	void on_draw(const Camera& camera)override; 	//渲染
	void on_input(const ExMessage& msg)override; 	//处理玩家输入
	void on_exit()override; 	//退出时卸载

private:
	bool is_level_selected = false;			//是否已选择关卡
	bool is_autodeployed[level_count] = { false };			//是否已选择自动作战
	int level_num = 0;
};


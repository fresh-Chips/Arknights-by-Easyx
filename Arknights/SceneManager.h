#pragma once
#include "Scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* levelselector_scene;
extern Scene* introduction_scene;
extern Scene* login_scene;
/*********
	添加新场景步骤：
	1、新建场景类并继承Scene基类
	2、在主文件中声明该类的全局指针，并在下面的初始化阶段实例化该场景
	3、回到这个文件，将该场景指针声明到这个文件的上面，同时为枚举扩展
	4、在switch_to中的switch语句内添加跳转条件分支
	5、记得在该场景的相关成员函数中添加用于跳转场景的语句
	6、检查新场景头文件中全局变量的引用
*********/
class SceneManager
{
public:
	enum class SceneType	//使用枚举，避免子场景之间互相传参
	{
		Menu,
		Game,
		LevelSelector,
		Introduction,
		Login
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		current_scene->on_enter();
	}
	void switch_to(SceneType type);
	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}
	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}
	void on_input(const ExMessage& msg) 
	{
		current_scene->on_input(msg);
	}
private:
	Scene* current_scene = nullptr;
};


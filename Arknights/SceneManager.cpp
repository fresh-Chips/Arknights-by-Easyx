#include "SceneManager.h"

void SceneManager::switch_to(SceneType type)
{	
	//由于场景较少，使场景生命周期与程序生命周期相等
	//使用时直接重置场景状态而非删除并创建

	current_scene->on_exit();	//退出当前场景
	switch (type)				//找到新的场景
	{
	case SceneType::Menu:
		current_scene = menu_scene;
		break;
	case SceneType::Game:
		current_scene = game_scene;
		break;
	case SceneType::LevelSelector:
		current_scene = levelselector_scene;
		break;
	case SceneType::Introduction:
		current_scene = introduction_scene;
		break;
	case SceneType::Login:
		current_scene = login_scene;
		break;
	default:
		break;
	}
	current_scene->on_enter();	//进入新场景
}


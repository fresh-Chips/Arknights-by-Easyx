#include "MenuScene.h"

void MenuScene::on_enter()
{
	//测试计时器类
	timer.set_wait_time(1000);
	timer.set_one_shot(false);
	//使用lambda语句
	timer.set_callback([]() {std::cout << "shot!" << std::endl; });
	//播放音乐
	mciSendString("play bgm_menu repeat", NULL, 0, NULL);
}

void MenuScene::on_update(int delta)
{
	//timer.on_update(delta);  //测试功能，没什么用
}

void MenuScene::on_draw(const Camera& camera)
{
	const Vector2& pos_camera = camera.get_position();
	putimagePNG(0 - pos_camera.x, 0 - pos_camera.y, &img_menu_background);

	settextcolor(RED);
	settextstyle(30, 0, "宋体");
	outtextxy(0, 0, "开启调试模式后(本界面中按任意键开关)");
	if (is_debug)outtextxy(0, 35, "已开启");
	else outtextxy(0, 35, "未开启");
	outtextxy(0, 70, "按'Q'显示攻击范围");
	outtextxy(0, 105, "按'C'加50费");
}

void MenuScene::on_input(const ExMessage& msg)
{
	if (msg.message == WM_KEYUP)
	{
		//切换调试模式
		is_debug = !is_debug;
		//跳转游戏场景的功能
		//scene_manager.switch_to(SceneManager::SceneType::Game);
						  //第一个双冒号表示管理器作用域下的枚举
		//目前在测试镜头抖动
		//main_camera.shake(10, 350);
	}
	if (msg.message == WM_LBUTTONUP)
	{
		if (msg.x > 720 && msg.x < 1280 && msg.y>115 && msg.y < 275)
		{
			//进入选关界面
			scene_manager.switch_to(SceneManager::SceneType::LevelSelector);
		}
		else if (msg.x > 928 && msg.x < 1200 && msg.y>295 && msg.y < 415)
		{
			//进入图鉴界面
			scene_manager.switch_to(SceneManager::SceneType::Introduction);
		}
	}
}

void MenuScene::on_exit()
{
	std::cout << "成功退出菜单" << std::endl;
}
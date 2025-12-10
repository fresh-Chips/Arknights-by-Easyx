#include "LevelSelector.h"

void LevelSelector::on_enter()
{
	//部分初始化，是否自动作战可以保留
	is_level_selected = false;
	level_num = 0;

	std::cout << "开始选择关卡" << std::endl;
}
void LevelSelector::on_update(int delta)
{

}

void LevelSelector::on_draw(const Camera& camera)
{
	putimagePNG(0, 0, &img_levelselector_background);

	if (is_level_selected) 
	{
		putimagePNG(0, 0, &img_levelselector_confirm);
		if (is_autodeployed[level_num])
		{
			setfillcolor(WHITE);
			solidrectangle(1056, 585, 1075, 605);
		}
	}	
}

void LevelSelector::on_input(const ExMessage& msg)
{
	if (is_level_selected && msg.message == WM_LBUTTONUP) 
	{
		if (!(msg.x < 1280 && msg.x > 1030 && msg.y < 720 && msg.y > 570))
		{
			is_level_selected = false;	//没点那两个按钮就取消选择
		}
		else if (msg.x < 1245 && msg.x > 1036 && msg.y < 690 && msg.y > 627)
		{
			Scene::level_num = level_num;	//通过静态变量传递关卡编号
			Scene::is_auto = is_autodeployed[level_num];
			//准备切换音乐
			mciSendString("stop bgm_menu", NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Game);			
		}
		else if (msg.x < 1245 && msg.x > 1045 && msg.y < 615 && msg.y > 570)
		{		
			is_autodeployed[level_num] = !is_autodeployed[level_num];
		}
	}
	else if (msg.message == WM_LBUTTONUP) 
	{
		if (msg.x < 372 && msg.x > 12 && msg.y < 63 && msg.y > 16) 
		{
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
		else if (msg.x < 460 && msg.x > 313 && msg.y < 466 && msg.y > 420) 
		{
			is_level_selected = true;
			level_num = 1;			
		}
		else if (msg.x < 915 && msg.x > 770 && msg.y < 268 && msg.y > 222) 
		{
			is_level_selected = true;
			level_num = 2;
		}
		else if (msg.x < 915 && msg.x > 770 && msg.y < 166 && msg.y > 120) 
		{
			is_level_selected = true;
			level_num = 3;
		}
	}	 
	
}

void LevelSelector::on_exit()
{

}

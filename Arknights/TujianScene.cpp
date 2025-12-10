#include "TujianScene.h"

void TujianScene::on_enter()
{
}

void TujianScene::on_update(int delta)
{
}

void TujianScene::on_draw(const Camera& camera)
{
	putimage(0, 0, &img_introduction);
}

void TujianScene::on_input(const ExMessage& msg)
{
	if (msg.message == WM_KEYUP || msg.message == WM_LBUTTONUP)
	{
		scene_manager.switch_to(SceneManager::SceneType::Menu);
	}
}

void TujianScene::on_exit()
{
}

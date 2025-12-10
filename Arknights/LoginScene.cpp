#include "LoginScene.h"

void LoginScene::on_enter()
{
	switch_frame.set_one_shot(false);
	switch_frame.set_wait_time(31);
	switch_frame.set_callback([&]() {img_idx++; });

	mciSendString("play bgm_login repeat from 0", NULL, 0, NULL);
}

void LoginScene::on_update(int delta)
{
	if (is_exit) switch_frame.on_update(delta);

	if (img_idx >= 477)
		scene_manager.switch_to(SceneManager::SceneType::Menu);
}

void LoginScene::on_draw(const Camera& camera)
{
	if (!is_exit)putimage(0, 0, &img_login);
	else
	{
		char buffer[64];
		_stprintf_s(buffer, "res/start_video/start%d.jpg", img_idx);
		loadimage(img, buffer);
		putimage(0, -51, img); //源视频处理失误，这里调整一下
	}
}

void LoginScene::on_input(const ExMessage& msg)
{
	if (msg.message == WM_KEYUP || msg.message == WM_LBUTTONUP)
	{
		is_exit = true;
	}
}

void LoginScene::on_exit()
{
	delete img;
	mciSendString("stop bgm_login", NULL, 0, NULL);
}

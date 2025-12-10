#include "Camera.h"

Camera::Camera()
{
	timer_shake.set_one_shot(true);
	timer_shake.set_callback([&]()
		{
			is_shaking = false;
			reset();			//抖动结束后重置摄像机位置
		});
}

void Camera::shake(float strength, int duration)
{
	is_shaking = true;
	shaking_strength = strength;

	timer_shake.set_wait_time(duration);
	timer_shake.restart();
}

void Camera::on_update(int delta)
{
	timer_shake.on_update(delta);

	if (is_shaking)
	{
		//效果为以原点为中心，生成一个1.0*1.0大小的正方形随机范围，再乘上震动强度
		position.x = (-50 + rand() % 100) / 50.0 * shaking_strength;
		position.y = (-50 + rand() % 100) / 50.0 * shaking_strength;
	}
}


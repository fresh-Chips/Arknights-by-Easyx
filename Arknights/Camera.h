#pragma once

#include "Vector2.h"
#include "Timer.h"

class Camera
{
public:
	Camera();
	~Camera() = default;

	void shake(float strength, int duration);
	void on_update(int delta);

	const Vector2 get_position() const
	{
		return position;
	}
	
	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

private:
	Vector2 position;		  //摄像机位置
	Timer timer_shake;		  //摄像机抖动定时器
	bool is_shaking = false;  //摄像机是否在抖动
	float shaking_strength;	  //摄像机抖动强度
};


#pragma once

#include <functional>

#include "Vector2.h"
#include "Camera.h"
#include "tools.h"

extern const float bullet_speed;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_position(float x, float y)
	{
		position.x = x, position.y = y;
	}
	void set_velocity(float x, float y)
	{
		velocity.x = x, velocity.y = y;
	}
	void set_velocity(float speed)
	{
		velocity.x = speed * unit.x;
		velocity.y = speed * unit.y;
	}

	void set_angle(float angle)
	{
		this->angle = angle;
	}
	const Vector2& get_position()const
	{
		return position;
	}
	const Vector2& get_size()const
	{
		return size;
	}
	const Vector2& get_destination()const
	{
		return destination;
	}	
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
	void set_valid(bool flag)
	{
		valid = flag;
	}
	bool get_valid()const
	{
		return valid;
	}
	bool check_can_remove()const
	{
		return can_remove;
	}
	void set_destination(Vector2 des)
	{
		this->destination = des;
	}

	virtual void on_update(int delta);
	virtual void on_draw(const Camera& camera);
	virtual void on_collide()
	{
		if (callback)
			callback();
	}
	virtual bool check_collision(const Vector2& position, const Vector2& size);
protected:
	Vector2 size;				//子弹大小
	Vector2 position;			//子弹位置
	Vector2 velocity;			//子弹速度
	Vector2 destination;		//子弹终点，目标敌人的位置
	Vector2 unit;				//单位向量
								
	bool valid = true;			//子弹是否有效
	bool can_remove = false;	//子弹是否可以删除
	float angle = 0;			//子弹角度

	std::function<void()> callback;
};


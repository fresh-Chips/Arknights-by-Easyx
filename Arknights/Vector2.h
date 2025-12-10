#pragma once

#include <cmath>

class Vector2
{
public:
	float x = 0;
	float y = 0;
public:
	Vector2() = default;
	~Vector2() = default;
	Vector2(float x,float y):x(x),y(y){}//创建向量时需要给定初值

	//重载常见的运算符
	//使用加减法时需要传一个向量或者利用x和y现生成一个临时向量
	Vector2 operator+(const Vector2& vec) const 
	{
		return Vector2(x + vec.x, y + vec.y);
	}	//会返回一个新的向量，可以链式运算
	Vector2 operator-(const Vector2& vec) const 
	{
		return Vector2(x - vec.x, y - vec.y);
	}
	//加等减等传参方法同上，不过这里是对“this对象”的修改
	void operator+=(const Vector2& vec)		
	{
		this->x += vec.x;
		this->y += vec.y;
	}
	void operator-=(const Vector2& vec) 
	{
		this->x -= vec.x;
		this->y -= vec.y;
	}
	//这里是数乘向量，返回新向量
	Vector2 operator*(float val) const
	{	// val = value
		return Vector2(x * val, y * val);
	}
	//这里是向量的点乘，返回一个数，没用的话最后会考虑删除
	float operator*(const Vector2& vec) const
	{	
		return this->x * vec.x + this->y * vec.y;
	}
	//乘等的重载
	void operator *= (float val)
	{	
		x *= val, y *= val;
	}
	float length()const
	{
		return sqrt(x * x + y * y);
	}
	//将向量标准化（单位长度），可能没用
	Vector2 normalize()
	{
		float len = length();

		if (len == 0) return Vector2(0, 0);
		return Vector2(x / len, y / len);
	}
	//碰撞箱判断
	bool is_include(const Vector2& veclt, const Vector2& vecrb)
	{
		return (this->x > veclt.x && this->y > veclt.y
			 && vecrb.x > this->x && vecrb.y > this->y);
	}
};


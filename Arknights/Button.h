#pragma once

#include <graphics.h>
#include <functional>
#include <iostream>

#include "Vector2.h"
#include "tools.h"

class Button
{
public:
	Button() = default;
	~Button() = default;

	void button_init(Vector2 pos, Vector2 ran);
	void on_input(const ExMessage& msg);
	void on_draw() 
	{
		putimagePNG(position.x, position.y, current_img);
	}
	void set_img(IMAGE* img)
	{
		img_button = img; 
		img_button_switch = img_button;	//十分安全，防止指针被置空
		current_img = img_button;
	}
	void set_img(IMAGE* img, IMAGE* img_sw)
	{
		img_button = img;
		img_button_switch = img_sw;
		current_img = img_button;
	}
	void on_switch()	//这里打开开关会切换图片
	{
		is_turn_on = !is_turn_on;
		current_img = is_turn_on ? img_button_switch : img_button;
	}
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
	void reset()
	{
		is_turn_on = false; 
		current_img = img_button;
	}
	bool get_is_turn()const { return is_turn_on; }

	
private:
	bool is_turn_on = false;

	Vector2 position;		//左上角的坐标，使用矩形
	Vector2 pos_range;		//矩形的右下角,用于判断点击
	IMAGE* img_button;
	IMAGE* img_button_switch = nullptr;//按下按钮后，可以切换图片
	IMAGE* current_img = img_button;

	std::function<void()> callback;	//点击后触发的函数
};


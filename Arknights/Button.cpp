#include "Button.h"

void Button::button_init(Vector2 pos, Vector2 ran)
{
	position = pos;
	pos_range = ran;
}

void Button::on_input(const ExMessage& msg)
{
	if (callback == nullptr) return;//没有回调说明不用处理事件
	if (msg.x > position.x && msg.y > position.y
		&& msg.x < pos_range.x && msg.y < pos_range.y)
	{
		callback();
	}
}

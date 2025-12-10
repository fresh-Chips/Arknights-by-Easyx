#include "Animation.h"


void Animation::on_update(int delta)
{
	timer += delta;		//计时
	if (timer >= interval)
	{
		timer = 0;
		index_frame++;
		if (index_frame >= atlas->get_size()) 
		{	//循环播放从头开始，否则停在最后一帧
			index_frame = is_loop ? 0 : atlas->get_size() - 1;
			if (!is_loop && callback)	//动画不循环且存在回调函数时激活
				callback();
		}
	}
}

void Animation::on_draw(const Camera& camera, int x, int y)
{
	const Vector2& pos_camera = camera.get_position();
	putimagePNG(x - pos_camera.x, y - pos_camera.y, atlas->get_image(index_frame));
}

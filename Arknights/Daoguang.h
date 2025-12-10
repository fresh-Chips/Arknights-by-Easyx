#pragma once

#include <graphics.h>

#include "Bullet.h"
#include "Atlas.h"

extern Atlas atlas_frostleaf_danoguang;
extern const float pi;

class Daoguang : public Bullet
{
public:
	Daoguang();
	~Daoguang() = default;

	virtual void on_update(int delta)override;
	virtual void on_draw(const Camera& camera)override;
	void set_img()
	{
		//注意前面的处理已经将弧度转化为角度了
		//这里为了可读性舍弃了短路的优化
		if (angle <= 22.5 && angle > -22.5)
			img = atlas_frostleaf_danoguang.get_image(0);
		else if (angle <= -22.5 && angle > -67.5)
			img = atlas_frostleaf_danoguang.get_image(1);
		else if (angle <= -67.5 && angle > -112.5)
			img = atlas_frostleaf_danoguang.get_image(2);
		else if (angle <= -112.5 && angle > -157.5)
			img = atlas_frostleaf_danoguang.get_image(3);
		else if (angle <= -157.5 || angle > 157.5)
			img = atlas_frostleaf_danoguang.get_image(4);
		else if (angle <= 157.5 && angle > 112.5)
			img = atlas_frostleaf_danoguang.get_image(5);
		else if (angle <= 112.5 && angle > 67.5)
			img = atlas_frostleaf_danoguang.get_image(6);
		else if (angle <= 67.5 && angle > 22.5)
			img = atlas_frostleaf_danoguang.get_image(7);
	}
	void on_init()
	{
		unit = (destination - position).normalize();

		angle = -atan2(unit.y, unit.x) * 180.0 / pi;
		set_velocity(bullet_speed);
		set_img();
	}
private:
	IMAGE* img = nullptr;
};


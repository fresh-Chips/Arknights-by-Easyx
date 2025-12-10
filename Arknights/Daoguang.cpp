#include "Daoguang.h"

Daoguang::Daoguang()
{
	size.x = 60, size.y = 60;
}

void Daoguang::on_update(int delta)
{
    //由于固定了终点，这里不用再更新方向了
	position += velocity * (float)delta;

    if (check_collision(destination, Vector2(50, 50)))
        valid = false;
}

void Daoguang::on_draw(const Camera& camera)
{
	Vector2 cam = camera.get_position();
    putimagePNG(position.x - size.x / 2 - cam.x,
                position.y - size.y / 2 - cam.y, img);
}

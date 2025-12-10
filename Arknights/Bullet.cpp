#include "Bullet.h"

bool Bullet::check_collision(const Vector2& position, const Vector2& size)
{
    return this->position.x + this->size.x / 2 >= position.x
        && this->position.x + this->size.x / 2 <= position.x + size.x
        && this->position.y + this->size.y / 2 >= position.y
        && this->position.y + this->size.y / 2 <= position.y + size.y;
}

void Bullet::on_update(int delta)
{

}

void Bullet::on_draw(const Camera& camera)
{ 

}
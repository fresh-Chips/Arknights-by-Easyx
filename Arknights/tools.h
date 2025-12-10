#pragma once

#include <graphics.h>
#include "Camera.h"

void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);//实现透明图片无黑边导入

//inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img);

void flip_image(IMAGE* s, IMAGE* t);
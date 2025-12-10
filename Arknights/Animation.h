#pragma once
#include "Atlas.h"
#include "tools.h"
#include "Camera.h"

#include <functional>
class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void on_update(int delta);
	void on_draw(const Camera& camera, int x, int y);
	//——简单接口——//
	void reset()	//重置动画
	{
		interval = 0;
		index_frame = 0;
	}
	void set_atlas(Atlas* new_atlas)	//设置图片集
	{
		reset();	//先重置防止索引越界
		interval = 16;		//基本上都是这个数，有需求再单独改
		atlas = new_atlas;
	}
	void set_loop(bool jud)	//确认是否需要循环播放
	{
		is_loop = jud;
	}
	void set_interval(int ms)	//设置帧间隔
	{
		interval = ms;
	}
	int get_index_frame()	//获取帧索引
	{
		return index_frame;
	}
	void restart()			//重置计时器状态
	{
		index_frame = 0;
		timer = 0;
	}
	void set_index_frame(int idx)	//设置帧索引
	{
		index_frame = idx;
	}
	IMAGE* get_frame()	//获取当前帧图片
	{
		return atlas->get_image(index_frame);
	}
	bool check_loop_finished()
	{
		if (is_loop == true) return false;	//循环播放的不用结束
		return (index_frame == atlas->get_size() - 1);	//播到最后时结束
	}
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
private:
	int interval = 16;				//帧间隔，默认16ms
	int index_frame = 0;			//帧索引
	int timer = 0;					//计时器
	bool is_loop = true;
	Atlas* atlas = nullptr;
	std::function<void()> callback;	//回调函数
};


#pragma once

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void on_update(int delta);
	//简单函数直接写，不用在类外实现
	void restart()	//重置计时器
	{
		pass_time = 0;
		shotted = false;
	}
	void set_wait_time(int val)
	{
		wait_time = val;
	}
	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
	void pause()
	{
		paused = true;
	}
	void resume()
	{
		paused = false;
	}
	int get_time()
	{
		return pass_time;
	}
private:
	int pass_time = 0;		       //经过时间
	int wait_time = 0;		       //等待时间
	bool paused = false;	       //是否暂停
	bool shotted = false;	       //是否计时
	bool one_shot = false;	       //单次计时
	std::function<void()> callback;//触发回调
};


#include "Timer.h"

void Timer::on_update(int delta)
{
	if (paused) return;//暂停时不用执行，也不会累加时间

	pass_time += delta;	//累加时间
	if (pass_time >= wait_time)
	{
//如果现在要持续计时或者单次计时还没有触发则触发，后面是检查回调是否有效
		if ((!one_shot || (one_shot && !shotted)) && callback)
			callback(); //执行回调函数
		shotted = true;	//此时已经触发了定时器
		pass_time = 0;	//重置已经度过的时间
	}
}
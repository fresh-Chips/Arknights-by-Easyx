#pragma once

#include <graphics.h>
#include <vector>

class Atlas	 //图片集
{
public:
	Atlas() = default;
	~Atlas() = default;
	void load_from_file(LPCTSTR path_template, int num);
	//几个常用接口
	void clear() 
	{
		img_list.clear();
	}
	int get_size() 
	{
		return (int)img_list.size();
	}
	IMAGE* get_image(int index)
	{
		if (index < 0 || index >= img_list.size()) return nullptr;
		return img_list[index];
	}
	void add_image(const IMAGE& img)
	{
		IMAGE* temp = new IMAGE(img);
		img_list.push_back(temp);
	}
private:
	std::vector<IMAGE*> img_list;
};



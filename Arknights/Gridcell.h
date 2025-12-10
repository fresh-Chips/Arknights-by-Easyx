#pragma once

#include "Exusiai.h"
#include "Muelsyse.h"
#include "Frostleaf.h"
#include "Vector2.h"
#include <vector>

extern Exusiai exusiai_templete;
extern Muelsyse muelsyse_templete;
extern Frostleaf frostleaf_templete;

class Gridcell
{
public:
	enum class Gridtype
	{
		deploy_ban,
		ground,
		platform,
		waterpool
	};
public:
	Gridcell() = default;
	~Gridcell() = default;

	bool findlocation(int x, int y)const;	//用于循环中
	void create_character(Character::Type ty);
	void cleargrid();
	
	void set_cell(int x, int yt, int yb, int h, int i, int width = 100);//中间三个每张地图单独设置
	void set_cell(int x, int yt, int yb, int width = 100);	   //重载
	
	void set_type(Gridcell::Gridtype tp) { gridtype = tp; }
	Gridcell::Gridtype get_type() const { return gridtype; }

	bool is_occupy() { return occu; }
	void set_occupy(bool is) 
	{ 
		occu = is; 
	}
	int get_x() { return x_left; }
	Character* get_character() { return ch; }

private:
	Character* ch = nullptr;

	int x_left;
	int y_top;
	int x_right;
	int y_bottom;
	bool occu = false;		//occupy，占用中的格子不可重复部署
	Gridtype gridtype = Gridcell::Gridtype::ground;//默认为地面，其他的初始化时改
};


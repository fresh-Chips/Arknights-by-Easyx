#include "Gridcell.h"

bool Gridcell::findlocation(int x, int y) const
{
	if (occu) return false;		//占用了就不能放置
	//禁止部署的位置也不行
	if (gridtype == Gridcell::Gridtype::deploy_ban) return false;
	
	return (x > x_left && x < x_right
			&& y > y_top && y < y_bottom);
}

void Gridcell::create_character(Character::Type ty)
{
	occu = true;
	
	switch (ty)
	{	
		case Character::Type::exusiai:
			ch = new Exusiai(exusiai_templete);
			break;
		case Character::Type::muelsyse:
			ch = new Muelsyse(muelsyse_templete);
			break;
		case Character::Type::frostleaf:
			ch = new Frostleaf (frostleaf_templete);
			break;
		default:
			break;	
	}
	ch->init_current_animation();
	ch->set_position(Vector2((x_left + x_right) / 2, (y_bottom + y_top) / 2));
	
	if (gridtype == Gridtype::ground 
		&& ty!=Character::Type::muelsyse)	//允许在地面上的干员进行阻挡
		ch->set_can_block(true);			//缪尔赛斯不能阻挡	
}

void Gridcell::cleargrid()
{
	delete ch;
	occu = false;
}

void Gridcell::set_cell(int x, int yt, int yb, int h, int i, int width)
{
	//每张地图要单独设置y
	y_top = yt + i * h;
	x_left = x;
	x_right = x_left + width;
	y_bottom = yb + i * h;
}

void Gridcell::set_cell(int x, int yt, int yb, int width)
{
	//这个用于单独设置一行
	y_top = yt;
	x_left = x;
	x_right = x_left + width;
	y_bottom = yb;
}

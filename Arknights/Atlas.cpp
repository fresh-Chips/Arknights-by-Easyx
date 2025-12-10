#include "Atlas.h"

void Atlas::load_from_file(LPCTSTR path_template, int num)
{
	img_list.clear();	//使用前先清空
	img_list.resize(num);
	
	TCHAR buffer[128];	
	for (int i = 0; i < num; i++)
	{
		_stprintf_s(buffer, path_template, i + 1);//有问题的话注意i从0还是1开始
		IMAGE temp;

		loadimage(&temp, buffer);
		img_list[i] = new IMAGE(temp);
	}
}

#include "GameScene.h"

GameScene::GameScene()
{
	init_enemy_pool();
	//初始化地图函数装载到数组中，便于调用
	mapInital.push_back(nullptr);
	mapInital.push_back([this]() { this->initmap_1(); });
	mapInital.push_back([this]() { this->initmap_2(); });
	mapInital.push_back([this]() { this->initmap_3(); });
	//初始化按钮
	pause_button.button_init(Vector2(1130, 20), Vector2(1240, 120));
	pause_button.set_img(&img_button_pause, &img_button_continue);
	pause_button.set_callback([this]() {this->switch_pause(); });

	speed_button.button_init(Vector2(1010, 20), Vector2(1120, 120));
	speed_button.set_img(&img_button_speed1, &img_button_speed2);
	speed_button.set_callback([this]() {this->switch_speed(); });
	
	quit_button.button_init(Vector2(15, 20), Vector2(115, 100));
	quit_button.set_img(&img_button_quit);
	quit_button.set_callback([this]() {this->switch_quit(); });

	cost_button.button_init(Vector2(1110, 500), Vector2(1110, 500));
	cost_button.set_img(&img_cost_bg);
	cost_button.set_callback(nullptr);

	score_button.button_init(Vector2(440, 10), Vector2(440, 10));
	score_button.set_img(&img_scoreboard);
	score_button.set_callback(nullptr);

	button_list.push_back(&pause_button);
	button_list.push_back(&speed_button);
	button_list.push_back(&quit_button);
	button_list.push_back(&cost_button);
	button_list.push_back(&score_button);

	//设置费用数字的字体
	gettextstyle(&tc);
	tc.lfHeight = 60;
	tc.lfWeight = 120;
	strcpy_s(tc.lfFaceName, "Segoe UI Black");
	tc.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
	setbkmode(TRANSPARENT);		//透明背景
}

void GameScene::create_enemy(Enemy::EnemyType tp, int enter_num)
{
	if (enter_num > entrance_cnt)
	{
		std::cout << "获取了错误的入口编号" << std::endl;
		system("pause");
		exit(1);
	}

	int start_p, end_p, i;
	start_p = end_p = i = 0;
	switch (tp)
	{
	case Enemy::EnemyType::primebug:
		start_p = 0;
		end_p = 40;
		break;
	case Enemy::EnemyType::ritor:
		start_p = 40;
		end_p = 80;
		break;
	case Enemy::EnemyType::shield:
		start_p = 80;
		end_p = 120;
		break;
	case Enemy::EnemyType::avenger:
		start_p = 120;
		end_p = 150;
		break;

	default:
		break;
	}
	//定位到池子中最靠前的未上场敌人，让i停到没出场的敌人上
	//注意所有敌人初始都是未激活的
	for (i = start_p; i < end_p && enemypool[i]->get_is_active(); i++);
	//如果这个敌人未被激活，则i会停
	try{
		if (i == end_p && enemypool[i-1]->get_is_active()) //最后一个也被激活
		{
			throw"池子中的所有敌人已激活";
		}
	}
	catch (const char* err) {
		cout << "捕获到异常：" << err << endl;
		system("pause");
	}
	Enemy* temp = enemypool[i];
	if (temp == nullptr) 
	{
		cout << "生成敌人失败" << endl;
		/*system("pause");
		exit(1);*/
		return;
	}
	temp->copy_templete();	//复制一下数据
	temp->set_is_active(true);
	temp->init_current_animation();

	temp->set_enter_num(enter_num, cp_index[enter_num], cp_start[enter_num]);
	int x = checkpoint[cp_start[enter_num]].x;
	int y = checkpoint[cp_start[enter_num]].y;
	temp->init_position(Vector2(x,y));

	temp->renew_direction(checkpoint);
	temp->set_end_position(defence_target[enter_num]);//这里要改，不安全
	temp->set_target_distance(def_target_distance[enter_num]);
	active_enemy.push_back(temp);
}

void GameScene::on_enter()
{ 
	get_level_num();
	
	mapInital[level_num]();
	check_enemy_pool();

	is_quit = false;
	is_success = false;
	is_failed = false;
	is_pause = false;
	is_fast = false;
	choose = false;		//会有卡bug的可能所以重置一下
	game_time = 0;

	//自动作战设置指令
	if (is_auto_operate)
	{
		set_command();
	}
	//清空要输入的指令
	input_command.clear();
	//清空敌人波次的队列
	while (!enemy_wave.empty())
		enemy_wave.pop();
	//输入敌人波次的队列
	set_enemy_wave();
	//播放音乐
	switch (level_num)
	{
	case 1:
		mciSendString("play bgm_level_1 repeat from 0", NULL, 0, NULL);
		break;
	case 2:
		mciSendString("play bgm_level_2 repeat from 0", NULL, 0, NULL);
		break;
	case 3:
		mciSendString("play bgm_level_3 repeat from 0", NULL, 0, NULL);
		break;
	}
	
	test_emy.set_one_shot(false);
	test_emy.set_wait_time(3000);
	test_emy.set_callback([this](){this->create_enemy(Enemy::EnemyType::shield, 0); });
	
	enemy_debug.set_one_shot(false);
	enemy_debug.set_wait_time(2000);
	enemy_debug.set_callback([this]()
		{
			cout << "第一个" << endl;
			for (int i = 0; i < active_enemy.size(); i++)
			{
				cout << i << " " << active_enemy[i]->get_health() 
					<< " " << active_enemy[i]->get_distance() << endl;
			}
			cout << "最后一个" << endl << endl;
		});
}

void GameScene::on_update(int delta)
{
	int temp_delta = delta;		//有的功能不能暂停
	if (is_pause) delta = 0;	//最简单的暂停方式
	if (is_fast) delta *= 2;	//调二倍速
	//if (is_debug) test_emy.on_update(delta);	
	//更新运行时间
	game_time += delta;
	if (is_auto_operate && !player_command.empty())
		player_command.front().time_update(game_time, can_excute);
	
	if (!enemy_wave.empty())
		enemy_wave.front().time_update(game_time, can_create_enemy);
	//自动作战的输入，这里选择立即输入，如果在on_input中输入的话
	//会有延迟导致吞操作的可能性
	if (is_auto_operate && can_excute)
	{
		int i, j;
		player_command.front().execute(choose_tp, i, j);
		cost_total -= character_templete[(int)choose_tp]->get_cost();
		map_loc[i][j].create_character(choose_tp);
		active_ch.push_back(map_loc[i][j].get_character());
		active_ch.back()->set_gridcell_pos(i, j);

		choose_tp = Character::Type::invalid;
		player_command.pop();
		can_excute = false;
		std::cout << game_time << std::endl;
	}
	//创建敌人
	if (can_create_enemy)
	{
		can_create_enemy = false;
		Enemy::EnemyType tp;
		int num = 0;
		enemy_wave.front().execute(tp, num);
		enemy_wave.pop();	//队列的优势——无关索引，很安全
		create_enemy(tp, num);
	}
	//if (is_debug) enemy_debug.on_update(delta);
	//更新角色
	for (auto it = active_ch.begin(); it != active_ch.end(); it++)
	{
		if (!(*it)->get_is_active())
		{
			delete_ch(it);
			if (it != active_ch.end() - 1)
				it = active_ch.erase(it);
			else
			{
				active_ch.pop_back();	//避免了迭代器越界的问题
				break;		//不主动退出也会越界
			}
		}
		else
			(*it)->on_update(active_enemy, delta);
	}
	//更新敌人，使用迭代器是为了便于删除敌人
	for (auto it = active_enemy.begin(); it != active_enemy.end(); it++)
	{
		if ((*it)->get_health() <= 0)
			(*it)->set_is_active(false);
		if (!(*it)->get_is_active())	//处理被判定为退场的敌人
		{
			delete_enemy(it);
			if (it != active_enemy.end() - 1)
				it = active_enemy.erase(it);
			else
			{
				active_enemy.pop_back();
				break;
			}
		}
		else  (*it)->on_update(delta, checkpoint);
	}
	
	//将敌人按照到防守目标的距离升序排列，这样便于索敌
	sort(active_enemy.begin(), active_enemy.end()
		, [](Enemy* a, Enemy* b) {return *a < *b; });
	//这里先对指针解引用，再使用运算符重载进行比较
	
	//下面的删除方法更安全，但是遍历一遍不太划算
	/*auto new_end = remove_if(active_enemy.begin()
		, active_enemy.end()
		, [](Enemy* p) {return !p->get_is_active(); });
	active_enemy.erase(new_end, active_enemy.end());*/
	//更新费用增长
	cost_increase += delta;
	if (cost_increase > 1000)
	{
		cost_increase = 0;
		cost_total += 1;
	}
	if (cost_total >= 999)	//显示渲染前检查上限防止爆掉
		cost_total = 999;
	else if (cost_total < 0)
		cost_total = 0;

	//判断是否游戏结束
	if (player_health <= 0) 
	{
		player_health = 0;
		is_pause = true;
		is_failed = true;
	}
	else if (enemy_rest <= 0)
	{
		is_pause = true;
		is_success = true;
		enemy_rest = 0;
		if (img_success_x <= 200)
			img_success_x += 30;
		else
		{
			static int wait_time = 0;
			wait_time += temp_delta;
			if (wait_time > 1500)	//退出到选关界面
			{
				scene_manager.switch_to(SceneManager::SceneType::LevelSelector);
				wait_time = 0;
				img_success_x = -850;
			}
		}
	}
}

void GameScene::on_draw(const Camera& camera)
{
	const Vector2& pos_camera = camera.get_position();
	int px = pos_camera.x;
	int py = pos_camera.y;
	putimagePNG(0 - px, 0 - py, img_bg);
	//绘制底部头像
	for (int i = 0; i < ch_count; i++) {	//要改干员数量，这里是在生成下面的头像
		putimagePNG(1160 - i * 100 - px, 580 - py, img_head[i]);
	}
	//绘制敌人动画
	for (auto it : active_enemy) 
	{
		it->on_darw(camera);
	}
	//绘制遮挡敌人入口
	if (level_num == 1) putimage(1097, 260, &img_entry_block);
	//绘制干员动画
	for (auto it : active_ch)
	{
		it->on_darw(camera);
	}
	//绘制按钮
	for (int i = 0; i < button_list.size(); i++)
	{
		button_list[i]->on_draw();
	}
	//绘制费用条，实际上这种写法并不安全，好在没有更特殊的需求了
	setfillcolor(WHITE);
	solidrectangle(1110, 560, 1110 + cost_increase * 1.5 / 10, 565);
	
	//绘制文字
	settextstyle(&tc);
	char costtext[4];
	sprintf_s(costtext, sizeof(costtext), "%d", cost_total);
	settextcolor(WHITE);
	outtextxy(1175, 500, costtext);
	
	settextstyle(30, 0, "宋体");
	char number[4];
	sprintf_s(number, sizeof(number), "%d", enemy_rest);
	outtextxy(540, 17, number);
	sprintf_s(number, sizeof(number), "%d", enemy_total);
	outtextxy(590, 17, number);
	settextcolor(RGB(255, 120, 120));
	sprintf_s(number, sizeof(number), "%d", player_health);
	outtextxy(750, 17, number);
	//绘制拖动图片
	if (choose)
	{
		int idx = static_cast<int> (choose_tp);
		putimagePNG(now_x - img_drag[idx]->getwidth() / 2 - px,
			now_y - img_drag[idx]->getheight() / 2 - py, img_drag[idx]);
	}
	//绘制自动作战提示
	if(is_auto_operate) 
	{
		putimagePNG(0, 584, &img_autotips_board);
		putimagePNG(494, 622, &img_autotips);
	}
	//单独绘制退出界面
	if (is_quit) putimagePNG(0, 0, &img_quitgame);
	//绘制胜利界面
	if (is_success) putimagePNG(img_success_x, 255, img_success);
	else if (is_failed) putimagePNG(0, 0, &img_failed);
}

void GameScene::on_input(const ExMessage& msg)
{
	if (msg.message == WM_LBUTTONDOWN 
		&& !is_pause &&!is_auto_operate)
	{
		now_x = msg.x;		//减少变量该更新频率，用不着时不更新
		now_y = msg.y;
		if (msg.x < 1260 && msg.x > 1260 - 100 * ch_count && msg.y > 580)
		{
			int index = (1260 - msg.x) / 100;	//多个人物的序号计算
			
			if (cost_total >= character_templete[index]->get_cost())
			{	//费用够才能下
				choose = true;		//切换拖动状态
				choose_tp = static_cast<Character::Type> (index);
			}
		}
	}
	else if (choose && msg.message == WM_MOUSEMOVE) 
	{
		now_x = msg.x;
		now_y = msg.y;
	}
	else if (msg.message == WM_LBUTTONUP) 
	{
		if (choose && !is_pause && !is_quit
			&& !is_auto_operate) //暂停时不能部署，未开启自动作战时保存指令
		{
			bool is_find = false;
			int ni, nj;		//用于函数内传递地块的数组坐标
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < column; j++)
				{
					if (map_loc[i][j].findlocation(now_x, now_y))
					{
						is_find = true;
						//map_loc[i][j].set_occupy(true);创建函数中写过了
						ni = i;
						nj = j;
						break;
					}
				}
			}
			if (is_find && choose_tp != Character::Type::muelsyse)
			{
				if (map_loc[ni][nj].get_type() == Gridcell::Gridtype::waterpool)
					is_find = false;	//muelsyse可以放在水上
			}
			if (is_find)
			{
				cost_total -= character_templete[(int)choose_tp]->get_cost();
				map_loc[ni][nj].create_character(choose_tp);
				active_ch.push_back(map_loc[ni][nj].get_character());
				active_ch.back()->set_gridcell_pos(ni, nj);
				input_command.push_back(Command(choose_tp, ni, nj, game_time));
			}
			choose = false;
			choose_tp = Character::Type::invalid;
		}
		else if (!is_failed && !is_quit)
		{
			for (int i = 0; i < button_list.size(); i++)
			{
				button_list[i]->on_input(msg);
			}
		}
		else if (!is_failed && is_quit)	//退出视为失败
		{
			//这里逻辑略复杂一些，退出确认界面不用单独写成按钮类
			if (msg.y> 536 && msg.y < 612)	//两个按钮同高度
			{
				if (msg.x < 640)
				{
					switch_quit();				
				}					
				else
					is_failed = true;
			}
		}
		else if (is_failed)	//失败后点击返回主菜单
			scene_manager.switch_to(SceneManager::SceneType::Menu);
	}

	//if (is_auto_operate && can_excute)//自动作战的输入
	//{
	//	int i, j;
	//	player_command.front().execute(choose_tp, i, j);
	//	cost_total -= character_templete[(int)choose_tp]->get_cost();
	//	map_loc[i][j].create_character(choose_tp);
	//	active_ch.push_back(map_loc[i][j].get_character());
	//	active_ch.back()->set_gridcell_pos(i, j);

	//	choose_tp = Character::Type::invalid;
	//	player_command.pop();
	//	can_excute = false;
	//	std::cout << game_time << std::endl;
	//}
	if (is_debug)	//调试模式下的响应
	{
		if (msg.message == WM_KEYUP)	//防止一直按着一直输出
			switch (msg.vkcode)
			{
			case 0x43:
				cost_total += 50;		//调试模式下按C加50费
				if (cost_total >= 999)	//显示渲染前检查上限防止爆掉
					cost_total = 999;
				break;
			case 0x51:
				is_show_aimrange = !is_show_aimrange;	//按Q键切换打开和关闭
				break;
			case 0x52:
				cout << "剩余敌人数量：" << enemy_rest << endl;		
				//按R键显示剩余敌人数量
				break;
			case 0x53:
				for (auto it : active_enemy)
				{
					cout << it->get_distance() << endl;
				}
				//按S键显示敌人按距离升序排列
				break;
			default:
				break;
			}
	}
}

void GameScene::on_exit()
{
	clear_character();
	clear_enemy();
	clear_map_grid();
	checkpoint.clear();
	//重置按钮
	for (int i = 0; i < button_list.size(); i++)
	{
		button_list[i]->reset();
	}
	//如果胜利就写入本局操作，注意先清空文件
	if (is_success && !is_auto_operate)
	{
		char buffer[64];
		sprintf_s(buffer, "res/mapset/command%d.txt", level_num);
		ofstream file(buffer);
		if (!file.is_open())
		{
			std::cerr << "无法打开文件！" << std::endl;
			system("pause");
			exit(1);
		}

		for (auto& it : input_command)
		{
			it.write_in_file(file);
		}
		file.close();
	}
	//停止音乐
	switch (level_num)
	{
	case 1:
		mciSendString("stop bgm_level_1", NULL, 0, NULL);
		break;
	case 2:
		mciSendString("stop bgm_level_2", NULL, 0, NULL);
		break;
	case 3:
		mciSendString("stop bgm_level_3", NULL, 0, NULL);
		break;
	}
	//播放主菜单音乐
	mciSendString("seek bgm_menu to start", NULL, 0, NULL);
	mciSendString("play bgm_menu repeat", NULL, 0, NULL);
}

inline void GameScene::get_level_num()
{
	level_num = Scene::level_num;
	is_auto_operate = Scene::is_auto;
}

void GameScene::init_enemy_pool()
{
	enemypool.resize(150);
	int i = 0;
	for (; i < 40; i++) 
	{
		enemypool[i] = new Primebug(primebug_templete);	
	}
	for (; i < 80; i++) 
	{
		enemypool[i] = new Ritor(ritor_templete);	
	}
	for (; i < 120; i++) 
	{
		enemypool[i] = new Shield(shield_templete);	
	}
	for (; i < 150; i++) 
	{
		enemypool[i] = new Avenger(avenger_templete);
	}
}

void GameScene::clear_character()
{
	Character::reset_snd_count();	//清空句柄计数防止泄露，但是这个做法不够精细
	for (auto ch : active_ch) 
	{
		delete ch;
	}
	active_ch.clear();
}

void GameScene::clear_enemy()
{
	for (auto it : active_enemy)
	{
		it->set_is_active(false);//不要释放，否则池子会被破坏
	}
	active_enemy.clear();
}

void GameScene::clear_map_grid()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			if (map_loc[i][j].is_occupy())
				map_loc[i][j].set_occupy(false);
		}
	}
}

void GameScene::delete_enemy(vector<Enemy*>::iterator& it)
{
	enemy_rest--;
	if ((*it)->get_is_agress())
	{
		player_health -= (*it)->get_target_value();
		main_camera.shake(10, 350);		//有敌人进点就抖动一下
	}

	//不删除删除敌人的堆内存，只让其失效并返回池子	
	//这里迭代器绝对不能置空
}

void GameScene::delete_ch(vector<Character*>::iterator& it)
{
	int i, j;
	(*it)->get_gridcell_pos(i, j);
	map_loc[i][j].cleargrid();	//这里已经delete了，不能再来一次
	//出去后立即更新迭代器
}

void GameScene::check_enemy_pool()
{
	for (int i = 0; i < enemypool.size(); i++)
	{
		if (enemypool[i] == NULL)
		{
			cout << "池子已损坏，下标为：" << i << endl;
			system("pause");
			exit(1);
		}
	}
}

void GameScene::initmap_1()
{
	img_bg = &img_level1_map;
	row = 4;
	column = 7;
	map_loc.clear();		//清空以便初始化
	map_loc.resize(row);          // 先重置行数
	for (auto& row : map_loc)
		row.resize(column);      // 再重置每行的列数

#pragma region{文件数据初始化}	
	ifstream file("res/mapset/1.txt");
	if (!file.is_open()) {
		cerr << "错误：无法打开文件 numbers.txt" << endl;
		system("pause");
		exit(1);
	}
	string input;
	for (int i = 0; i < row; i++)
	{
		getline(file, input);
		stringstream ss(input);
		int x = 0;
		for (int j = 0; j < column; j++)
		{
			ss >> x;
			map_loc[i][j].set_cell(x, 150, 240, 90, i);
			map_loc[i][j].set_occupy(false);
		}
	}
	//单独设置其他格子类型
	for (int j = 0; j < column; j++)
	{
		map_loc[0][j].set_type(Gridcell::Gridtype::platform);
		map_loc[3][j].set_type(Gridcell::Gridtype::platform);
	}
	map_loc[1][1].set_type(Gridcell::Gridtype::platform);
	map_loc[2][3].set_type(Gridcell::Gridtype::platform);
	//设置检查点
	int chp_num = 0;
	cp_start.clear();
	cp_index.clear();
	cp_start.push_back(0);
	while (getline(file, input))
	{
		if (input.empty()) continue;
		if (input.find("line break") != std::string::npos)
		{
			cp_index.push_back(chp_num - 1);	//后面加的1，这里返回减1
			cp_start.push_back(chp_num);		//上一个结尾的下一个为起点
			continue;
		}
		stringstream ss(input);
		int f;
		Enemy::CheckPoint t;
		ss >> t.x >> t.y >> f;
		t.forward = static_cast<Enemy::Direction>(f);
		t.num = chp_num;
		checkpoint.push_back(t);
		chp_num++;
	}
	cp_index.push_back(chp_num - 1);
	file.close();
#pragma endregion

	cost_total = 20;		//初始化费用
	player_health = 10;
	enemy_total = 10;
	enemy_rest = enemy_total;
	entrance_cnt = 1;

	defence_target.clear();
	defence_target.push_back(Vector2(205, 280));//单独设置

	def_target_distance.clear();
	def_target_distance.push_back(1300);
}

void GameScene::initmap_2()
{
	img_bg = &img_level2_map;
	row = 5;
	column = 8;
	map_loc.clear();
	map_loc.resize(row);          // 先重置行数
	for (auto& row : map_loc)
		row.resize(column);      // 再重置每行的列数

#pragma region{文件数据初始化}	
	ifstream file("res/mapset/2.txt");
	if (!file.is_open()) {
		cerr << "错误：无法打开文件 numbers.txt" << endl;
		system("pause");
		exit(1);
	}
	
	string input;
	for (int i = 0; i < row; i++)
	{
		getline(file, input);
		stringstream ss(input);
		int x = 0;
		if (i < 2)
		{
			for (int j = 0; j < column; j++)
			{		
				ss >> x;
				map_loc[i][j].set_cell(x, 120, 185, 85, i);
				map_loc[i][j].set_occupy(false);
			}
		}
		else
		{
			for (int j = 0; j < column; j++)
			{
				ss >> x;
				map_loc[i][j].set_cell(x, 115, 205, 90, i);
				map_loc[i][j].set_occupy(false);
			}
		}
	}
	//单独设置其他格子类型
	for (int j = 4; j < column; j++)
	{
		map_loc[1][j].set_type(Gridcell::Gridtype::platform);
		map_loc[3][j].set_type(Gridcell::Gridtype::platform);
	}
	for (int j = 0; j < 3; j++)
	{
		map_loc[0][j].set_type(Gridcell::Gridtype::platform);
		map_loc[4][j].set_type(Gridcell::Gridtype::platform);
	}
	map_loc[1][0].set_type(Gridcell::Gridtype::platform);
	map_loc[3][0].set_type(Gridcell::Gridtype::platform);
	//设置检查点
	int chp_num = 0;
	cp_start.clear();
	cp_index.clear();
	cp_start.push_back(0);
	while (getline(file, input))
	{
		if (input.empty()) continue;
		if (input.find("line break") != std::string::npos)
		{
			cp_index.push_back(chp_num - 1);	//后面加的1，这里返回减1
			cp_start.push_back(chp_num);		//上一个结尾的下一个为起点
			continue;
		}
		stringstream ss(input);
		int f;
		Enemy::CheckPoint t;
		ss >> t.x >> t.y >> f;
		t.forward = static_cast<Enemy::Direction>(f);
		t.num = chp_num;
		checkpoint.push_back(t);
		chp_num++;
	}
	cp_index.push_back(chp_num - 1);
	file.close();
#pragma endregion

	defence_target.clear();
	defence_target.push_back(Vector2(95, 285));
	defence_target.push_back(Vector2(95, 285));
	defence_target.push_back(Vector2(95, 285));

	def_target_distance.clear();
	def_target_distance.push_back(1125);
	def_target_distance.push_back(1250);
	def_target_distance.push_back(1190);

	cost_total = 20;
	player_health = 3;
	enemy_total = 20;
	enemy_rest = enemy_total;
	entrance_cnt = 3;
}

void GameScene::initmap_3()
{
	img_bg = &img_level3_map;
	row = 6;
	column = 9;
	map_loc.clear();
	map_loc.resize(row);          // 先重置行数
	for (auto& row : map_loc)
		row.resize(column);      // 再重置每行的列数

#pragma region{文件数据初始化}	
	ifstream file("res/mapset/3.txt");
	if (!file.is_open()) {
		cerr << "错误：无法打开文件 numbers.txt" << endl;
		system("pause");
		exit(1);
	}

	string input;
	for (int i = 0; i < row; i++)
	{
		getline(file, input);
		stringstream ss(input);
		int x = 0;
		if (i <= 2)
		{
			for (int j = 0; j < column; j++)
			{
				ss >> x;
				map_loc[i][j].set_cell(x, 114, 178, 64, i, 85);
				map_loc[i][j].set_occupy(false);
			}
		}
		else
		{
			for (int j = 0; j < column; j++)
			{
				ss >> x;
				map_loc[i][j].set_cell(x, 337, 422, 85, i - 3, 85);
				map_loc[i][j].set_occupy(false);
			}
		}
	}
	//单独设置其他格子类型
	for (int i = 7; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			map_loc[i][j].set_type(Gridcell::Gridtype::deploy_ban);
		}
	}
	for (int i = 7; i < row; i++)
	{
		for (int j = 2; j < 4; j++)
		{
			map_loc[i][j].set_type(Gridcell::Gridtype::waterpool);
		}
	}
	
	//设置检查点
	int chp_num = 0;
	cp_start.clear();
	cp_index.clear();
	cp_start.push_back(0);
	while (getline(file, input))
	{
		if (input.empty()) continue;
		if (input.find("line break") != std::string::npos)
		{
			cp_index.push_back(chp_num - 1);	//后面加的1，这里返回减1
			cp_start.push_back(chp_num);		//上一个结尾的下一个为起点
			continue;
		}
		stringstream ss(input);
		int f;
		Enemy::CheckPoint t;
		ss >> t.x >> t.y >> f;
		t.forward = static_cast<Enemy::Direction>(f);
		t.num = chp_num;
		checkpoint.push_back(t);
		chp_num++;
	}
	cp_index.push_back(chp_num - 1);
	file.close();
#pragma endregion

	defence_target.clear();
	defence_target.push_back(Vector2(211, 142));
	defence_target.push_back(Vector2(205, 204));
	defence_target.push_back(Vector2(182, 288));
	defence_target.push_back(Vector2(161, 354));
	defence_target.push_back(Vector2(135, 451));
	defence_target.push_back(Vector2(108, 548));

	def_target_distance.clear();
	def_target_distance.push_back(1075);
	def_target_distance.push_back(1081);
	def_target_distance.push_back(1104);
	def_target_distance.push_back(1125);
	def_target_distance.push_back(1151);
	def_target_distance.push_back(1178);

	cost_total = 20;
	player_health = 3;
	enemy_total = 20;
	enemy_rest = enemy_total;
	entrance_cnt = 6;
}

void GameScene::set_command()
{
	while (!player_command.empty())
		player_command.pop();	//先清空队列

	char buffer[64];
	sprintf_s(buffer, "res/mapset/command%d.txt", level_num);
	ifstream file(buffer);
	if (!file.is_open())
	{
		std::cerr << "无法打开文件！" << std::endl;
		system("pause");
		exit(1);
	}

	string input;
	while (getline(file, input))
	{
		if (file.eof()) break;
		Command temp;
		temp.read_from_file(input);
		player_command.push(temp);
	}
}

void GameScene::set_enemy_wave()
{
	char buffer[64];
	sprintf_s(buffer, "res/mapset/wave%d.txt", level_num);
	ifstream file(buffer);
	if (!file.is_open())
	{
		std::cerr << "无法打开文件！" << std::endl;
		system("pause");
		exit(1);
	}

	string input;
	getline(file, input);		//先释放文件第一行提示词
	enemy_total = 0;			//就不用手动设置了
	while (getline(file, input))
	{
		if (input == "end") break;	//手写文件，结尾给出end提示
		EnemyWave temp;
		temp.read_from_file(input);
		enemy_wave.push(temp);
		enemy_total++;
	}
	enemy_rest = enemy_total;
}

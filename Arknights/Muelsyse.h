#pragma once

#include "Character.h"

extern Atlas atlas_muelsyse_idle_right;
extern Atlas atlas_muelsyse_idle_left;
extern Atlas atlas_muelsyse_atk_right;
extern Atlas atlas_muelsyse_atk_left;
extern int cost_total;

class Muelsyse: public Character
{
public:
	Muelsyse();
	~Muelsyse() = default;
	Muelsyse(const Muelsyse& p) = default;

	//void on_update(const std::vector<Enemy*>& active_enemy, int delta) override;
	void set_animation() override;
	void skill(int delta)override;

private:
	void produce_cost();
};


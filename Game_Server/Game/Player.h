#pragma once
#include "ActiveRequests.h"

struct PlayerInfo
{
	int materials;
	int prod;
	int money;
	int factories;
	
	int used_materials;
	int used_prod;
	int used_money;
	int used_factories;

	int month_til_payment;
	
	bool has_ended_turn;
	ActiveRequest requests[4];
	bool is_active;

	PlayerInfo() : materials(4), prod(2), money(10000), factories(2),
					used_prod(0), used_money(0), used_factories(0), used_materials(0),
					month_til_payment(-1),
				has_ended_turn(false), is_active(false) {}
};
#pragma once
#include <cstdlib>

struct AuctionState
{
	double material_coeff;
	int material_min_price;
	double prod_coeff;
	int prod_max_price;
};

class Auction
{
	static int level;
	static double material_coeff[5];
	static int material_min_price[5];
	static double prod_coeff[5];
	static int prod_max_price[5];

public:
	static AuctionState Reroll()
	{
		level = rand() % 5;

		return AuctionState{ material_coeff[level], material_min_price[level],
			prod_coeff[level], prod_max_price[level] };
	}
};

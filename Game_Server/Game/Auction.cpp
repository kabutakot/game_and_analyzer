#include "Auction.h"

int    Auction::level;
double Auction::material_coeff[] = {1.0, 1.5, 2.0, 2.5, 3.0};
int    Auction::material_min_price[] = {800, 650, 500, 400, 300};
double Auction::prod_coeff[] = {3.0, 2.5, 2.0, 1.5, 1.0};
int    Auction::prod_max_price[] = {6500, 6000, 5500, 5000, 4500};
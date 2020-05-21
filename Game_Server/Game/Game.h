#pragma once
#include "Auction.h"
#include "Player.h"

const int kMaxPlayers = 10;

class Game
{
	AuctionState _auc_state;
	int _amount;
	PlayerInfo _player_info[kMaxPlayers];
	
public:
	Game(int amount_of_players) : _amount(amount_of_players) {}

	void Activate(int id) { _player_info[id].is_active = true; }
	void Deactivate(int id) { _player_info[id].is_active = false; }
	void AddRequest(int id, ActiveRequest req) { _player_info[id].requests[req.Type()] = req; }
};

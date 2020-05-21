#pragma once
#include <cstdint>

enum MessageType : uint16_t { AUCTION_INFO_R, PLAYER_INFO_R, ACCEPT, ERROR };
enum CommandType : uint16_t { AUCTION_MATERIALS, AUCTION_PROD, PRODUCE, BUILD, END_TURN, AUCTION_INFO, PLAYER_INFO };

#pragma pack(push, 1)
struct DataHeader
{
	MessageType type; //DATA
	uint16_t length;
};
#pragma pack(pop)


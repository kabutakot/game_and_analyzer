#pragma once

enum RequestType {SELL_PROD, BUY_MATERIALS, PRODUCE, BUILD};
class ActiveRequest
{
	RequestType _type;
	int _amount;
public:
	ActiveRequest(RequestType type, int amount) : _type(type), _amount(amount) {}
	int Amount() { return _amount; }
	RequestType Type() { return _type; }
};
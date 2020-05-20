#pragma once
#include "RpnItems.h"
#include "Shared/MyVector.h"

class Invoker
{
public:
	static RpnOperand* info (RpnOperand** params, int amount);
	static RpnOperand* turn (RpnOperand** params, int amount);
	static RpnOperand* player (RpnOperand** params, int amount);
	static RpnOperand* prod (RpnOperand** params, int amount);
	static RpnOperand* build (RpnOperand** params, int amount);
	static RpnOperand* buy (RpnOperand** params, int amount);
	static RpnOperand* sell (RpnOperand** params, int amount);
	static RpnOperand* print (RpnOperand** params, int amount);
	static RpnOperand* assign (RpnOperand** params, int amount);
	static RpnOperand* greater (RpnOperand** params, int amount);
	static RpnOperand* less (RpnOperand** params, int amount);
	static RpnOperand* plus (RpnOperand** params, int amount);
	static RpnOperand* minus (RpnOperand** params, int amount);
	static RpnOperand* multiply (RpnOperand** params, int amount);
	static RpnOperand* divide (RpnOperand** params, int amount);
	static RpnOperand* logic_and (RpnOperand** params, int amount);
	static RpnOperand* logic_or (RpnOperand** params, int amount);
	static RpnOperand* unary_plus (RpnOperand** params, int amount);
	static RpnOperand* unary_minus (RpnOperand** params, int amount);
	static RpnOperand* logic_not (RpnOperand** params, int amount);
};
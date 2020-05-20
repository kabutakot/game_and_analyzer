#include "Invoker.h"

#include <cassert>
#include <cstdio>

#include "ErrorException.h"

//
//void del (RpnOperand** params, int amount)
//{
//	for (int i = 0; i < params.size(); i++)
//	{
//		delete params[i];
//	}
//}

RpnOperand* Invoker::info(RpnOperand** params, int amount)
{
	return new RpnConstant(0);
}

RpnOperand* Invoker::turn(RpnOperand** params, int amount)
{
	return new RpnConstant(0);
}

RpnOperand* Invoker::player(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue());	
}

RpnOperand* Invoker::prod(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue());
}

RpnOperand* Invoker::build(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue());
}

RpnOperand* Invoker::buy(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() + params[1]->GetValue());
}

RpnOperand* Invoker::sell(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() + params[1]->GetValue());
}

RpnOperand* Invoker::print(RpnOperand** params, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		if (params[i]->GetType() == LITERAL)
		{
			RpnLiteral* pli = static_cast<RpnLiteral*>(params[i]);
			printf("%s", pli->GetStringValue());
		}
		else
		{
			printf("%d", params[i]->GetValue());
		}

		printf(" ");
	}
	printf("\n");
	return NULL;
}

RpnOperand* Invoker::assign(RpnOperand** params, int amount)
{
	if (params[0]->GetType() != VARIABLE)
		throw "Invoker::assign Wrong type of RpnOperand";
	
	RpnVar* pv0 = static_cast<RpnVar*>(params[0]);

	pv0->GetAssignValue() = params[1]->GetValue();

	return NULL;
}

RpnOperand* Invoker::greater(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() > params[1]->GetValue());
}

RpnOperand* Invoker::less(RpnOperand** params, int amount)
{

	return new RpnConstant(params[0]->GetValue() < params[1]->GetValue());
}

RpnOperand* Invoker::plus(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() + params[1]->GetValue());
}

RpnOperand* Invoker::minus(RpnOperand** params, int amount)
{

	return new RpnConstant(params[0]->GetValue() - params[1]->GetValue());
}

RpnOperand* Invoker::multiply(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() * params[1]->GetValue());
}

RpnOperand* Invoker::divide(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() / params[1]->GetValue());
}

RpnOperand* Invoker::logic_and(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() && params[1]->GetValue());
}

RpnOperand* Invoker::logic_or(RpnOperand** params, int amount)
{
	return new RpnConstant(params[0]->GetValue() || params[1]->GetValue());
}

RpnOperand* Invoker::unary_plus(RpnOperand** params, int amount)
{
	return new RpnConstant(abs(params[0]->GetValue()));
}

RpnOperand* Invoker::unary_minus(RpnOperand** params, int amount)
{
	return new RpnConstant(-abs(params[0]->GetValue()));
}

RpnOperand* Invoker::logic_not(RpnOperand** params, int amount)
{
	return new RpnConstant(!params[0]->GetValue());
}

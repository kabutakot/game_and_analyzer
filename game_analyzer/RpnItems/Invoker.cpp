#include "Invoker.h"

#include <cassert>
#include <cstdio>

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
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);
	
	if (pv0 != NULL)
		return new RpnConstant(pv0->GetValue());
	if (pc0 != NULL)
		return new RpnConstant(pc0->GetValue());

	return NULL;
	
}

RpnOperand* Invoker::prod(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	if (pv0 != NULL)
		return new RpnConstant(pv0->GetValue());
	if (pc0 != NULL)
		return new RpnConstant(pc0->GetValue());

	return NULL;
}

RpnOperand* Invoker::build(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	if (pv0 != NULL)
		return new RpnConstant(pv0->GetValue());
	if (pc0 != NULL)
		return new RpnConstant(pc0->GetValue());

	return NULL;
}

RpnOperand* Invoker::buy(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);
	
	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::sell(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::print(RpnOperand** params, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		RpnConstant* pci = dynamic_cast<RpnConstant*>(params[i]);
		RpnVar* pvi = dynamic_cast<RpnVar*>(params[i]);
		RpnLiteral* pli = dynamic_cast<RpnLiteral*>(params[i]);

		if (pci != NULL)
			printf("%d", pci->GetValue());
		if (pvi != NULL)
			printf("%d", pvi->GetValue());
		if (pli != NULL)
			printf("%s", pli->GetValue());

		printf(" ");
	}
	printf("\n");
	return NULL;
}

RpnOperand* Invoker::assign(RpnOperand** params, int amount)
{
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);
	assert(pv0 != NULL);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);


	if (pc1 != NULL)
	{
		pv0->GetValue() = pc1->GetValue();
	}

	if (pv1 != NULL)
	{
		pv0->GetValue() = pv1->GetValue();
	}


	return NULL;
}

RpnOperand* Invoker::greater(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() > pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() > pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() > pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() > pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::less(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() < pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() < pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() < pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() < pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::plus(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() + pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() + pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::minus(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() - pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() - pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() - pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() - pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::multiply(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() * pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() * pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() * pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() * pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::divide(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() * pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() * pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() * pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() * pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::logic_and(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() && pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() && pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() && pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() && pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::logic_or(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	RpnConstant* pc1 = dynamic_cast<RpnConstant*>(params[1]);
	RpnVar* pv1 = dynamic_cast<RpnVar*>(params[1]);

	if (pc0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() || pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pc0->GetValue() || pv1->GetValue());
		}
	}

	if (pv0 != NULL)
	{
		if (pc1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() || pc1->GetValue());
		}

		if (pv1 != NULL)
		{
			return new RpnConstant(pv0->GetValue() || pv1->GetValue());
		}
	}

	return NULL;
}

RpnOperand* Invoker::unary_plus(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	if (pv0 != NULL)
		return new RpnConstant(abs(pv0->GetValue()));
	if (pc0 != NULL)
		return new RpnConstant(abs(pc0->GetValue()));

	return NULL;
}

RpnOperand* Invoker::unary_minus(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	if (pv0 != NULL)
		return new RpnConstant(-abs(pv0->GetValue()));
	if (pc0 != NULL)
		return new RpnConstant(-abs(pc0->GetValue()));

	return NULL;
}

RpnOperand* Invoker::logic_not(RpnOperand** params, int amount)
{
	RpnConstant* pc0 = dynamic_cast<RpnConstant*>(params[0]);
	RpnVar* pv0 = dynamic_cast<RpnVar*>(params[0]);

	if (pv0 != NULL)
		return new RpnConstant(!(pv0->GetValue()));
	if (pc0 != NULL)
		return new RpnConstant(!(pc0->GetValue()));

	return NULL;
}

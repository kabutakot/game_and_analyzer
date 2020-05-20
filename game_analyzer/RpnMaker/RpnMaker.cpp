#include "RpnMaker/RpnMaker.h"

void RpnMaker::ExecuteRpn()
{
	for (int i = 0; i <_rpn_items_amount; i++)
	{
		RpnItem* item = _rpn_items[i];
		switch (item->GetType())
		{
		case VARIABLE:
			{
				RpnVar* var = dynamic_cast<RpnVar*>(item);
				_operand_stack[_operand_stack_index++] = new RpnVar(var->GetValue(), "stack");
				break;
			}
		case CONSTANT:
			{
				RpnConstant* var = dynamic_cast<RpnConstant*>(item);
				_operand_stack[_operand_stack_index++] = new RpnConstant(var->GetValue());
				break;
			}
		case LITERAL:
			{
				RpnLiteral* var = dynamic_cast<RpnLiteral*>(item);
				_operand_stack[_operand_stack_index++] = new RpnLiteral(var->GetValue());
				break;
			}
		case OPERATION:
			{
				RpnOperation* var = dynamic_cast<RpnOperation*>(item);
				RpnOperand* result = var->Invoke(&_operand_stack[_operand_stack_index - var->GetAmount()], var->GetAmount());

				for (int j = 1; j < var->GetAmount(); j++)
				{
					delete _operand_stack[_operand_stack_index - j];
				}
				_operand_stack_index -= var->GetAmount();
				if (var->HasResult())
				{
					_operand_stack[_operand_stack_index++] = result;
				}
				break;
			}
		case GOTO:
			{
				RpnGoto* var = dynamic_cast<RpnGoto*>(item);
				if (!var->GetIfFalse())
				{
					RpnConstant* constant = dynamic_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
					i = constant->GetValue();
					delete constant;
				}
				else
				{
					RpnConstant* cond = dynamic_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
					RpnConstant* to_where = dynamic_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
					if (!cond->GetValue())
					{
						i = to_where->GetValue();
					}
					delete cond;
					delete to_where;
				}
				break;
			}
		}
		//PrintOperandStack();
	}
}
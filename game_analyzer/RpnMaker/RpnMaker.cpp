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
				RpnVar* var = static_cast<RpnVar*>(item);
				_operand_stack[_operand_stack_index++] = new RpnVar(var->GetAssignValue(), var->GetLabel());
				break;
			}
		case CONSTANT:
			{
				RpnConstant* var = static_cast<RpnConstant*>(item);
				_operand_stack[_operand_stack_index++] = new RpnConstant(var->GetValue());
				break;
			}
		case LITERAL:
			{
				RpnLiteral* var = static_cast<RpnLiteral*>(item);
				_operand_stack[_operand_stack_index++] = new RpnLiteral(var->GetStringValue());
				break;
			}
		case OPERATION:
			{
				RpnOperation* var = static_cast<RpnOperation*>(item);
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
				RpnGoto* var = static_cast<RpnGoto*>(item);
				if (!var->IfFalse())
				{
					RpnConstant* constant = static_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
					i = constant->GetValue();
					delete constant;
				}
				else
				{
					RpnConstant* cond = static_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
					RpnConstant* to_where = static_cast<RpnConstant*>(_operand_stack[--_operand_stack_index]);
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
#pragma once
#include "RpnItems/RpnItems.h"
#include "SyntaxAnalyzer/VarTable.h"


class RpnMaker
{
	RpnItem* _rpn_items[800];
	int _rpn_items_amount;
	int _rpn_items_index;

	RpnOperand* _operand_stack[150];
	int _operand_stack_index;

public:
	RpnMaker() : _operand_stack_index(0) {};


	void SetRpnItems(RpnItem** rpn_items, int amount)
	{
		memcpy(_rpn_items, rpn_items, amount * sizeof(RpnItem*));
		_rpn_items_amount = amount;
	}
	void PrintRpn()
	{
		for (int i = 0; i < _rpn_items_amount; i++)
		{
			_rpn_items[i]->Print();
			printf(" ");
		}
		printf("\n");

		for (int i = 0; i < VarTable::_var_table_index; i++)
		{
			printf("%d\n", VarTable::_var_table[i]);
		}
		printf("\n");
	}

	void PrintOperandStack()
	{
		for (int i =0; i < _operand_stack_index; i++)
		{
			RpnOperand* item = _operand_stack[i];
			switch (item->GetType())
			{
			case VARIABLE:
			{
				RpnVar* var = dynamic_cast<RpnVar*>(item);
				printf("var %d\n", var->GetValue());
				break;
			}
			case CONSTANT:
			{
				RpnConstant* var = dynamic_cast<RpnConstant*>(item);
				printf("const %d\n", var->GetValue());
				break;
			}
			case LITERAL:
			{
				RpnLiteral* var = dynamic_cast<RpnLiteral*>(item);
				printf("literal %s", var->GetValue());
				break;
			}
			}
		}
		printf("================\n");
	}
	void ExecuteRpn();

	~RpnMaker()
	{
		for (int i = 0; i < _rpn_items_amount; i++)
		{
			delete _rpn_items[i];
		}

		for (int i = 0; i < _operand_stack_index; i++)
		{
			delete _operand_stack[i];
		}
	}
};

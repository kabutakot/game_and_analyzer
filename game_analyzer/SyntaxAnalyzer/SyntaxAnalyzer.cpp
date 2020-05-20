
#include "SyntaxAnalyzer.h"
//#include <cstring>


const char * const SyntaxAnalyzer::_control_keywords[] = { "if", "while" };
const char * const SyntaxAnalyzer::_functions_0p[] = { "info", "turn" };
const char * const SyntaxAnalyzer::_functions_1p[] = { "player", "prod", "build" };
const char * const SyntaxAnalyzer::_functions_2p[] = { "buy", "sell" };
const char * SyntaxAnalyzer::_function_print = "print";
const char * const SyntaxAnalyzer::_assign[] = { "=" };
const char * const SyntaxAnalyzer::_binary_operators[] = { ">", "<", "+", "-", "*", "/", "&", "|" };
const char * const SyntaxAnalyzer::_unary_operators[] = { "+", "-", "!" };


const Operation SyntaxAnalyzer::_functions_0p_op[] = {
	{_functions_0p[0], Invoker::info, 6}, {_functions_0p[1], Invoker::turn, 6} };
const Operation SyntaxAnalyzer::_functions_1p_op[] = {
	{_functions_1p[0], Invoker::player, 6},{_functions_1p[1], Invoker::prod, 6},{_functions_1p[2], Invoker::build, 6}
};
const Operation SyntaxAnalyzer::_functions_2p_op[] = {
	{_functions_2p[0], Invoker::buy, 6},{_functions_2p[1], Invoker::sell, 6}
};
const Operation SyntaxAnalyzer::_functions_print_op = {_function_print, Invoker::print, 6};
const Operation SyntaxAnalyzer::_assign_op[] = {{_assign[0], Invoker::assign, 0}};
const Operation SyntaxAnalyzer::_binary_operators_op[] = {
	{_binary_operators[0], Invoker::greater, 2},{_binary_operators[1], Invoker::less, 2},
	{_binary_operators[2], Invoker::plus, 3},{_binary_operators[3], Invoker::minus, 3},
	{_binary_operators[4], Invoker::multiply, 4},{_binary_operators[5], Invoker::divide, 4},
	{_binary_operators[6], Invoker::logic_and, 1},{_binary_operators[7], Invoker::logic_or, 1},
};
const Operation SyntaxAnalyzer::_unary_operators_op[] = {
	{_unary_operators[0], Invoker::unary_plus, 5},{_unary_operators[1], Invoker::unary_minus, 5},
	{_unary_operators[2], Invoker::logic_not, 5},
};


const char * const SyntaxAnalyzer::_delims[] = { ")", ";", "," };

const char   SyntaxAnalyzer::_identifiers[] = { '$', '?' };

const char * SyntaxAnalyzer::_start = "start";
const char * SyntaxAnalyzer::_finish = "finish";

void SyntaxAnalyzer::ExpectString(const char* s1)
{
	
	if (_lexems==0 || cmpstr(_lexems->word, s1))
	{
		char err[100] = {0};
		if (_lexems!=0)
		{
			fprintf(stderr, "lexem: %s, expected: %s\n", _lexems->word, s1);
			throw Error(err, 100, *_lexems);
		}
		else
		{
			fprintf(stderr, "no lexem , expected: %s\n", s1);
			throw Error(err, 100);
		}
	}
}

bool SyntaxAnalyzer::InArray(const char* s, const char* const* arr, int sz)
{
	for (int i = 0; i < sz; i++)
	{
		if (!cmpstr(s, arr[i]))
		{
			_last_index_in_array = i;
			return true;
		}
	}
	_last_index_in_array = -1;
	return false;
}

void SyntaxAnalyzer::Start()
{
	ExpectString(_start);

	Next();
	Block();

	ExpectString(_finish);
	Next();
	if (_lexems != 0)
	{
		fprintf(stderr, "Lexems after \"finish\" keyword");
		while (_lexems != 0)
		{
			_lexems->Print();
		}
		throw "Lexems after \"finish\" keyword";
	}
	else
	{
		fprintf(stderr, "Syntax Analyzer success!\n");
	}
}

void SyntaxAnalyzer::Block()
{
	ExpectString("{");
	Next();
	
	while (cmpstr(_lexems->word, "}"))
	{
		//write
		if (IsVariable())
		{
			AddVariable(&_lexems->word[1]);
			Next();
			ExpectString("=");

			Next();
			Statement();

			AddOperation(new RpnOperation(_assign_op[0], 2, false));
			ExpectString(";");
			Next();
		}
		//result of the function is irrelevant, only actions inside
		else if (IsFunction())
		{
			Functions();
			ExpectString(";");
			Next();
		}
		else if (IsPrintFunction())
		{
			PrintFunction();
			ExpectString(";");
			Next();
		}
		else if (InArray(_lexems->word, _control_keywords, sizeof(_control_keywords)/sizeof(_control_keywords[0])))
		{
			if (!cmpstr(_lexems->word, _control_keywords[0]))
			{
				If();
			}
			else
			{
				While();
			}
		}
	}
	Next();
}

void SyntaxAnalyzer::Statement()
{
	RpnOperation* rpn_op_stack[80];
	int stack_index = 0;

	RpnOperation* rpn_unary_op_stack[80];
	int unary_stack_index = 0;
	do
	{
		while (InArray(_lexems->word, _unary_operators, sizeof(_unary_operators)/sizeof(_unary_operators[0])))
		{
			AddUnaryOperationOnStack(rpn_unary_op_stack, unary_stack_index, _unary_operators_op[_last_index_in_array], 1, true);
			Next();
		}

		if (_lexems->word[0] == '(')
		{
			Next();
			Statement();

			ExpectString(")");
			PushOperations(rpn_unary_op_stack, unary_stack_index);
			Next();
		}
		//read
		else if (IsVariable())
		{
			AddVariable(&_lexems->word[1]);
			PushOperations(rpn_unary_op_stack, unary_stack_index);
			Next();
		}
		//take the result
		else if (IsFunction())
		{
			Functions();
			PushOperations(rpn_unary_op_stack, unary_stack_index);
		}
		else if (IsNum())
		{
			AddConstant(atoi(_lexems->word));
			PushOperations(rpn_unary_op_stack, unary_stack_index);
			Next();
		}
		else
		{
			const char buf[] = "Expected identifier, function, number or braces\n";
			throw Error(buf, sizeof(buf), *_lexems);
		}


		if (InArray(_lexems->word, _binary_operators, sizeof(_binary_operators)/sizeof(_binary_operators[0])))
		{
			AddOperationOnStack(rpn_op_stack, stack_index, _binary_operators_op[_last_index_in_array], 2, true);
			Next();
			continue;
		}
		
	} while (!InArray(_lexems->word, _delims, sizeof(_delims)/sizeof(_delims[0])));
	PushOperations(rpn_op_stack, stack_index);
}

void SyntaxAnalyzer::Functions()
{
	const char * func_name = &_lexems->word[1];
	
	if (InArray(func_name, _functions_0p, sizeof(_functions_0p)/sizeof(_functions_0p[0])))
	{
		const Operation& op = _functions_0p_op[_last_index_in_array];
		Next();
		Functions0();
		AddOperation(new RpnOperation(op, 0, true));
	}
	else if (InArray(func_name, _functions_1p, sizeof(_functions_1p)/sizeof(_functions_1p[0])))
	{
		const Operation& op = _functions_1p_op[_last_index_in_array];
		Next();
		Functions1();
		AddOperation(new RpnOperation(op, 1, true));
	}
	else if (InArray(func_name, _functions_2p, sizeof(_functions_2p)/sizeof(_functions_2p[0])))
	{
		const Operation& op = _functions_2p_op[_last_index_in_array];
		Next();
		Functions2();
		AddOperation(new RpnOperation(op, 2, true));
	}
	else
	{
		char buf[] = "Expected function\n";
		throw Error(buf, sizeof(buf), *_lexems);
	}
}

void SyntaxAnalyzer::Functions0()
{
	ExpectString("(");
	Next();
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::Functions1()
{
	ExpectString("(");
	Next();
	Statement();
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::Functions2()
{
	ExpectString("(");
	Next();
	Statement();
	ExpectString(",");
	Statement();
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::PrintFunction()
{
	Next();
	ExpectString("(");
	int count = 0;
	do
	{
		Next();
		if (IsLiteral())
		{
			AddLiteral(_lexems->word);
			Next();
		}
		else 
		{
			Statement();
		}
		count++;
	} while (!cmpstr(_lexems->word, ","));
	
	ExpectString(")");
	AddOperation(new RpnOperation(_functions_print_op, count, false));
	Next();
}

void SyntaxAnalyzer::ControlFunctions()
{
	Next();
	ExpectString("(");
	Next();
	Statement();
	ExpectString(")");
	Next();
	Block();
}

void SyntaxAnalyzer::If()
{
	Next();
	ExpectString("(");
	Next();
	Statement();
	ExpectString(")");
	int pos = _rpn_items_index;
	AddConstant(0);
	AddGoto(true);
	Next();
	Block();

	static_cast<RpnConstant*>(_rpn_items[pos])->SetValue(_rpn_items_index);
}

void SyntaxAnalyzer::While()
{
	Next();
	ExpectString("(");
	int pos1 = _rpn_items_index;
	Next();
	Statement();
	ExpectString(")");
	int pos2 = _rpn_items_index;
	AddConstant(0);
	AddGoto(true);
	Next();
	Block();
	AddConstant(pos1);
	AddGoto(true);
	static_cast<RpnConstant*>(_rpn_items[pos2])->SetValue(_rpn_items_index);
}


void SyntaxAnalyzer::AddVariable(const char* label)
{
	int index = -1;

	for (int i = 0; i < _var_table_index; i++)
	{
		if (!cmpstr(_var_table_names[i], label))
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		_rpn_items[_rpn_items_index++] = new RpnVar(_var_table[index], label);
	}
	else
	{
		_var_table[_var_table_index] = 0;
		memcpy(&_var_table_names[_var_table_index], label, lenstr(label));
		_rpn_items[_rpn_items_index++] = new RpnVar(_var_table[_var_table_index], label);
		_var_table_index++;
	}
}

void SyntaxAnalyzer::AddOperation(RpnOperation * op)
{
	_rpn_items[_rpn_items_index++] = op;
}


void SyntaxAnalyzer::AddOperationOnStack(RpnOperation** stack, int& index, const Operation& op, int amount, bool has_return)
{
	if(index > 0)
	{
		//push everything that has greater or equal priority
		index--;
		while (index >= 0 && stack[index]->GetOp().priority >= op.priority)
		{
			AddOperation(stack[index]);
			index--;
		}
		index++;

	}
	stack[index++] = new RpnOperation(op, amount, has_return);
}

void SyntaxAnalyzer::PushOperations(RpnOperation** stack, int& index)
{
	while (--index >= 0)
	{
		AddOperation(stack[index]);
	}
}

void SyntaxAnalyzer::AddUnaryOperationOnStack(RpnOperation** stack, int& index, const Operation& op, int amount,
                                              bool has_return)
{
	//because all unary op has the same priority we just add them
	stack[index++] = new RpnOperation(op, amount, has_return);
}

void SyntaxAnalyzer::AddConstant(int value)
{
	_rpn_items[_rpn_items_index++] = new RpnConstant(value);
}

void SyntaxAnalyzer::AddLiteral(const char* label)
{
	_rpn_items[_rpn_items_index++] = new RpnLiteral(label, lenstr(label));
}

void SyntaxAnalyzer::AddGoto(bool if_false)
{
	_rpn_items[_rpn_items_index++] = new RpnGoto(if_false);
}

void SyntaxAnalyzer::Analyze(lexem_list* lexems)
{
	if (lexems == 0)
		return;
	
	_lexems = lexems;

	Start();
	
}

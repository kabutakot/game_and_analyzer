#pragma once

#include <cstring>
#include "ErrorException.h"
#include "LexAnalyzer/LexAnalyzer.h"
#include "RpnItems/RpnItems.h"
#include "Shared/shared.h"
#include "RpnItems/Invoker.h"


class SyntaxAnalyzer
{
	//should be shared_ptr
	lexem_list* _lexems;

	//RPN
	int _var_table[80];
	char _var_table_names[80][80];
	int _var_table_index;
	RpnItem* _rpn_items[800];
	int _rpn_items_index;
	
	int _last_index_in_array;

	static const char * const _control_keywords[2];
	static const char * const _functions_0p[2];
	static const char * const _functions_1p[3];
	static const char * const _functions_2p[2];
	static const char * _function_print;
	
	static const char * const _assign[1];
	static const char * const _binary_operators[8];
	static const char * const _unary_operators[3];
	static const char * const _delims[3];

	static const char _identifiers[2];

	static const char * _start;
	static const char * _finish;


	static const Operation _functions_0p_op[2];
	static const Operation _functions_1p_op[3];
	static const Operation _functions_2p_op[2];

	static const Operation _functions_print_op;

	static const Operation _assign_op[1];
	static const Operation _binary_operators_op[8];
	static const Operation _unary_operators_op[3];

	
	void Next()
	{
		_lexems = _lexems->next;
	}

	void ExpectString(const char* s1);

	bool InArray(const char* s, const char* const * arr, int sz);

	bool IsVariable() { return _lexems->word[0] == '$'; }

	bool IsFunction() { return _lexems->word[0] == '?'; }
	
	bool IsNum() { return _lexems->word[0] >= '0' && _lexems->word[0] <= '9'; }

	bool IsPrintFunction() { return !cmpstr(_lexems->word, _function_print); }
	
	bool IsLiteral() { return _lexems->word[0] == '"'; }


	void Start();
	void Block();
	//за проверку правильного разделителя отвечает вызывающий эту функцию
	void Statement();
	void Functions();

	void Functions0();
	void Functions1();
	void Functions2();

	void PrintFunction();

	void ControlFunctions();
	void If();
	void While();


	void AddVariable(const char* label);
	void AddOperation(RpnOperation * op);
	void AddOperationOnStack(RpnOperation** stack, int& index, const Operation& op, int amount, bool has_return = true);
	void PushOperations(RpnOperation** stack, int& index);
	void AddUnaryOperationOnStack(RpnOperation** stack, int& index, const Operation& op, int amount, bool has_return = true);
	void AddConstant(int value);
	void AddLiteral(const char* label);
	void AddGoto(bool if_false);

	
	
public:
	SyntaxAnalyzer() : _rpn_items_index(0), _var_table_index(0) {}
	void Analyze(lexem_list* lexems);

	void PrintRpn()
	{
		for (int i = 0; i < _rpn_items_index; i++)
		{
			_rpn_items[i]->Print();
			printf(" ");
		}
		printf("\n");

		for (int i = 0; i <_var_table_index; i++)
		{
			printf("%s %d\n", _var_table_names[i], _var_table[i]);
		}
		printf("\n");
	}
};




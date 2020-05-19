#pragma once

#include <cstring>


#include "ErrorException.h"
#include "LexAnalyzer/LexAnalyzer.h"

class SyntaxAnalyzer
{
	//should be shared_ptr
	lexem_list* _lexems;

	static const char * const _control_keywords[];
	static const char * const _functions_0p[];
	static const char * const _functions_1p[];
	static const char * const _functions_2p[];
	static const char * _function_print;
	
	static const char * const _assign[];
	static const char * const _binary_operators[];
	static const char * const _unary_operators[];
	static const char * const _delims[];

	static const char _identifiers[];

	static const char * _start;
	static const char * _finish;



	
	void Next()
	{
		_lexems = _lexems->next;
	}
	
	void ExpectString(const char* s1)
	{
		if (strcmp(_lexems->word, s1))
		{
			char err[100] = {0};
			sprintf(err, "lexem: %s, expected: %s", _lexems->word, s1);
			throw Error(err, *_lexems);
		}
	}

	bool InArray(const char* s, const char * const * arr)
	{
		//test this shit
		for (int i = 0; i < sizeof(arr); i++)
		{
			if (!strcmp(s, arr[i]))
				return true;
		}
		return false;
	}

	bool InArray(char s, const char * arr)
	{
		//test this shit
		for (int i = 0; i < sizeof(arr); i++)
		{
			if (s == arr[i])
				return true;
		}
		return false;
	}

	bool IsVariable()
	{
		return _lexems->word[0] == '$';
	}

	bool IsFunction()
	{
		return _lexems->word[0] == '?';
	}

	bool IsPrintFunction()
	{
		return !strcmp(_lexems->word, _function_print);
	}


	void Start();
	void A();
	void Block();
	void B1();
	//за проверку правильного разделителя отвечает вызывающий эту функцию
	void C1();
	void Functions();

	void Functions0();
	void Functions1();
	void Functions2();

	void PrintFunction();

	void ControlFunctions();
	void If();
	void While();
	
public:
	void Analyze(lexem_list* lexems);
};


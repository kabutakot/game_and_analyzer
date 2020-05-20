#pragma once

#include <cstring>


#include "ErrorException.h"
#include "LexAnalyzer/LexAnalyzer.h"

class SyntaxAnalyzer
{
	//should be shared_ptr
	lexem_list* _lexems;

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



	
	void Next()
	{
		_lexems = _lexems->next;
	}

	void ExpectString(const char* s1);

	bool InArray(const char* s, const char* const * arr, int sz);

	bool InArray(char s, const char* arr, int sz);

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
	
public:
	void Analyze(lexem_list* lexems);
};


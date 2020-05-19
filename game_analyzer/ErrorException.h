#pragma once

#include "LexAnalyzer/LexAnalyzer.h"



class Error
{
	const char * _error_mess;
	lexem_list _error_lexem;

public:
	Error(const char * error_mess, lexem_list error_lexem) :
		_error_mess(error_mess), _error_lexem(error_lexem)
	{}

	void Print()
	{
		printf("Caught exception!\n");
		printf(_error_mess);
		_error_lexem.Print();
	}
};
#pragma once

#include "LexAnalyzer/LexAnalyzer.h"



class Error
{
	char _error_mess[150];
	lexem_list _error_lexem;

public:
	Error(const char * error_mess, int sz, lexem_list error_lexem) :
		_error_lexem(error_lexem)
	{
		memset(_error_mess, 0, 150);
		memcpy(_error_mess, error_mess, sz);
	}
	
	Error(const char * error_mess, int sz)
	{
		memset(_error_mess, 0, 150);
		memcpy(_error_mess, error_mess, sz);
	}

	void Print()
	{
		fprintf(stderr, "Caught exception!\n");
		fprintf(stderr, "%s", _error_mess);
		_error_lexem.Print();
	}
};
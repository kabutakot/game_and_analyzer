
#include "SyntaxAnalyzer.h"

const char * const SyntaxAnalyzer::_control_keywords[] = { "if", "while" };
const char * const SyntaxAnalyzer::_functions_0p[] = { "info", "turn" };
const char * const SyntaxAnalyzer::_functions_1p[] = { "player", "prod", "build" };
const char * const SyntaxAnalyzer::_functions_2p[] = { "buy", "sell" };
const char * SyntaxAnalyzer::_function_print = "print";

const char * const SyntaxAnalyzer::_assign[] = { "=" };
const char * const SyntaxAnalyzer::_binary_operators[] = { ">", "<", "+", "-", "*", "/", "&", "|" };
const char * const SyntaxAnalyzer::_unary_operators[] = { "+", "-", "!" };
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
	//test this shit
	for (int i = 0; i < sz; i++)
	{
		if (!cmpstr(s, arr[i]))
			return true;
	}
	return false;
}

bool SyntaxAnalyzer::InArray(char s, const char* arr, int sz)
{
	//test this shit
	for (int i = 0; i < sz; i++)
	{
		if (s == arr[i])
			return true;
	}
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
			Next();
			ExpectString("=");

			Next();
			Statement();

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
			ControlFunctions();
		}
	}
	Next();
}

void SyntaxAnalyzer::Statement()
{
	do
	{
		while (InArray(_lexems->word, _unary_operators, sizeof(_unary_operators)/sizeof(_unary_operators[0])))
		{
			Next();
		}

		if (_lexems->word[0] == '(')
		{
			Next();
			Statement();

			ExpectString(")");
			Next();
		}
		//read
		else if (IsVariable())
		{
			Next();
		}
		//take the result
		else if (IsFunction())
		{
			Functions();
		}
		else if (IsNum())
		{
			Next();
		}
		else
		{
			const char buf[] = "Expected identifier, function, number or braces\n";
			throw Error(buf, sizeof(buf), *_lexems);
		}


		if (InArray(_lexems->word, _binary_operators, sizeof(_binary_operators)/sizeof(_binary_operators[0])))
		{
			Next();
			continue;
		}
		
	} while (!InArray(_lexems->word, _delims, sizeof(_delims)/sizeof(_delims[0])));
}

void SyntaxAnalyzer::Functions()
{
	const char * func_name = &_lexems->word[1];

	if (InArray(func_name, _functions_0p, sizeof(_functions_0p)/sizeof(_functions_0p[0])))
	{
		Next();
		Functions0();
	}
	else if (InArray(func_name, _functions_1p, sizeof(_functions_1p)/sizeof(_functions_1p[0])))
	{
		Next();
		Functions1();
	}
	else if (InArray(func_name, _functions_2p, sizeof(_functions_2p)/sizeof(_functions_2p[0])))
	{
		Next();
		Functions2();
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
	do
	{
		Next();
		if (IsLiteral())
		{
			Next();
		}
		else 
		{
			Statement();
		}
	} while (!cmpstr(_lexems->word, ","));
	
	ExpectString(")");
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


void SyntaxAnalyzer::Analyze(lexem_list* lexems)
{
	if (lexems == 0)
		return;
	
	_lexems = lexems;

	Start();
	
}

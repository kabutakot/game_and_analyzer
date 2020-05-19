
#include "SyntaxAnalyzer.h"

#include <cstring>


const char * const SyntaxAnalyzer::_control_keywords[] = { "if", "while" };
const char * const SyntaxAnalyzer::_functions_0p[] = { "info", "turn" };
const char * const SyntaxAnalyzer::_functions_1p[] = { "player", "prod", "build" };
const char * const SyntaxAnalyzer::_functions_2p[] = { "buy", "sell" };
const char * SyntaxAnalyzer::_function_print = "print";

const char * const SyntaxAnalyzer::_assign[] = { ":=" };
const char * const SyntaxAnalyzer::_binary_operators[] = { ">", "<", "+", "-", "*", "/", "&", "|" };
const char * const SyntaxAnalyzer::_unary_operators[] = { "-", "!" };
const char * const SyntaxAnalyzer::_delims[] = { ")", ";", "," };

const char   SyntaxAnalyzer::_identifiers[] = { '$', '?' };

const char * SyntaxAnalyzer::_start = "start";
const char * SyntaxAnalyzer::_finish = "finish";

void SyntaxAnalyzer::Start()
{
	ExpectString(_start);

	Next();
	Block();

	ExpectString(_finish);
	Next();
	if (_lexems != NULL)
	{
		printf("Lexems after \"finish\" keyword");
		while (_lexems != NULL)
		{
			_lexems->Print();
		}
		throw "Lexems after \"finish\" keyword";
	}
}

void SyntaxAnalyzer::Block()
{
	ExpectString("{");
	Next();
	
	while (strcmp(_lexems->word, "}"))
	{
		//write
		if (IsVariable())
		{
			Next();
			//delete this possibly
			B1();

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
		else if (InArray(_lexems->word, _control_keywords))
		{
			ControlFunctions();
		}
	}
	Next();
}

void SyntaxAnalyzer::B1()
{
	ExpectString(":=");

	Next();
	C1();
}

void SyntaxAnalyzer::C1()
{
	do
	{
		while (InArray(_lexems->word, _unary_operators))
		{
			Next();
		}

		if (_lexems->word[0] == '(')
		{
			Next();
			C1();

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
			Next();
		}
		else if (_lexems->type == num)
		{
			Next();
		}
		else
		{
			throw Error("Expected identifier, function, number or braces\n", *_lexems);
		}


		if (InArray(_lexems->word, _binary_operators))
		{
			Next();
		}
		
	} while (!InArray(_lexems->word, _delims));
}

void SyntaxAnalyzer::Functions()
{
	const char * func_name = &_lexems->word[1];

	if (InArray(func_name, _functions_0p))
	{
		Next();
		Functions0();
	}
	else if (InArray(func_name, _functions_1p))
	{
		Next();
		Functions1();
	}
	else if (InArray(func_name, _functions_2p))
	{
		Next();
		Functions2();
	}
	else
	{
		throw Error("Expected function\n", *_lexems);
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
	C1();
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::Functions2()
{
	ExpectString("(");
	Next();
	C1();
	ExpectString(",");
	C1();
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::PrintFunction()
{
	Next();
	ExpectString("(");
	Next();
	
	do
	{
		if (_lexems->type == literal)
		{
			Next();
		}
		else 
		{
			C1();
		}
	} while (strcmp(_lexems->word, ","));
	
	ExpectString(")");
	Next();
}

void SyntaxAnalyzer::ControlFunctions()
{
	Next();
	ExpectString("(");
	Next();
	C1();
	ExpectString(")");
	Next();
	Block();
}


void SyntaxAnalyzer::Analyze(lexem_list* lexems)
{
	if (lexems == NULL)
		return;
	
	_lexems = lexems;

	Start();
	
}

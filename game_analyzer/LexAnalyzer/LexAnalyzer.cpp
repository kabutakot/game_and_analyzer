#include "LexAnalyzer.h"




void LexAnalyzer::clean_lexem()
{

}

LexAnalyzer::LexAnalyzer()
{
	all = false;
	flag = home;
	word = 0;
	line = 1;
	first = 0;
}

LexAnalyzer::~LexAnalyzer()
{
	//clean_item();
	clean_lexem();
}

void LexAnalyzer::handler_home(const char c)
{

}

void LexAnalyzer::step(const char c)
{
	switch (flag)
	{
	case home:
		handler_home(c);
		break;
	case indent:
		handler_indent(c);
		break;
	case numb:
		handler_numb(c);
		break;
	case com:
		handler_com(c);
		break;
	case oper:
		handler_oper(c);
		break;
		// case liter:
		// handler_liter(c);
		// break;
	case err:
		flag = err;
	}
	if (all)
	{
		all = false;
		step(c);
	}
	else
		if (c == '\n')
			line++;
}


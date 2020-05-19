#include "LexAnalyzer.h"

bool is_separator(const char c)
{
	bool t1 = (c == '+' || c == '-' || c == '*' || c == '/');
	bool t2 = (c == '(' || c == ')' || c == '{' || c == '}' || c == '#' || c == ';');
	bool t3 = (c == '&' || c == '|' || c == '!' || c == '>' || c == '>' || c == '=');
	return t1 || t2 || t3;
}

void LexAnalyzer::clean_item()
{
	item *p;
	while (lex)
	{
		p = lex;
		lex = lex->next;
		delete p;
	}
}

LexAnalyzer::LexAnalyzer()
{
	rep = false;
	flag = home;
	lex = 0;
	line = 1;
	first = 0;
}

LexAnalyzer::~LexAnalyzer()
{
	clean_item();
	//    clean_lexem();
}
bool LexAnalyzer::is_keywd()
{
	char *wd = word_lex();
	static const char str1[] = "if";
	static const char str2[] = "while";
	static const char str3[] = "start";
	static const char str4[] = "finish";
	return(!cmpstr(wd, str1) || !cmpstr(wd, str2) || !cmpstr(wd, str3) || !cmpstr(wd, str4));
}

void LexAnalyzer::add(const char c)
{
	item *p;
	if (lex)
	{
		p = lex;
		while (p->next)
		{
			p = p->next;
		}
		p->next = new item;
		p = p->next;
	}
	else
	{
		lex = new item;
		p = lex;
	}
	p->next = 0;
	p->q = c;
}

int LexAnalyzer::len_lex()
{
	int i = 0;
	item *p;
	p = lex;
	while (p)
	{
		p = p->next;
		i++;
	}
	return i;
}

char* LexAnalyzer::word_lex()
{
	char* wd = new char[len_lex() + 1];
	int i = 0;
	item *p;
	p = lex;
	while (p)
	{
		wd[i] = p->q;
		i++;
		p = p->next;
	}
	wd[i] = '\0';
	return wd;
}

void LexAnalyzer::add_lex(lexem type)
{
	lexem_list *p;
	if (first)
	{
		p = first;
		while (p->next)
		{
			p = p->next;
		}
		p->next = new lexem_list;
		p = p->next;
	}
	else
	{
		first = new lexem_list;
		p = first;
	}
	p->next = 0;
	p->word = word_lex();
	p->line = line;
	p->type = type;
	clean_item();
}

void LexAnalyzer::handler_home(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
	{
		return;
	}
	if (is_separator(c))
	{
		if (c == '#')
			flag = com;
		else
		{
			add(c);
			add_lex(separator);
		}
		return;
	}
	if (c == '"')
	{
		flag = liter;
		return;
	}
	add(c);
	if (c == '?' || c == '$')
	{
		flag = indent;
		return;
	}
	if (c >= '0'&&c <= '9')
	{
		flag = numb;
		return;
	}
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
	{
		flag = keywd;
		return;
	}
	add_lex(defect);
	flag = err;
}

void LexAnalyzer::handler_indent(char c)
{
	if ((len_lex() == 1) && (c<'a' || c>'z') && (c<'A' || c>'Z'))
	{
		add(c);
		add_lex(defect);
		flag = err;
		return;
	}
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9'))
	{
		add(c);
	}
	else
	{
		add_lex(identifier);
		rep = true;
		flag = home;
	}
}

void LexAnalyzer::handler_keywd(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
	{
		add(c);
	}
	else if (is_keywd())
	{
		add_lex(keyword);
		rep = true;
		flag = home;
	}
	else
	{
		add_lex(defect);
		flag = err;
	}
}

void LexAnalyzer::handler_numb(char c)
{
	if (c >= '0' && c <= '9')
	{
		add(c);
	}
	else
	{
		add_lex(num);
		rep = true;
		flag = home;
	}
}

void LexAnalyzer::handler_com(char c)
{
	if (c == '\n')
		flag = home;
}

void LexAnalyzer::handler_liter(char c)
{
	if (c == '"')
	{
		add_lex(literal);
		flag = home;
	}
	else
	{
		add(c);
	}
}

void LexAnalyzer::step(char c)
{
	switch (flag)
	{
	case home:
		handler_home(c);
		break;
	case indent:
		handler_indent(c);
		break;
	case keywd:
		handler_keywd(c);
		break;
	case numb:
		handler_numb(c);
		break;
	case com:
		handler_com(c);
		break;
	case liter:
		handler_liter(c);
		break;
	case err:
		flag = err;
	}
	if (rep)
	{
		rep = false;
		step(c);
		return;
	}
	if (c == '\n')
	{
		line++;
	}
}


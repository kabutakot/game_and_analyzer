#pragma once

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "shared.h"

enum lexem
{
	keyword,
	identifier,
	literal,
	separator,
	num,
	defect
};

struct lexem_list
{
	char *word;
	lexem type;
	int line;
	lexem_list *next;

	void Print()
	{
		if (word != NULL)
			printf("Line: %d, lexem: %s\n", line, word);
		else
			printf("Line: %d, empty_lexem!\n", line);
	}
};


class LexAnalyzer
{
	enum states
	{
		home,
		indent,
		keywd,
		numb,
		com,
		liter,
		err
	};
	struct item
	{
		char q;
		item *next;
	};
	bool rep;
	states flag;
	item *lex;
	int line;
	lexem_list *first;
	
private:
	void handler_home(char c);
	void handler_indent(char c);
	void handler_keywd(char c);
	void handler_numb(char c);
	void handler_com(char c);
	void add(const char c);
	void add_lex(lexem type);
	int len_lex();
	char* word_lex();
	bool is_keywd();
	void clean_item();
	void clean_lexem();
	void handler_liter(char c);
public:
	LexAnalyzer();
	~LexAnalyzer();
	void step(char c);
	lexem_list *push() { return first; }
	bool at_home() { return (flag == home); }
};

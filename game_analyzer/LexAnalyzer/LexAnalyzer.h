#pragma once
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


enum lexem
{
	keyword,
	identifier,
	literal,
	separator,
	operation,
	num,
	defect
};

enum states
{
	home,
	indent,
	numb,
	com,
	oper,
	// liter,
	err
};


struct lexem_list
{
	char *word = NULL;
	int line;

	lexem type;
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
	bool all;
	bool liter;
	states flag;
	char *word;
	int line;
	lexem_list *first;
	
private:
	LexAnalyzer();
	~LexAnalyzer();
	void handler_home(const char c);
	void handler_indent(const char c);
	void handler_numb(const char c);
	void handler_com(const char c);
	void handler_oper(const char c);
	// void handler_liter(const char c);
	void clean_lexem();
public:
	void step(const char c);
};
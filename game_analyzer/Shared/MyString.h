#pragma once
#include "MyVector.h"

class MyString : public MyVector<char>
{
public:
	MyString(const char *s)
	{
		*this = s;
	}

	MyString& operator=(const char* s)
	{
		int sz = strlen(s);
		resize(sz);
		strncpy(get(), s, sz);
		return *this;
	}
};

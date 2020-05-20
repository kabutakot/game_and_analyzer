#pragma once
#include "MyVector.h"

class MyString : public MyVector<char>
{
public:
	MyString(int amount = 0) : MyVector(amount) { memset(get(), 0, amount); }
	
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

	const char * c_str() { return get(); }
};

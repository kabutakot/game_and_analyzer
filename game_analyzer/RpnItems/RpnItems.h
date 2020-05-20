#pragma once
#include <cstdlib>
#include "Shared/shared.h"
#include <cstdio>
#include <cstring>

enum Functionality {CONSTANT = 0, VARIABLE/*, WRITE_VARIABLE*/, LITERAL, OPERATION, SIZE};


class RpnItem
{
protected:
	Functionality _type;
	
	RpnItem() : _type(SIZE) {}
	RpnItem(Functionality type) : _type(type) {}
	
public:
	Functionality GetType() const { return _type; }
	virtual ~RpnItem() {}

	virtual void Print() = 0;
};

class RpnOperand : public RpnItem
{
public:
	RpnOperand(Functionality type) : RpnItem(type) {}


};

class RpnGoto : public RpnItem
{
	bool _if_false;
public:
	RpnGoto(bool if_false) : RpnItem(OPERATION), _if_false(if_false) {}
	bool GetIfFalse() { return _if_false; }

	void Print()
	{
		if (_if_false)
			printf("!F");
		else
			printf("!T");
	}
};

//class RpnReadVar : public RpnOperand
//{
//	int _val;
//	
//public:
//	RpnReadVar(int val, const char *& label) : RpnOperand(READ_VARIABLE, label), _val(val) {}
//
//	int GetValue() const { return _val; }
//
//	
//};

class RpnVar : public RpnOperand
{
	int& _ref;
	char _label[80];
	
public:
	RpnVar(int& ref, const char* label) : RpnOperand(VARIABLE), _ref(ref)
	{
		memcpy(_label, label, lenstr(label));
	}

	int& GetValue() { return _ref; }

	void Print()
	{
		printf("%s", _label);
	}
};

class RpnConstant : public RpnOperand
{
	int _val;

public:
	RpnConstant(int val) : RpnOperand(CONSTANT), _val(val) {};

	int GetValue() { return _val; }
	void SetValue(int val) { _val = val; }

	void Print()
	{
		printf("%d", _val);
	}
};

class RpnLiteral : public RpnOperand
{
	char _value[80];
public:
	RpnLiteral(const char * value, int size) : RpnOperand(LITERAL) { memcpy(_value, value, size); }

	const char * GetValue() { return _value; }

	void Print()
	{
		printf("%s", _value);
	}
};


struct Operation
{
	const char * label;
	RpnOperand* (*func_impl)(RpnOperand**, int);
	int priority;
};

class RpnOperation : public RpnItem
{
	Operation _op;
	const int _amount;
	bool _has_return;

public:
	RpnOperation(const Operation& op, int amount, bool has_return = true)
	: RpnItem(OPERATION), _op(op), _amount(amount), _has_return(has_return) {}

	const Operation& GetOp() { return _op; }
	int GetAmount() { return _amount; }
	RpnOperand* Invoke ( RpnOperand** params, int amount) { return _op.func_impl(params, amount); }

	void Print()
	{
		printf("%s", _op.label);
	}
};

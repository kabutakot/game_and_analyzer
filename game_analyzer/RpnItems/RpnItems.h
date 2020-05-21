#pragma once
#include <cassert>
#include <cstdlib>
#include "Shared/shared.h"
#include <cstdio>
#include <cstring>

enum Functionality {CONSTANT = 0, VARIABLE, GOTO, LITERAL, OPERATION, SIZE};


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
	virtual int GetValue() { assert(false); return 0; }
	RpnOperand(Functionality type) : RpnItem(type) {}

};

class RpnGoto : public RpnItem
{
	bool _if_false;
public:
	RpnGoto(bool if_false) : RpnItem(GOTO), _if_false(if_false) {}
	bool IfFalse() { return _if_false; }

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

	int& GetAssignValue() { return _ref; }
	virtual int GetValue() { return _ref; }
	const char* GetLabel() { return _label; }
	
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
	RpnLiteral(const char * value) : RpnOperand(LITERAL) { memcpy(_value, value, lenstr(value)); }

	const char * GetStringValue() { return _value; }

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
	bool HasResult() { return _has_return; }
	
	void Print()
	{
		printf("%s", _op.label);
	}
};

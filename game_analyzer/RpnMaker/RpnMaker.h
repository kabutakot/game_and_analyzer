#pragma once

template <typename T>
class MyVector
{
	const int kInitialElements = 20;
	T* arr;
	int size = 0;
	int max_size = kInitialElements;
	
public:
	MyVector(int elem_count)
	{
		arr = new T[elem_count];
		size = 0;
		max_size = elem_count;
	}

	void push_back(const T& elem)
	{
		if (size == max_size)
		{
			T* new_arr = new T[max_size * 2];
		}
	}
	
	~MyVector();
	{
		delete arr;
	}
};

class RpnMaker
{
	
};
#pragma once
#pragma once
#include <cstring>
#include <cstdlib>

template <typename T>
class MyVector
{
	static const int kInitialElements;
	T* _arr;
	int _size;
	int _max_size;

protected:

	//use this carefully
	T* get()
	{
		return _arr;
	}
	
public:
	MyVector(int elem_count = kInitialElements)
	{
		_arr = NULL;
		if (elem_count > 0)
			_arr = new T[elem_count];
		_size = 0;
		_max_size = elem_count;
	}

	MyVector(const MyVector<T>& other)
	{
		*this = other;
	}

	
	MyVector<T>& operator=(const MyVector<T>& other)
	{
		//self-assignment
		if (_arr == other._arr)
			return *this;

		if (_max_size < other._max_size)
		{
			delete[] _arr;
			_arr = new T[other._max_size];
		}

		memcpy(_arr, other._arr, sizeof(T) * other._size);
		_size = other._size;
		_max_size = other._max_size;
		return *this;
	}

	T& operator[](int index)
	{
		if (index >= _size)
		{
			throw "MyVector out of bounds";
		}

		return _arr[index];
	}

	const T& operator[](int index) const
	{
		if (index >= _size)
		{
			throw "MyVector out of bounds";
		}

		return _arr[index];
	}
	
	void push_back(const T& elem)
	{
		if (_size == _max_size)
		{
			int new_size ;
			if (_max_size < 20)
			{
				new_size = _max_size + 20;
			}
			else
			{
				new_size = _max_size * 2;
			}

			resize(new_size);
		}
		_arr[_size++] = elem;
	}

	void resize(int new_size)
	{
		if (new_size > _max_size)
		{
			T* new_arr = new T[new_size];
			memcpy(new_arr, _arr, _max_size * sizeof(T));
			_max_size = new_size;
			delete[] _arr;
			_arr = new_arr;
		}
		else if (new_size < _max_size)
		{
			_max_size = new_size;
			_size = _size > new_size ? new_size : _size;
		}
	}

	int size() const
	{
		return _size;
	}

	void sort(int (*compar)(const void*, const void*))
	{
		qsort(_arr, _size, sizeof(T), compar);
	}
	
	virtual ~MyVector()
	{
		delete[] _arr;
	}
};

template<typename T>
const int MyVector<T>::kInitialElements = 20;

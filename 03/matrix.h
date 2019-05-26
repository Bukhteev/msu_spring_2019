#pragma once
#include <iostream>
#include <cstring>

class Matrix
{
	const size_t rows;
	const size_t cols;
	int *data;
public:

	Matrix(size_t rs, size_t cs):
	rows(rs), cols(cs), data(new int[rs * cs]) 

	{}

	~Matrix()
	{
		delete[] data;
	}

	size_t getRows() const
	{
		return rows;
	}

	size_t getColumns() const
	{
		return cols;
	}


	class Array{
		size_t size;
		int *data;

	public:
		Array(int* first, size_t s): data(first),size(s)
		{}


		~Array()
		{}

		int& operator[](size_t i)
		{
			if(i >= size){
				throw std::out_of_range("");
			}
			return data[i];
		} 

		int const operator[](size_t i) const
		{
			if(i >= size){
				throw std::out_of_range("");
			}
			return data[i];
		}

	};




	Array operator[](size_t i)
	{
		if(i >= rows){
				throw std::out_of_range("");
			}
			return Array(&data[cols * i], cols);
	}

	const Array operator[](size_t i) const
	{
		if(i >= rows){
				throw std::out_of_range("");
			}
			return Array(&data[cols * i], cols);
	}


	bool operator==(const Matrix& other) const
	{
		if (this == &other) {
			return true;
		}
		bool result = (rows == other.rows &&
				cols == other.cols &&
				memcmp(data, other.data, rows * cols * sizeof(int)) == 0);
		return result;
	}

	bool operator!=(const Matrix& other) const
	{
		return !(*this == other);

	}
	

	Matrix& operator*=(const int number){
		for(size_t i = 0; i < rows * cols; i++){
			data[i]*=number;
		}
		return *this;
	}


};

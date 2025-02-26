#pragma once
#include <iostream>
using namespace std;
class matrix
{
	private:
		int size;
		int** matr;
	public:

		matrix()
		{
			size = 0;
			matr = new int* [size];
		}
		matrix(int n)
		{
			size = n;
			matr = new int* [size];
			for (int i = 0; i < size; i++)
			{
				matr[i] = new int[size];
			}

			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (i == j)
					{
						matr[i][j] = 1;
					}
					else
					{
						matr[i][j] = 0;
					}
				}
				
			}
		}
		matrix(int n, int* mas)
		{
			size = n;
			matr = new int* [size];
			for (int i = 0; i < size; i++)
			{
				matr[i] = new int[size];
			}
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (i == j)
					{
						matr[i][j] = mas[i];
					}
					else
					{
						matr[i][j] = 0;
					}
				}
			}
		}
		matrix(matrix const& that)
		{
			size = that.size;
			matr = new int* [size];
			for (int i = 0; i < size; i++)
			{
				matr[i] = new int[size];
				for (int j = 0; j < size; j++)
				{
					matr[i][j] = that.matr[i][j];
				}
			}
		}
		~matrix()
		{
			for (int i = 0; i < size; i++)
			{
				delete[] matr[i];
 			}
			delete[] matr;
		}
		
		matrix operator+ (matrix const & that) const
		{
			if (this->size != that.size)
			{
				matrix result;
				return result;
			}
			else
			{
				matrix result(this->size);
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						result.matr[i][j] = this->matr[i][j] + that.matr[i][j];
					}
				}
				return result;
			}
		}
		matrix operator- (matrix const & that) const
		{
			if (this->size != that.size)
			{
				matrix result;
				return result;
			}
			else
			{
				matrix result(this->size);
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						result.matr[i][j] = this->matr[i][j] - that.matr[i][j];
					}
				}
				return result;
			}
		}
		matrix operator* (matrix const& that) const
		{
			if (this->size != that.size)
			{
				matrix result;
				return result;
			}
			else
			{
				matrix result(this->size);
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						result.matr[i][j] = 0;
						for (int k = 0; k < this->size; k++)
							result.matr[i][j] += this->matr[i][k] * that.matr[k][j];
					}
				}
				return result;
			}

		}
		bool operator== (matrix const& that) const
		{
			if (this->size != that.size)
			{
				return false;
			}
			else
			{
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						if (this->matr[i][j] != that.matr[i][j])
						{
							return false;
						}
					}
				}
				return true;
			}
		}
		bool operator!= (matrix const& that) const
		{
			if (this->size != that.size)
			{
				return true;
			}
			else
			{
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						if (this->matr[i][j] != that.matr[i][j])
						{
							return true;
						}
					}
				}
				return false;
			}
		}
		matrix operator= (const matrix& that)
		{
			return *this;
		}
		matrix operator! () const
		{
			matrix result(this->size);
			for (int i = 0; i < this->size; i++) {
				for (int j = 0; j < this->size; j++) {
					result.matr[j][i] = this->matr[i][j];
				}
			}
			return result;
		}
		matrix operator() (int a, int b) 
		{
			bool flag1 = 0, flag2 = 0;
			matrix result(this->size - 1);
			for (int i = 0; i < this->size-1; i++) {
				if (i + 1 == a) {
					flag1 = 1;
				}
				for (int j = 0; j < this->size-1; j++) {
					if (j + 1 == b) {
						flag2 = 1;
					}
					result.matr[i][j] = this->matr[i + flag1][j + flag2];
				}
				flag2 = 0;
			}
			return result;
		}
		int* operator() (int a) 
		{
			/**/
		}

		void print()
		{
			if (this->size == 0)
			{

			}
			else
			{
				for (int i = 0; i < this->size; i++)
				{
					for (int j = 0; j < this->size; j++)
					{
						cout << this->matr[i][j] << " ";
					}
					cout << endl;
				}
			}
		}
		void input(int** arr) 
		{
			for (int i = 0; i < this->size; i++)
			{
				for (int j = 0; j < this->size; j++) {
					this->matr[i][j] = arr[i][j];
				}
			}
		}
};


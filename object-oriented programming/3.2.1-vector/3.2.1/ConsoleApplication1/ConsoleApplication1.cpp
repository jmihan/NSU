#include <iostream>
#include <string>
#include <fstream>
using namespace std;
ifstream in("input.txt", ios::in);
ofstream out("output.txt", ios::out);
template<typename T>
class vector
{
private:
	T* data;
	size_t size;//размерность вектора
	size_t capacity;//максимальная размерность
public:
	//конструктор по умолчанию
	vector()
	{
		this->size = 0;
		this->capacity = 10;
		data = new T[this->capacity];
	}
	//деструктор
	/*~vector()
	{
		delete data;
	}*/
	size_t getSize()
	{
		return size;
	}
	size_t getCapacity()
	{
		return capacity;
	}
	//добавление памяти
	void add_memory()
	{
		capacity *= 2;
		T* temp = data;
		data = new T[capacity];
		for (int i = 0; i < capacity; i++)
		{
			data[i] = temp[i];
		}
		delete[] temp;
	}
	void push_back(T a)
	{
		if (size == capacity)
		{
			add_memory();
		}
		data[size] = a;
		size++;
	}
	void remove(int ind)
	{
		if (ind >= size)
		{
			cout << "ERROR" << endl;
			return;
		}
		for (int i = ind + 1; i < size; i++)
		{
			data[i - 1] = data[i];
		}
		size--;
	}
	T& operator[](int const ind)
	{
		if (ind >= size)
		{
			out << "ERROR" << endl;
		}
		else
		{
			return data[ind];
		}
	}
	const T& operator[](int const ind) const
	{
		if (ind >= size)
		{
			out << "ERROR" << endl;
		}
		else
		{
			return data[ind];
		}
	}
	void shiftR1() 
	{
		T temp = data[0];
		T temp2;
		T end = data[size - 1];
		for (int i = 1; i < size; i++) 
		{
			temp2 = data[i];
			data[i] = temp;
			temp = temp2;
		}
		data[0] = end;
	}
	void shiftR(int s) 
	{
		for (int i = 0; i < s; i++) 
		{
			this->shiftR1();
		}
	}
	void shiftL1() 
	{
		T temp = data[size-1];
		T temp2;
		T end = data[0];
		for (int i = size-2; i >= 0; i--)
		{
			temp2 = data[i];
			data[i] = temp;
			temp = temp2;
		}
		data[size-1] = end;
	}
	void shiftL(int s) 
	{
		for (int i = 0; i < s; i++)
		{
			this->shiftL1();
		}
	}
	void print()
	{
		for (int i = 0; i < size; i++)
		{
			out << data[i]<<endl;
		}
	}
};
template<typename T>
void start(int n)
{
	T el;
	int ind;
	string s;
	string* str= new string[6];
	str[0] = "ADD";
	str[1] = "REMOVE";
	str[2] = "PRINT";
	str[3] = "UPDATE";
	str[4] = "LSH";
	str[5] = "RSH";

	vector<T> v;
	for (int i = 0; i < n; i++)
	{
		in >> s;
		if (s == str[0])
		{
			in >> el;
			v.push_back(el);
			//v.print();
		}
		if (s==str[1])
		{
			in >> ind;
			v.remove(ind);
		}
		if (s==str[2])
		{
			in >> ind;
			if (v.getSize() <= ind)
			{
				out << "ERROR" << endl;
			}
			else
			{
				out << v[ind] << endl;

			}
		}
		if (s == str[3])
		{
			in >> ind;
			in >> el;
			v[ind] = el;
		}
		if (s == str[4])
		{
			in >> ind;
			/*v.print();
			out << endl;*/
			v.shiftL(ind);
		}
		if (s == str[5])
		{
			in >> ind;
			v.shiftR(ind);
		}
	}
	v.print();
}
int main()
{
	char ch; int n;
	in >> ch;
	in >> n;
	switch (ch)
	{
	case 'I':
		start<int>(n);
		break;
	case 'D':
		start<double>(n);
		break;
	case 'S':
		start<string>(n);
		break;
	default:
		break;
	}
	return 0;
}

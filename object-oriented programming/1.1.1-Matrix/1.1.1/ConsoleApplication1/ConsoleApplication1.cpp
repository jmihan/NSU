#include <iostream>
#include <fstream>
//#include "matrix.h"
using namespace std;
ifstream in("input.txt", ios::in); 
ifstream indop("input1.txt", ios::in);
ofstream out("output.txt", ios::out);
class matrix
{
private:
	int size;
	int** matr;
public:
	class stolbec
	{
	private:
		int pos;
		matrix* stolb;
	public:
		stolbec(const int& a, matrix* st) 
		{
			this->pos = a;
			this->stolb = st;
		}
		int& operator[](const int& a)
		{
			return this->stolb->matr[a][this->pos];
		}
		matrix* getMatr()
		{
			return stolb;
		}
	};
	matrix* operator()(const int& a)
	{
		stolbec stolb(a, this);
		return stolb.getMatr();
	}
	matrix() //нулевая матрица
	{
		size = 0;
		matr = new int* [size];
	}
	matrix(int n) //матрица 
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

	matrix operator+ (matrix const& that) const
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
	matrix operator- (matrix const& that) const
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
		this->size = that.size;
		for (int i = 0; i < this->size; i++)
		{
			for (int j = 0; j < this->size; j++)
			{
				this->matr[i][j] = that.matr[i][j];
			}
		}
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
		for (int i = 0; i < this->size - 1; i++) {
			if (i + 1 == a) {
				flag1 = 1;
			}
			for (int j = 0; j < this->size - 1; j++) {
				if (j + 1 == b) {
					flag2 = 1;
				}
				result.matr[i][j] = this->matr[i + flag1][j + flag2];
			}
			flag2 = 0;
		}
		return result;
	}
	int* operator[] (int a)
	{
		if ((a >= this->size) || (a < 0))
		{
			cout << "INCORRECT POSITION" << endl;
		}
		return this->matr[a];
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
					out << this->matr[i][j] << " ";
				}
				out << endl;
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
int main()
{
    setlocale(LC_ALL, "Russian");
	//отладка операций
    /*int n = 0;
    cin >> n;
    int* mas = new int[n];
    for (int i = 0; i < n; i++)
    {
        cin >> mas[i];
    }
    matrix a, b(n), c(n,mas);
    cout <<"Матрица а: " << endl;
    a.print();
    cout << endl;
    cout << "Матрица b: " << endl;
    b.print();
    cout << endl;
    cout << "Матрица c: " << endl;
    c.print();
    cout << endl;

    matrix d = b + c;
    cout << "Матрица d = b + c: " << endl;
    d.print();
    cout << endl;
    matrix f = d - b;
    cout << "Матрица f = d - b: " << endl;
    f.print();
    cout << endl;
    matrix e = f * d;
    cout << "Матрица e = a * d: " << endl;
    e.print();
    cout << endl;
    
    cout << "Матрица e = f * d: " << endl;
    e = f * d;
    e.print();
    cout << endl;

    
    cout << "Введите матрицу 3х3: " << endl;
    int size = 3;
    int** arr = new int* [size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            cin >> arr[i][j];
        }
    }
    
    e.input(arr);
    e.print();
    cout << endl;

    cout << "Транспонированная матрица e: " << endl;
    (!e).print();

    cout << endl;
    matrix aa = e(2,2);
    aa.print();*/ 
    //Задание на вычисление выражения
    /*int N = 0;
    int k = 0;
    in >> N;
    in >> k;
    int* karr = new int[N];
    for (int i = 0; i < N; i++)
    {
        karr[i] = k;
    }
    matrix A(N), B(N), C(N), D(N), K(N, karr);
    delete[] karr;
    int** mas = new int* [N];
    for (int i = 0; i < N; i++)
    {
        mas[i] = new int[N];
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            in >> mas[i][j];
        }
    }
    A.input(mas);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            in >> mas[i][j];
        }
    }
    B.input(mas);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            in >> mas[i][j];
        }
    }
    C.input(mas);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            in >> mas[i][j];
        }
    }
    D.input(mas);

    matrix a = B * (!C);
    matrix b = A + a;
    matrix c = b + K;
    matrix d = c * (!D);
    d.print();*/
	//Доп задание
	matrix a(10);
	int** mas = new int* [10];
	for (int i = 0; i < 10; i++)
	{
		mas[i] = new int[10];
		for (int j = 0; j < 10; j++)
		{
			indop >> mas[i][j];
		}
	}
	a.input(mas);
	a[5];
	cout <<"Old value a[5][3]: " << a[5][3] << endl;
	a[5][3] = 6;
	cout <<"New value a[5][3]: " << a[5][3] << endl;
	a(3);
	cout << "Old value a(3)[5]: " << a(3)[5] << endl;
	a(3)[5] = 12;
	cout << "New value a(3)[5]: " << a(3)[5] << endl << endl;
	a.print();
}

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;
ifstream in("input.txt", ios::in);
ofstream out("output.txt", ios::out);
class Equation
{
public:
    virtual double principalRoot() = 0;
    virtual void print() = 0;
};
class Linear : public Equation
{
private:
    double a, b;
public:
    Linear() : a(0), b(0) {}
    Linear(double a, double b) : a(a), b(b) {}
    double principalRoot()
    {
        return (- b / a);
    }
    void print()
    {
        cout << a << "x + " << b << " = 0" << endl;
    }
    ~Linear() {}
};
class Trigonometric : public Equation
{
private:
    double k;
public:
    Trigonometric() : k(0) {}
    Trigonometric(double k) : k(k) {}
    double principalRoot()
    {
        return abs(asin(k));
    }
    void print()
    {
        cout << "sin(x) - " << k << " = 0" << endl;
    }
    ~Trigonometric() {}
};
class Disjunction : public Equation
{
private:
    Trigonometric* a;
    Linear* b;
    int countTri;
    int countLin;
public:

    Disjunction(double* a, double* b, int t, int l,  double* k1) : countTri(t), countLin(l)
    {
        this->a = new Trigonometric[t];
        for (int i = 0; i < t; i++)
        {
            this->a[i] = Trigonometric(k1[i]);
        }
        this->b = new Linear[l];
        for (int i = 0; i < l; i++)
        {
            this->b[i] = Linear(a[i], b[i]);
        }
        /*Trigonometric a1(k);
        this->a = &a1;
        a1.~Trigonometric();
        Linear b1(a, b);
        this->b = &b1;
        b1.~Linear();*/
    }
    double sum()
    {
        double sum = 0;
        for (int i = 0; i < countTri; i++)
        {
            sum += a[i].principalRoot();
        }
        for (int i = 0; i < countLin; i++)
        {
            sum += b[i].principalRoot();
        }
    }
};
int main()
{
    int N = 0;
    double a = 0, b = 0;
    char ch = ' ';
    double sum = 0;
    in >> N;
    for (int i = 0; i < N; i++)
    {
        in >> ch;
        if (ch == 'L')
        {
            in >> a >> b;
            Linear a1(a, b);
            //a1.print();
            sum += a1.principalRoot();
            //cout << a1.principalRoot() << endl;
        }
        if (ch == 'T')
        {
            in >> a;
            Trigonometric a2(a);
            //a2.print();
            sum += a2.principalRoot();
            //cout << a2.principalRoot() << endl;
        }
        
    }
    out << fixed << setprecision(2) << sum << endl;
}

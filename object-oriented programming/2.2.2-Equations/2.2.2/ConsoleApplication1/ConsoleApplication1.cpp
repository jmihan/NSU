#include <iostream>
#include <cmath>
using namespace std;
class Equation
{
protected:

public:
    virtual double principalRoot() = 0;
    virtual void print() = 0;
};
class Linear : public Equation
{
private:
    double a, b;
public:
    Linear(double a, double b) : a(a), b(b) {}
    double principalRoot()
    {
        return (- b / a);
    }
    void print()
    {
        cout << a << "x + " << b << " = 0" << endl;
    }
};
class Trigonometric : public Equation
{
private:
    double k;
public:
    Trigonometric(double k) : k(k) {}
    double principalRoot()
    {
        return abs(asin(k));
    }
    void print()
    {
        cout << "sin(x) - " << k << " = 0" << endl;
    }
};
class Disjunction : public Equation
{
private:
    double a, b;
public:
    double sum()
    {

    }
};
int main()
{
    int N = 0;
    double a = 0, b = 0;
    char ch = ' ';
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        cin >> ch;
        if (ch == 'L')
        {
            cin >> a >> b;
            Linear a(a, b);
            a.print();
            cout << a.principalRoot() << endl;
        }
        if (ch == 'T')
        {
            cin >> a;
            Trigonometric a(a);
            a.print();
            cout << a.principalRoot() << endl;
        }
        
    }
}

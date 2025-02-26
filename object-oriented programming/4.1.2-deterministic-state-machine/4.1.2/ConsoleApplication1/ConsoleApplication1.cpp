#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
ifstream in("input.txt", ios::in);
ofstream out("output.txt", ios::out);
typedef struct transition { //функция перехода
    int from; //из какого состояния
    int to; //в какое состояние
    char value; //символ, соответствующий значению функции перехода
} tran;
string strr;
bool rez = false;
class automat
{
private:
    int n; //количество состояний автомата
    int k; //номер начального состояния
    int f; //количество конечных состояний
    int* mas_f; //массив конечных состояний
    int p; //количество функций переходов
    tran* mas_p; //массив функций переходов
    int state; //ДКА - 1, НКА - 0, неизвестно - -1
    vector<char> sigma; //алфавит
public:
    automat(): n(0), k(0), f(0), mas_f(nullptr), p(0), mas_p(nullptr), state(-1) {}
    automat(int n, int k, int f, int* mas_f, int p, tran* mas_p) :n(n), k(k), f(f), mas_f(mas_f), p(p), mas_p(mas_p), state(-1) { check(); createSigma(); }
    automat(int n, int k, int f, int p, int* mas_f, tran* mas_p) : n(n), k(k), f(f), p(p), state(-1)
    {
        this->mas_f = new int[f];
        swap(this->mas_f, mas_f);
        this->mas_p = new tran[p];
        swap(this->mas_p, mas_p);
        check();
    }
    ~automat() { delete[] mas_f; delete[] mas_p; }
    
    int getN() { return n; }
    int getK() { return k; }
    int getF() { return f; }
    int* getMas_f() { return mas_f; }
    int getP() { return p; }
    tran* getMas_p() { return mas_p; }
    int getState() { return state; }
    vector<char> getSigma() { return sigma; }

    void setN(int n) { this->n = n; }
    void setK(int k) { this->k = k; }
    void setF(int f) { this->f = f; }
    void setMas_f(int* mas_f) { this->mas_f = mas_f; }
    void setP(int p) { this->p = p; }
    void setMas_p(tran* mas_p) { this->mas_p = mas_p; }
    void setState(int state) { this->state = state; }
    void setSigma(vector<char> sigma) { this->sigma = sigma; }

    
    bool recognition(string str)
    {
        int vertex = k;
        bool flag = 0;
        if (state == 1)
        {
            return checkWord(str, k);
        }
        else
        {
            vertex = k;
            string str1 = str;
            strr.clear();
            rez = false;
            checkWordNka(str, vertex, strr, 0);
            return rez;
        }
        return false;
    }
    bool checkWordNka(string str, int vertex, string strr, int ind)
    {
        for (int i = 0; i < p; i++)
        {
            if ((mas_p[i].from == vertex) && (mas_p[i].value == str[ind]))
            {
                strr += str[ind];
                ind++;
                vertex = mas_p[i].to;
                for (int j = 0; j < f; j++)
                {
                    if ((vertex == mas_f[j]) && (strr == str))
                    {
                        rez = true;
                        return true;
                    }
                }
                if (rez)
                {
                    return true;
                }
                if (!checkWordNka(str, vertex, strr, ind))
                {
                    ind--;
                    vertex = mas_p[i].from;
                    strr.erase(ind, 1);
                }
                else
                {
                    return true;
                }

            }
        }
        return false;
    }
    bool checkWord(string str, int vertex)
    {
        int flag = 0;
        for (int i = 0; i < str.length(); i++)
        {
            for (int j = 0; j < p; j++)
            {
                if ((mas_p[j].from == vertex) && (mas_p[j].value == str[i]))
                {
                    vertex = mas_p[j].to;
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                return false;
            }
            flag = 0;
        }
        for (int i = 0; i < f; i++)
        {
            if (vertex == mas_f[i])
            {
                return true;
            }
        }
        return false;
    }
    void check()
    {
        for (int i = 0; i < p; i++)
        {
            for (int j = (i + 1); j < p; j++)
            {
                if ((mas_p[i].from == mas_p[j].from) && (mas_p[i].value == mas_p[j].value))
                {
                    state = 0;
                    return;
                }
            }
        }
        state = 1;
    }
    void createSigma()
    {
        bool fl = false;
        for (int i = 0; i < p; i++)
        {
            for (int j = 0; j < sigma.size(); j++)
            {
                if (mas_p[i].value == sigma[j])
                {
                    fl = true;
                }
            }
            if (!fl)
            {
                sigma.push_back(mas_p[i].value);
            }
            fl = false;
        }
    }


    vector<int> FromStateToStates(int from, char term) //вектор вершин, в которые можно попасть по term
    {
        vector<int> nextStates;
        bool flag = false;
        for (int i = 0; i < p; i++)
        {
            if ((mas_p[i].from == from) && (mas_p[i].value == term))
            {
                nextStates.push_back(mas_p[i].to);
                flag = true;
            }
        }
        if (flag)
        {
            return nextStates;
        }
        return nextStates;
    }
    vector<int> move(vector<int> currStates, char term) //в какие вершины можно попасть по term
    {
        vector<int> reachState;
        vector<int> nextState;
        for (int i = 0; i < currStates.size(); i++)
        {
            nextState = FromStateToStates(currStates[i], term);
            if (!nextState.empty())
            {
                bool fl = false;
                for (int j = 0; j < nextState.size(); j++)
                {
                    for (int k = 0; k < reachState.size(); k++)
                    {
                        if (nextState[j] == reachState[k])
                        {
                            fl = true;
                            break;
                        }
                    }
                    if (!fl)
                    {
                        reachState.push_back(nextState[j]);
                    }
                    fl = false;
                }
            }
        }
        return reachState;
    }
    void determination()
    {
        vector<int> que; //очередь
        vector<int> currStates; //множество состояний из очереди
        vector<int> newStates; //множество состояний, в которые можно попасть из currStates
        vector<int> Qd; //множество состояний построенного дка
        vector<tran> deltaD; //множество функций перехода построенного дка
        int countDeltaD = 0; //количество функций перехода
        bool st = true;
        que.push_back(k);
        while (!que.empty())
        {
            for (int i = 0; i < n; i++)
            {
                if (mas_p[i].from == que[0])
                {
                    currStates.push_back(mas_p[i].to);
                }
            }
            que.erase(que.begin() + 0);
            for (int i = 0; i < sigma.size(); i++)
            {
                newStates = move(currStates, sigma[i]);
                if (st)
                {
                    for (int j = 0; j < currStates.size(); j++)
                    {
                        Qd.push_back(currStates[j]);
                    }
                    st = false;
                }
                for (int j = 0; j < newStates.size(); j++)
                {
                    que.push_back(newStates[j]);
                }

                if (!newStates.empty())
                {
                    tran el;
                    for (int j = 0; j < currStates.size(); j++)
                    {
                        el.from = currStates[j];
                        el.to = newStates[j];
                        el.value = sigma[i];
                        deltaD.push_back(el);
                        countDeltaD++;
                    }
                }
            }
        }
    }
    automat comb(automat& nka1, automat& nka2)
    {
        tran* transitio2 = nka2.getMas_p();
        tran* transitio1 = nka1.getMas_p();
        tran a;
        for (int i = 0; i < nka2.getP(); i++)
        {
            if (transitio2->from > 0)
            {
                a.from = transitio2->from;
            }
            if (transitio2->to > 0)
            {
                a.to = transitio2->to;
                a.value = transitio2->value;
            }
            tran* transitio1n = new tran[nka1.getP() + 1];
            for (int i = 0; i < nka1.getP(); i++)
            {
                transitio1n[i] = transitio1[i];
            }
            transitio1n[nka1.getP()] = a;
            swap(transitio1, transitio1n);
            delete[] transitio1n;
            nka1.setP(nka1.getP() + 1);
        }
        int* end1 = nka1.getMas_f();
        int* end2 = nka2.getMas_f();
        for (int i = 0; i < nka2.getF(); i++)
        {
            if (end2[i] > 0)
            {
                int* end1n = new int[nka1.getF() + 1];
                for (int i = 0; i < nka1.getF(); i++)
                {
                    end1n[i] = end1[i];
                }
                end1n[nka1.getF()] = end2[i];
                swap(end1, end1n);
                delete[] end1n;
                nka1.setF(nka1.getF() + 1);
            }
        }
        nka1.setMas_f(end1);
        nka1.setMas_p(transitio1);
        return nka1;
    }
    automat loop(automat& nkaRes, automat& nka, int n) //nka автомат, построенный по выражению в скобках
    {
        tran* tr = nka.getMas_p();
        tr[0].from = n;
        tr[nka.getP() - 1].to = n;
        for (int i = 0; i < nka.getP(); i++)
        {
            tran a;
            a.from = tr[i].from;
            a.to = tr[i].to;
            a.value = tr[i].value;
            tran* tr1 = nkaRes.getMas_p();
            tran* tr1n = new tran[nkaRes.getP() + 1];
            for (int j = 0; j < nkaRes.getP(); j++)
            {
                tr1n[i] = tr1[i];
            }
            tr1[nkaRes.getP()] = a;
            swap(tr1, tr1n);
            delete[] tr1n;
            nkaRes.setP(nkaRes.getP() + 1);
            nkaRes.setMas_p(tr1);
        }
        return nkaRes;
    }
    automat RegularExpressions(string& str, int& state, vector<char>& sigm)
    {
        automat resultNka;
        int pos = str.find_first_of('|');
        if (pos != -1)
        {
            int first = 0;
            int second = 0;
            string str1 = str.substr(0, pos); //запоминаем левую часть от |
            automat nka1 = RegularExpressions(str1, first, sigm);
            string str2 = str.substr(pos + 1); //запоминаем правую часть от |
            automat nka2 = RegularExpressions(str2, second, sigm);
            resultNka = comb(nka1, nka2); //соединяем два полученных нка в один
            int* end = resultNka.getMas_f();
            int* endn = new int[resultNka.getF() + 2];
            for (int i = 0; i < resultNka.getF(); i++)
            {
                endn[i] = end[i];
            }
            endn[resultNka.getF()] = first;
            endn[resultNka.getF() - 1] = second;
            swap(end, endn);
            delete[] endn;
            resultNka.setF(resultNka.getF() + 2);
            resultNka.setMas_f(end);
        }
        else
        {
            int count = 0;
            for (int i = 0; i < str.length(); i++)
            {
                if (str[i] != '(')
                {
                    tran a;
                    a.from = state;
                    a.to = state + count + 1;
                    a.value = str[i];
                    bool fl = 0;
                    for (int j = 0; j < sigm.size(); j++) //есть ли str[i] в сигме
                    {
                        if (sigm[j] == str[i])
                        {
                            fl = 1;
                            break;
                        }
                    }
                    if (!fl)
                    {
                        sigm.push_back(str[i]);
                    }
                    fl = 0;
                    tran* tr = resultNka.getMas_p();
                    tran* trn = new tran[resultNka.getP() + 1];
                    for (int j = 0; j < resultNka.getP(); j++)
                    {
                        trn[j] = tr[j];
                    }
                    trn[resultNka.getP()] = a;
                    swap(tr, trn);
                    delete[] trn;
                    resultNka.setP(resultNka.getP() + 1);
                    resultNka.setMas_p(tr);
                    state++;
                }
                else
                {
                    string str1 = str.substr(i + 1);
                    int indSk = str.find_first_of(')');
                    str1 = str1.substr(0, indSk);
                    int n = state;
                    automat l = RegularExpressions(str1, n, sigm); //строим автомат выражения в скобках
                    resultNka = loop(resultNka, l, state); //присоединяем полученный автомат к окончательному автомату
                    i += indSk + 2;
                    count++;
                }
            }
        }
        return resultNka;
    }
};
void makeDkaFromRegularExpressions();
void makeDkaFromExpressions();
int main()
{
    int n = 0; in >> n;
    int k = 0; in >> k;
    int f = 0; in >> f;
    int* mas_f = new int[f];
    for (int i = 0; i < f; i++)
    {
        in >> mas_f[i];
    }
    int p = 0; in >> p;
    tran* mas_p = new tran[p];
    for (int i = 0; i < p; i++)
    {
        in >> mas_p[i].from >> mas_p[i].to >> mas_p[i].value;
    }
    automat a(n, k, f, mas_f, p, mas_p);
    int kolStr = 0; in >> kolStr;
    string str;
    for (int i = 0; i < kolStr; i++)
    {
        in >> str;
        if (a.recognition(str))
        {
            out << "YES" << endl;
        }
        else
        {
            out << "NO" << endl;
        }
        
    }
    //makeDkaFromRegularExpressions();
    return 0;
}
void makeDkaFromExpressions()
{
    string str;
    in >> str;
    automat b;
    vector<char> sigma;
    bool fl = false;
    for (int i = 0; i < str.length(); i++)
    {
        for (int j = 0; j < sigma.size(); j++)
        {
            if (str[i] == sigma[j])
            {
                fl = true;
                break;
            }
        }
        if (!fl)
        {
            sigma.push_back(str[i]);
        }
        fl = false;
    }
    b.setN(str.length() + 1);
    b.setK(0);
    b.setF(1);
    int* mas_f = new int[1];
    mas_f[0] = str.length();
    b.setMas_f(mas_f);
    b.setP(str.length());
    vector<tran> mas_p;
    tran el;
    for (int i = 0; i < str.length(); i++)
    {
        el.from = i;
        el.to = i + 1;
        el.value = str[i];
        mas_p.push_back(el);
        for (int j = 0; j < sigma.size(); j++)
        {
            if (sigma[j] != str[i])
            {
                el.from = i;
                el.to = i;
                el.value = sigma[j];
                mas_p.push_back(el);
            }
        }
    }
    tran* mas_p1 = new tran[mas_p.size()];
    for (int i = 0; i < mas_p.size(); i++)
    {
        mas_p1[i].from = mas_p[i].from;
        mas_p1[i].to = mas_p[i].to;
        mas_p1[i].value = mas_p[i].value;
    }
    b.setP(mas_p.size());
    b.setMas_p(mas_p1);
}
void makeDkaFromRegularExpressions()
{
    string str;
    vector<char> sigma;
    in >> str;
    automat b;
    int start = 0;
    b = b.RegularExpressions(str, start, sigma);
    b.determination();
}
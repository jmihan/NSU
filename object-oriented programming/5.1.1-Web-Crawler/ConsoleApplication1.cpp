#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <chrono>
#include <mutex>
#include <map>
#include <vector>
using namespace std;
int countVisitedSites = 0;
int couThreads = 0;
mutex m;
mutex m1;
mutex m2;
map<string, int> url_map;
ifstream in("input.txt", ios::in);
ofstream out("output.txt", ios::out);
void parser(string);
void copyFile(string, string);
int* max_border_array(string);
vector<int> KMP(string, string, int*); //Кнута — Морриса — Пратта
int main()
{
    auto begin = chrono::steady_clock::now();
    
    int countThread = 0;
    string str1;
    in >> str1;
    in >> countThread;
    couThreads = countThread;
    str1.erase(0, 7);
    str1 = "test_data/" + str1;

    countThread = 0;
    vector<thread> ths;
    ifstream in1(str1, ios::in);
    string str2; //весь файл
    while (getline(in1, str2)) {}
    copyFile(str1, str2);
    int i = 0;


    
   /* auto beginKmp = chrono::steady_clock::now();
    vector<int> index;
    index = KMP(str2, "<a href=", NULL);
    auto endKmp = chrono::steady_clock::now();
    auto elapsed_msKmp = chrono::duration_cast<chrono::milliseconds>(endKmp - beginKmp);
    cout << "The time: " << elapsed_msKmp.count() << " ms\n";*/
    /*
    for (int i = 0; i < index.size(); i++)
    {
        str1.clear();
        int pos = str2.find('"', index[i] + 3);
        str1 = str2.substr(index[i] + 9, pos - index[i] + 7);
        while (str1[str1.length() - 1] != 'l')
        {
            str1.pop_back();
        }
        str1.erase(0, 7);
        str1 = "test_data/" + str1;
        if (url_map.count(str1))
        {
            continue;
        }
        url_map[str1] = 1;
        if (couThreads > 0)
        {
            thread th(parser, str1);
            countThread++;
            couThreads--;
            ths.emplace_back(move(th));
        }
        else
        {
            parser(str1);
        }
    }*/

    
    while (i != -1)
    {
        i = str2.find("<a href=");
        if (i == -1)
        {
            couThreads++;
            break;
        }
        i += 9;
        str1.clear();
        str2.erase(0, i);
        int pos = str2.find("\">");
        if (pos == -1)
        {
            continue;
        }
        str1 = str2.substr(0, pos);
        str1.erase(0, 7);
        str1 = "test_data/" + str1;

        if (url_map.count(str1))
        {
            continue;
        }
        url_map[str1] = 1;
        if (couThreads > 0)
        {
            thread th(parser, str1);
            countThread++;
            couThreads--;
            ths.emplace_back(move(th));
        }
        else
        {
            parser(str1);
        }
    }

    for (int i = 0; i < countThread; i++)
    {
        ths[i].join();
    }
    
    auto end1 = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end1 - begin);
    cout << countVisitedSites << endl;
    cout << "The time: " << elapsed_ms.count() << " ms\n";
    in.close();
    out.close();
    return 0;
}
void parser(string str)
{
    int i = 0;
    try
    {
        ifstream in1(str, ios::in);
    }
    catch(...)
    {
        return;
    }
    ifstream in1(str, ios::in);
    string str2; //весь файл
    while (getline(in1, str2)) {}
    m.lock();
    copyFile(str, str2);
    countVisitedSites++;
    m.unlock();
    
    /*vector<int> index;
    index = KMP(str2, "<a href=", NULL);
    for (int i = 0; i < index.size(); i++)
    {
        str.clear();
        int pos = str2.find('"', index[i] + 3);
        str = str2.substr(index[i] + 9, pos - index[i] + 7);
        while (str[str.length() - 1] != 'l')
        {
            str.pop_back();
        }
        str.erase(0, 7);
        str = "test_data/" + str;
        if (url_map.count(str))
        {
            continue;
        }
        url_map[str] = 1;
        if (couThreads > 0)
        {
            thread th(parser, str);
            m2.lock();
            couThreads--;
            m2.unlock();
            th.join();
        }
        else
        {
            parser(str);
        }
    }*/


    while (i != -1)
    {
        i = str2.find("<a href=");
        if (i == -1)
        {
            m1.lock();
            couThreads++;
            m1.unlock();
            break;
        }
        i += 9;
        str.clear();
        str2.erase(0, i);
        int pos = str2.find("\">");
        if (pos == -1)
        {
            continue;
        }
        str = str2.substr(0, pos);
        str.erase(0, 7);
        str = "test_data/" + str;

        if (url_map.count(str))
        {
            continue;
        }
        url_map[str] = 1;
        if (couThreads > 0)
        {
            thread th(parser, str);
            m2.lock();
            couThreads--;
            m2.unlock();
            th.join();
        }
        else
        {
            parser(str);
        }
    }
    in1.close();
}
void copyFile(string url, string sorce)
{
    url.erase(0, 10);
    url = "new_data\\" + url;
    
    ofstream out1(url, ios::out);

    out1 << sorce << endl;
    out1.close();
}
int* max_border_array(string T) 
{
    int i, t;
    int n = T.length();
    int* br = new int[n];
    br[0] = 0;
    for (i = 1; i < n; i++) 
    {
        t = br[i - 1];
        while ((t > 0) && (T[i] != T[t])) 
        {
            t = br[t - 1];
        }
        if (T[i] == T[t]) 
        {
            br[i] = t + 1;
        }
        else 
        {
            br[i] = 0;
        }
    }
    return br;
}
vector<int> KMP(string str, string sub, int* br) 
{
    vector<int> solution;
    int poz = 0;
    br = max_border_array(str);
    for (int i = 0; i < str.length(); i++)
    {
        while (poz == sub.size() || (poz > 0 && sub[poz] != str[i])) 
        {
            poz = br[poz - 1];
            if ((sub.length() - poz) > (str.length() - i))
            {
                break;
            }
        }
        if (str[i] == sub[poz]) 
        {
            poz++;
        }
        if (poz == sub.size()) 
        {
            solution.push_back(i - poz + 1);
        }
    }
    return solution;
}
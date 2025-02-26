#include <iostream>
#include <fstream>
#include <string>
using namespace std;
ifstream in("input.txt", ios::in);
ofstream out("output.txt", ios::out);

template <typename K, typename V>
class HashMap {
protected:
    int size; //размер таблицы
    int count; //количество элементов в таблице (без удаленных)
    int allCount; //количество элементов в таблице (с удаленными)
    int sizeM; //размер матриц
    const double resize_koef = 0.75; //коэффициент для изменения размера таблицы

    class pair {
    private:
        K key;
        V value;
        bool state; //false - пара удалена
    public:
        pair(K key, V value) : key(key), value(value), state(true) {}
        pair() {}
        ~pair() {}
        K getKey() { return this->key; }
        V getValue() { return this->value; }
        bool getState() { return this->state; }

        void changeState(bool state) { this->state = state; }
        void changeValue(V value) { this->value = value; }

        pair& operator= (V value) //присваивание значения
        {
            this->value = value;
            return *this;
        }
        pair& operator= (const pair& that) //присваивание пары
        {
            this->key = that.key;
            this->value = that.value;
            return *this;
        }
        void print()
        {
            cout << this->key << " " << this->value << endl;
        }
    };

    
    void setMemory(int size) //выделение памяти
    {
        table = new pair*[size];
        this->size = size;
        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }
    void addMemory() //добавление памяти
    {
        count = 0;
        allCount = 0;
        pair** newTable = new pair*[size * 2];
        this->changeSize(size * 2);
        for (int i = 0; i < size; i++)
        {
            newTable[i] = nullptr;
        }
        swap(table, newTable);
        for (int i = 0; i < (size / 2); i++)
        {
            if ((newTable[i]) && (newTable[i]->getState()))
            {
                Add(newTable[i]->getKey(), newTable[i]->getValue());
            }
        }

        for (int i = 0; i < (size / 2); i++)
        {
            if (newTable[i])
            {
                delete newTable[i];
            }
        }
        delete[] newTable;
    }
    void rehash() //если процент существующих элементов стал меньше 50
    {
        count = 0;
        allCount = 0;
        pair** newTable = new pair * [size];
        this->changeSize(size);
        for (int i = 0; i < size; i++)
        {
            newTable[i] = nullptr;
        }
        swap(table, newTable);
        for (int i = 0; i < size; i++)
        {
            if ((newTable[i]) && (newTable[i]->getState()))
            {
                Add(newTable[i]->getKey(), newTable[i]->getValue());
            }
        }

        for (int i = 0; i < size; i++)
        {
            if (newTable[i])
            {
                delete newTable[i];
            }
        }
        delete[] newTable;
    }
    pair** table; //массив пар
public:
    HashMap(int size) : size(size), count(0), allCount(0), sizeM(0) { this->setMemory(size); }
    ~HashMap()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i])
            {
                delete table[i];
            }
        }
        delete[] table;
    }

    int getSize() { return size; }
    int getCount() { return count; }
    int getAllCount() { return allCount; }

    int getHash(K key) { return hash<K>()(key) % size; }
    int getHash1(K key) { return (hash<K>()(key) % 5 + 1) % size; } //для двойного хеширования
    
    //доп задание
    /*int getHashMatrix(K key) 
    { 
        int hsh = 0;
        for (int i = 0; i < sizeM; i++)
        {
            hsh += key[i][i];
        }
        return (hsh * 13) % size;
    }
    int getHashMatrix1(K key)
    {
        int hsh = 0;
        for (int i = 0; i < sizeM; i++)
        {
            hsh += key[i][i];
        }
        return ((hsh * 13) % 5 + 1) % size;
    }*/

    void changeSize(int size) { this->size = size; }
    void changeCount(int count) { this->count = count; }
    void changeAllCount(int allCount) { this->allCount = allCount; }
    void changeSizeM(int size) { this->sizeM = size; }

    int variousValue() //подсчет различных значений
    {
        this->rehash();
        int k = 0;
        bool f = false;
        for (int i = 0; i < size; i++)
        {
            for (int j = (i + 1); j < size; j++)
            {
                if (table[i] && table[j])
                {
                    if (table[i]->getValue() == table[j]->getValue())
                    {
                        f = true;
                        break;
                    }
                }
            }
            if (!f && table[i])
            {
                k++;
            }
            f = false;
        }
        return k;
    }

    pair* Search(K key)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        
        if (table[hsh] == NULL)
        {
            return NULL;
        }
        if (table[hsh] != NULL)
        {
            if (table[hsh]->getKey() == key)
            {
                return table[hsh];
            }
            else
            {
                int hsh1 = 0;
                if (sizeM == 0)
                {
                    hsh1 = getHash1(key);
                }
                else
                {
                    //hsh1 = getHashMatrix1(key);
                }
                int i = (hsh + hsh1) % size;
                while (true)
                {
                    if (table[i]->getKey() == key)
                    {
                        return table[i];
                    }
                    i += hsh1;
                    if (i > size)
                    {
                        //cout << "|||Key not found|||" << endl;
                        return NULL;
                    }
                }
            }
        }
    }
    void Add(K key, V value)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        if (table[hsh] == NULL)
        {
            table[hsh] = new pair(key, value);
            count++;
            allCount++;
            return;
        }
        if (table[hsh]->getKey() == key)
        {
            table[hsh]->changeValue(value);
            return;
        }
        else
        {
            int hsh1 = 0;
            if (sizeM == 0)
            {
                hsh1 = getHash1(key);
            }
            else
            {
                //hsh1 = getHashMatrix1(key);
            }
            int i = (hsh + hsh1) % size;
            while (true)
            {
                if (table[i] == NULL)
                {
                    table[i] = new pair(key, value);
                    count++;
                    allCount++;
                    return;
                }
                i += hsh1;
                if (i >= size)
                {
                    this->addMemory();
                }
            }
        }
        if ((count / size) >= resize_koef)
        {
            this->addMemory();
        }
    }
    void deleteEl(K key)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        if (table[hsh] == NULL)
        {
            //cout << "|||Key not found|||" << endl;
            return;
        }
        if (table[hsh] != NULL)
        {
            if (table[hsh]->getKey() == key)
            {
                table[hsh]->changeState(false);
                count--;
                if ((count / allCount) <= 0.5)
                {
                    this->rehash();
                }
                return;
            }
            else
            {
                int hsh1 = 0;
                if (sizeM == 0)
                {
                    hsh1 = getHash1(key);
                }
                else
                {
                    //hsh1 = getHashMatrix1(key);
                }
                int i = (hsh + hsh1) % size;
                while (true)
                {
                    if (table[i]->getKey() == key)
                    {
                        table[i]->changeState(false);
                        count--;
                        if ((count / allCount) <= 0.5)
                        {
                            this->rehash();
                        }
                        return;
                    }
                    i += hsh1;
                    if (i > size)
                    {
                        //cout << "|||Key not found|||" << endl;
                        return;
                    }
                }
            }
        }
    }
    HashMap& operator=(const HashMap& that)
    {
        this->~HashMap();
        this->setMemory(that.size);
        this->size = that.size;
        this->count = that.count;
        for (int i = 0; i < size; i++)
        {
            if (table[i] != NULL)
            {
                this->Add(that.table[i]->getKey(), that.table[i]->getValue());
            }
        }
        return *this;
    }
    void printmap()
    {
        this->rehash();
        for (int i = 0; i < this->getSize(); i++)
        {
            if (table[i])
            {
                table[i]->print();
            }
        }
    }
    class iterator
    {
    private:
        HashMap<K, V>& map;
        pair* mas; 
        int hsh;
    public:
        iterator(HashMap<K, V> map, pair* mas, int hsh) : map(map), mas(mas), hsh(hsh) {}
        iterator& operator++ ()
        {
            hsh++;
            mas = NULL;
            for (hsh; hsh < map.getSize(); hsh++)
            {
                if (map.table[hsh] != NULL)
                {
                    mas = map.table[hsh];
                    break;
                }
            }
            return *this;
        }
        pair* operator-> () { return mas; }
    };
    iterator start()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != NULL)
            {
                return iterator(*this, table[i], i);
            }
        }
        return iterator(*this, NULL, (size - 1));
    }
    iterator theEnd() { return iterator(*this, NULL, (size - 1)); }
};

template <typename K, typename V>
class MultiHashMap {
protected:
    int size; //размер таблицы
    int count; //количество элементов в таблице (без удаленных)
    int allCount; //количество элементов в таблице (с удаленными)
    int sizeM; //размер матриц
    const double resize_koef = 0.75; //коэффициент для изменения размера таблицы

    class pair {
    private:
        K key;
        V value;
        bool state; //false - пара удалена
    public:
        pair(K key, V value) : key(key), value(value), state(true) {}
        pair() {}
        ~pair() {}
        K getKey() { return this->key; }
        V getValue() { return this->value; }
        bool getState() { return this->state; }

        void changeState(bool state) { this->state = state; }
        void changeValue(V value) { this->value = value; }

        pair& operator= (V value) //присваивание значения
        {
            this->value = value;
            return *this;
        }
        pair& operator= (const pair& that) //присваивание пары
        {
            this->key = that.key;
            this->value = that.value;
            return *this;
        }
        void print()
        {
            cout << this->key << " " << this->value << endl;
        }
    };


    void setMemory(int size) //выделение памяти
    {
        table = new pair * [size];
        this->size = size;
        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }
    void addMemory() //добавление памяти
    {
        count = 0;
        allCount = 0;
        pair** newTable = new pair * [size * 2];
        this->changeSize(size * 2);
        for (int i = 0; i < size; i++)
        {
            newTable[i] = nullptr;
        }
        swap(table, newTable);
        for (int i = 0; i < (size / 2); i++)
        {
            if ((newTable[i]) && (newTable[i]->getState()))
            {
                Add(newTable[i]->getKey(), newTable[i]->getValue());
            }
        }

        for (int i = 0; i < (size / 2); i++)
        {
            if (newTable[i])
            {
                delete newTable[i];
            }
        }
        delete[] newTable;
    }
    void rehash() //если процент существующих элементов стал меньше 50
    {
        count = 0;
        allCount = 0;
        pair** newTable = new pair * [size];
        this->changeSize(size);
        for (int i = 0; i < size; i++)
        {
            newTable[i] = nullptr;
        }
        swap(table, newTable);
        for (int i = 0; i < size; i++)
        {
            if ((newTable[i]) && (newTable[i]->getState()))
            {
                Add(newTable[i]->getKey(), newTable[i]->getValue());
            }
        }

        for (int i = 0; i < size; i++)
        {
            if (newTable[i])
            {
                delete newTable[i];
            }
        }
        delete[] newTable;
    }
    pair** table; //массив пар
public:
    MultiHashMap(int size) : size(size), count(0), allCount(0), sizeM(0) { this->setMemory(size); }
    ~MultiHashMap()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i])
            {
                delete table[i];
            }
        }
        delete[] table;
    }

    int getSize() { return size; }
    int getCount() { return count; }
    int getAllCount() { return allCount; }

    int getHash(K key) { return hash<K>()(key) % size; }
    int getHash1(K key) { return (hash<K>()(key) % 5 + 1) % size; } //для двойного хеширования

    //доп задание
    /*int getHashMatrix(K key)
    {
        int hsh = 0;
        for (int i = 0; i < sizeM; i++)
        {
            hsh += key[i][i];
        }
        return (hsh * 13) % size;
    }
    int getHashMatrix1(K key)
    {
        int hsh = 0;
        for (int i = 0; i < sizeM; i++)
        {
            hsh += key[i][i];
        }
        return ((hsh * 13) % 5 + 1) % size;
    }*/

    void changeSize(int size) { this->size = size; }
    void changeCount(int count) { this->count = count; }
    void changeAllCount(int allCount) { this->allCount = allCount; }
    void changeSizeM(int size) { this->sizeM = size; }

    int variousValue() //подсчет различных значений
    {
        this->rehash();
        int k = 0;
        bool f = false;
        for (int i = 0; i < size; i++)
        {
            for (int j = (i + 1); j < size; j++)
            {
                if (table[i] && table[j])
                {
                    if (table[i]->getValue() == table[j]->getValue())
                    {
                        f = true;
                        break;
                    }
                }
            }
            if (!f && table[i])
            {
                k++;
            }
            f = false;
        }
        return k;
    }

    pair* Search(K key)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }

        if (table[hsh] == NULL)
        {
            return NULL;
        }
        if (table[hsh] != NULL)
        {
            if (table[hsh]->getKey() == key)
            {
                return table[hsh];
            }
            else
            {
                int hsh1 = 0;
                if (sizeM == 0)
                {
                    hsh1 = getHash1(key);
                }
                else
                {
                    //hsh1 = getHashMatrix1(key);
                }
                int i = (hsh + hsh1) % size;
                while (true)
                {
                    if (table[i]->getKey() == key)
                    {
                        return table[i];
                    }
                    i += hsh1;
                    if (i > size)
                    {
                        //cout << "|||Key not found|||" << endl;
                        return NULL;
                    }
                }
            }
        }
    }
    void Add(K key, V value)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        if (table[hsh] == NULL)
        {
            table[hsh] = new pair(key, value);
            count++;
            allCount++;
            return;
        }
        else
        {
            int hsh1 = 0;
            if (sizeM == 0)
            {
                hsh1 = getHash1(key);
            }
            else
            {
                //hsh1 = getHashMatrix1(key);
            }
            int i = (hsh + hsh1) % size;
            while (true)
            {
                if (table[i] == NULL)
                {
                    table[i] = new pair(key, value);
                    count++;
                    allCount++;
                    return;
                }
                i += hsh1;
                if (i >= size)
                {
                    this->addMemory();
                }
            }
        }
        if ((count / size) >= resize_koef)
        {
            this->addMemory();
        }
    }
    void deleteEl(K key)
    {
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        if (table[hsh] == NULL)
        {
            //cout << "|||Key not found|||" << endl;
            return;
        }
        if (table[hsh] != NULL)
        {
            if (table[hsh]->getKey() == key)
            {
                table[hsh]->changeState(false);
                count--;
                if ((count / allCount) <= 0.5)
                {
                    this->rehash();
                }
                int hsh1 = 0;
                if (sizeM == 0)
                {
                    hsh1 = getHash1(key);
                }
                else
                {
                    //hsh1 = getHashMatrix1(key);
                }
                int i = (hsh + hsh1) % size;
                while (true)
                {
                    if (table[i]->getKey() == key)
                    {
                        table[i]->changeState(false);
                        count--;
                        if ((count / allCount) <= 0.5)
                        {
                            this->rehash();
                        }
                    }
                    i += hsh1;
                    if (i > size)
                    {
                        //cout << "|||Key not found|||" << endl;
                        return;
                    }
                }
                return;
            }
            else
            {
                int hsh1 = 0;
                if (sizeM == 0)
                {
                    hsh1 = getHash1(key);
                }
                else
                {
                    //hsh1 = getHashMatrix1(key);
                }
                int i = (hsh + hsh1) % size;
                while (true)
                {
                    if (table[i]->getKey() == key)
                    {
                        table[i]->changeState(false);
                        count--;
                        if ((count / allCount) <= 0.5)
                        {
                            this->rehash();
                        }
                    }
                    i += hsh1;
                    if (i > size)
                    {
                        //cout << "|||Key not found|||" << endl;
                        return;
                    }
                }
            }
        }
    }
    int countElem(K key)
    {
        this->rehash();
        int hsh = 0;
        if (sizeM == 0)
        {
            hsh = getHash(key);
        }
        else
        {
            //hsh = getHashMatrix(key);
        }
        if (table[hsh] == NULL)
        {
            return 0;
        }
        int k = 0;
        int hsh1 = 0;
        if (sizeM == 0)
        {
            hsh1 = getHash1(key);
        }
        else
        {
            //hsh1 = getHashMatrix1(key);
        }
        int i = (hsh + hsh1) % size;
        while (true)
        {
            if (table[i]->getKey() == key)
            {
                k++;
            }
            i += hsh1;
            if (i > size)
            {
                //cout << "|||Key not found|||" << endl;
                return k;
            }
        }
    }
    pair** getAllPair(K key)
    {
        pair** mas = new pair * [this->countElem(key)];
        int k = 0;
        for (int i = 0; i < size; i++)
        {
            if (table[i]->getKey() == key)
            {
                mas[k] = table[i];
                k++;
            }
        }
        return mas;
    }
    MultiHashMap& operator=(const MultiHashMap& that)
    {
        this->~HashMap();
        this->setMemory(that.size);
        this->size = that.size;
        this->count = that.count;
        for (int i = 0; i < size; i++)
        {
            if (table[i] != NULL)
            {
                this->Add(that.table[i]->getKey(), that.table[i]->getValue());
            }
        }
        return *this;
    }
    void printmap()
    {
        this->rehash();
        for (int i = 0; i < this->getSize(); i++)
        {
            if (table[i])
            {
                table[i]->print();
            }
        }
    }
    class iterator
    {
    private:
        HashMap<K, V>& map;
        pair* mas;
        int hsh;
    public:
        iterator(HashMap<K, V> map, pair* mas, int hsh) : map(map), mas(mas), hsh(hsh) {}
        iterator& operator++ ()
        {
            hsh++;
            mas = NULL;
            for (hsh; hsh < map.getSize(); hsh++)
            {
                if (map.table[hsh] != NULL)
                {
                    mas = map.table[hsh];
                    break;
                }
            }
            return *this;
        }
        pair* operator-> () { return mas; }
    };
    iterator start()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != NULL)
            {
                return iterator(*this, table[i], i);
            }
        }
        return iterator(*this, NULL, (size - 1));
    }
    iterator theEnd() { return iterator(*this, NULL, (size - 1)); }
};

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
    };
    stolbec operator()(const int& a)
    {
        stolbec stolb(a, this);
        return stolb;
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

template <typename K, typename V>
void start(int);

template <typename K, typename V>
void extra(int);

int main()
{
    int N = 0;
    char k, v;
    in >> k >> v; //k тип ключа, v типа значения
    in >> N; //количество команд
    switch (k)
    {
    case 'I':
        switch (v)
        {
        case 'I':
            start<int, int>(N);
            break;
        case 'D':
            start<int, double>(N);
            break;
        case 'S':
            start<int, string>(N);
            break;
        }
        break;
    case 'D':
        switch (v)
        {
        case 'I':
            start<double, int>(N);
            break;
        case 'D':
            start<double, double>(N);
            break;
        case 'S':
            start<double, string>(N);
            break;
        }
        break;
    case 'S':
        switch (v)
        {
        case 'I':
            start<string, int>(N);
            break;
        case 'D':
            start<string, double>(N);
            break;
        case 'S':
            start<string, string>(N);
            break;
        }
        break;
   /* case 'M':
        switch (v)
        {
        case 'I':
            extra<matrix, int>(N);
            break;
        case 'D':
            extra<matrix, double>(N);
            break;
        case 'S':
            extra<matrix, string>(N);
            break;
        }
        break;*/
    }
    return 0;
}
template <typename K, typename V>
void start(int size)
{
    HashMap<K, V> map(size); //хеш таблица размера size
    char ch;
    K key;
    V value;
    for (int i = 0; i < size; i++)
    {
        in >> ch;
        switch (ch)
        {
        case 'A':
            in >> key >> value;
            map.Add(key, value);
            break;
        case 'R':
            in >> key;
            map.deleteEl(key);
            break;
        }
    }
    out << map.getCount() << ' ' << map.variousValue();
}
/*доп задание*/
//template <typename K, typename V>
//void extra(int size) 
//{
//    HashMap<K, V> map(size);
//    char ch;
//    int n = 0;
//    in >> n;
//    map.changeSizeM(n);
//    int** mas;
//    V value;
//    K matr;
//    for (int i = 0; i < size; i++)
//    {
//        in >> ch;
//        switch (ch)
//        {
//        case 'A':
//            matr(n);
//            mas = new int* [n];
//            for (int i = 0; i < n; i++)
//            {
//                mas[i] = new int[n];
//                for (int j = 0; j < n; j++)
//                {
//                    in >> mas[i][j];
//                }
//            }
//            matr.input(mas);
//            in >> value;
//            map.Add(matr, value);
//            for (int i = 0; i < n; i++)
//            {
//                delete mas[i];
//            }
//            delete[] mas;
//            break;
//        case 'R':
//            matr(n);
//            mas = new int* [n];
//            for (int i = 0; i < n; i++)
//            {
//                mas[i] = new int[n];
//                for (int j = 0; j < n; j++)
//                {
//                    in >> mas[i][j];
//                }
//            }
//            matr.input(mas);
//            map.deleteEl(matr);
//            for (int i = 0; i < n; i++)
//            {
//                delete mas[i];
//            }
//            delete[] mas;
//            break;
//        }
//    }
//    out << map.getCount() << ' ' << map.variousValue();
//}

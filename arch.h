#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

struct para1
{
    para1()
    {
        c = "";
        n = 0;
    }
    para1(std::string _c, int _n)
    {
        c = _c;
        n = _n;
    }
    std::string c;
    int n;
};

bool comp(para1 p) {
    return (p.n == 0);
}

bool if_chislo(char c) {
    return (c <= 57 && c >= 48);
}

void ins(std::vector<para1> &kuda, const std::vector<para1> &chto, int p) {
    kuda.reserve(kuda.size() + chto.size() + 1);
    for (int i = p; i < p + chto.size(); i++) // поддерживаем позицию дл¤ вставки
        kuda.insert(kuda.begin() + i, chto[i - p]);
    return;
}

std::vector<para1> rep(para1 a) {
    int n = a.n / 126;
    if (a.n % 126 != 0)
        n++;
    std::vector<para1> rez;

    if (a.c.size() > 1) {
        int i = 0;
        rez.resize(n);
        for (i = 0; i < n - 1; i++) { // разбиение на подстроки длинной 126
            rez[i].n = 126;
            rez[i].c = a.c.substr(i * 126, 126);
        }

        if (a.c.length() % 126 != 0) { // добавление хвостика
            rez[n - 1].n = a.c.length() % 126;
            rez[n - 1].c = a.c.substr(126 * i, a.c.length() % 126);
        }
    } else { /// если строка повтор¤ющихс¤
        para1 temp;
        for (int i = 0; i < n - 1; i++) {
            temp = para1(a.c, 126);
            rez.push_back(temp);
        }
        temp = para1(a.c, a.n % 126);
        rez.push_back(temp);
    }

    return rez;
}

void preobrazovanie(std::vector<para1> &v) { /// замена в векторе выход¤щих за рамки пар
    int i = 0;

    while (i < (int)v.size())
    {
        // cout << i << " " << v.size() << '\n';
        if (v[i].n > 126)
        {                               // если больше 126
            std::vector<para1> n = rep(v[i]); // замен¤ем пару на множество пар
            v.erase(v.begin() + i);     // удал¤ем материнскую пару
            ins(v, n, i);               // вставл¤ем множество в вектор
        }
        i++;
    }
}

std::vector<para1> arch(const std::string &s)
{ /// преобразование строки к вектору пар

    /// строку к цифрам и числам
    std::vector<para1> rez;
    int i = 0, j = 0;
    while (j < s.size())
    {
        while (s[i] == s[j])
            j++;
        para1 p;
        p.n = (j - i);
        p.c += s[i];
        i = j;
        rez.push_back(p);
    }
    
    /// объединение неповтор¤ющихс¤ в одну пару
    for (int i = rez.size() - 1; i > 0; i--)
    {
        if ((rez[i].n == 1 || rez[i].c.length() > 1) && rez[i - 1].n == 1)
        {
            rez[i - 1].c += rez[i].c; // скатывание шара
            rez[i].c = "";            // удаление следа
            rez[i - 1].n += rez[i].n; // скатывание шара
            rez[i].n = 0;             // отметка
        }
    }
    // удаление отмеченных
    auto negative_it = find_if(rez.begin(), rez.end(), comp);
    while (negative_it != rez.end())
    {
        rez.erase(negative_it);
        negative_it = find_if(rez.begin(), rez.end(), comp);
    }
    
    /// преобразование, если кол-во больше 126
    preobrazovanie(rez);
    
    for (int i = 0; i < rez.size(); i++)
    {
        if (rez[i].n == 27)
            rez[i].n += 100;
    }
    
    /// преобразование, чтобы отличать повтор¤ющиес¤ от неповтор¤ющихс¤
    for (int i = 0; i < rez.size(); i++)
        if (rez[i].c.length() == 1)
            rez[i].n += 128;
    
    return rez;
}

void show(std::vector<std::vector<para1>> &v, std::ostream &out)
{ /// вывод
    v.resize(v.size());
    for (int i = 0; i < v.size(); i++)
        v[i].resize(v[i].size());

    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            
            std::bitset<8> b;
            b = v[i][j].n;
            out << b << ' ';
            for (int z = 0; z < v[i][j].c.length(); z++)
            {
                b = v[i][j].c[z];
                out << b << ' ';
            }
        }
        out << '\n';
    }
}

void arch(std::string inputFile, std::string outputFile)
{
    std::ifstream fin(inputFile);
    std::ofstream fout(outputFile);
    std::vector<std::string> a; /// хранение рисунка - вектор строк
    std::string s;
    while (std::getline(fin, s))
    {
        a.push_back(s);
    }

    a.resize(a.size());

    std::vector<std::vector<para1>> v(a.size(), std::vector<para1>()); // вектор пар - число/строка

    for (int i = 0; i < v.size(); i++)
        v[i] = arch(a[i]);

    show(v, fout);
}

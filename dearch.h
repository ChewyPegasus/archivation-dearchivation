#include <string>
#include <fstream>
#include <vector>

struct para
{
    para()
    {
        c = "";
        n = 0;
    }
    para(std::string _c, int _n)
    {
        c = _c;
        n = _n;
    }
    std::string c;
    int n;
};

int per(const std::string &s)
{ // перевод строки битов в число
    int rez = 0, st = 1;
    for (int i = s.size() - 1; i >= 0; i--)
    {
        if (s[i] == '1')
            rez += st;
        st *= 2;
    }
    return rez;
}

void show(std::ostream &out, const std::vector<std::vector<para>> &a)
{
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < a[i].size(); j++)
        {
            if (a[i][j].n > 128)
            {
                for (int o = 0; o < a[i][j].n - 128; o++)
                    out << a[i][j].c;
            }
            else
                out << a[i][j].c;
        }
        out << '\n';
    }
    return;
}

void dearch(std::string archFile, std::string outFile)
{
    std::ifstream fin(archFile);
    std::ofstream fout(outFile);

    std::string s;

    int n = 0;
    std::vector<std::string> b;
    while (getline(fin, s))
    {
        n++;
        b.push_back(s);
    }

    std::vector<std::vector<int>> a(n);
    fin.clear();
    fin.seekg(0, std::ios::beg);
    for (int i = 0; i < n; i++)
    {
        s = b[i]; // получение строки невыделенных двоичных представлений
        int pos = 0;
        while (s.find(" ", pos) != std::string::npos)
        {
            // выделяем двоичные представления и переводим их
            pos = s.find(" ", pos);
            std::string temp = s.substr(0, pos);

            int m = per(temp);

            a[i].push_back(m);
            s.erase(0, pos + 1);
        }
    }

    /// формирование пар
    std::vector<std::vector<para>> rez(n);
    int i = 0, j = 0;
    while (i < a.size())
    {
        while (j < a[i].size())
        {
            para p;
            p.n = a[i][j];
            if (p.n > 128)
            {
                j++;
                p.c += (char)a[i][j];
            }
            else
            {
                for (int o = 0; o < p.n; o++)
                {
                    j++;
                    p.c += (char)a[i][j];
                }
            }
            j++;
            rez[i].push_back(p);
        }
        i++;
        j = 0;
    }

    show(fout, rez);
}
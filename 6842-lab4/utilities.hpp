#include <bits/stdc++.h>
using namespace std;

class Process
{
private:
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int pid;

public:
    int &operator[](string var)
    {
        if (var == "at")
            return at;
        if (var == "bt")
            return bt;
        if (var == "ct")
            return ct;
        if (var == "tat")
            return tat;
        if (var == "wt")
            return wt;
        return pid;
    }

    void update_after_ct()
    {
        tat = ct - at;
        wt = tat - bt;
    }

    void display()
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", pid, at, bt, ct,
               tat, wt);
    }
};
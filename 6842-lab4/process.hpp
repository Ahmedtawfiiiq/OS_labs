#include <bits/stdc++.h>

using namespace std;

class Process
{
private:
    string at;  // arrive time
    string bt;  // burst time
    string ct;  // completion time
    string tat; // turn around time
    string wt;  // waiting time
    string pid; // process id

public:
    // overloading [] operator
    string &operator[](string var)
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
        return pid; // var = "id"
    }

    void update()
    {
        tat = to_string(stoi(ct) - stoi(at));
        wt = to_string(stoi(tat) - stoi(bt));
    }
};

class Processes
{
public:
    int numberOfProcesses;
    vector<Process> processes;

public:
    void doSimulation(int);
};

void organizeData(Processes &p, vector<vector<string>> inputData);

void doFCFS(Processes &p);
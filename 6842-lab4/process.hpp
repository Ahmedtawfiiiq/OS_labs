#include <bits/stdc++.h>

using namespace std;

class Process
{
public:
    int at;     // 1 arrive time
    int bt;     // 2 burst time
    int ct;     // 3 completion time
    int tat;    // 4 turn around time
    int wt;     // 5 waiting time
    string pid; // 6 process id

    pair<string, int> output;

    Process() : at(0), bt(0), ct(0), tat(0), wt(0), pid(""), output("", 0) {}
};

class Processes
{
public:
    string scheduleName;
    int stopRange;
    double tatMean;
    vector<Process> processes;
    Processes() : scheduleName(""), tatMean(0) {}
};

void organizeData(Processes &, vector<vector<string>>, int);

void stats(Processes);
void trace(Processes);

void doFCFS(Processes &);

void doRR(Processes &, int);

void doSPN(Processes &);

void doSRT(Processes &p);
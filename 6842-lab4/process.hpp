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

    Process() : at(0), bt(0), ct(0), tat(0), wt(0), pid("") {}
};

class Processes
{
public:
    vector<Process> processes;
};

void organizeData(Processes &, vector<vector<string>>);

void doFCFS(Processes &);
void doSimulationFCFS(Processes, int);

void doRR(Processes &, int);
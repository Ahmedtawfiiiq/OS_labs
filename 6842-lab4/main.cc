#include <bits/stdc++.h>

#include "tokenizer.hpp"
#include "process.hpp"

using namespace std;

int main()
{
    string visualizationType = "";
    getline(cin, visualizationType);

    vector<int> typeData;
    typeTokenizer(typeData);

    string s = ""; // when to stop the simulation
    getline(cin, s);
    int stopRange = stoi(s);

    string n = ""; // number of processes
    getline(cin, n);

    vector<vector<string>> processesData;
    processesTokenizer(processesData, stoi(n));

    Processes p;
    p.stopRange = stopRange;
    organizeData(p, processesData);

    switch (typeData[0])
    {
    case 1:
        p.scheduleName = "FCFS";
        doFCFS(p);
        if (visualizationType == "trace")
            traceFCFS(p);
        if (visualizationType == "stats")
            stats(p);
        break;
    case 2:
        p.scheduleName = "RR-" + to_string(typeData[1]);
        doRR(p, typeData[1]);
        if (visualizationType == "trace")
            traceRR(p);
        if (visualizationType == "stats")
            stats(p);
        break;
    default:
        break;
    }
}
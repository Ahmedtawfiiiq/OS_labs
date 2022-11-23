#include <bits/stdc++.h>

#include "tokenizer.hpp"
#include "process.hpp"

using namespace std;

int main()
{
    string visualizationType = "";
    getline(cin, visualizationType);

    vector<vector<int>> typeData;
    typeTokenizer(typeData);

    string s = ""; // when to stop the simulation
    getline(cin, s);
    int stopRange = stoi(s);

    string n = ""; // number of processes
    getline(cin, n);

    vector<vector<string>> processesData;
    processesTokenizer(processesData, stoi(n));
    // displayData(processesData);

    // Processes p;
    // p.stopRange = stopRange;
    // organizeData(p, processesData, stopRange);

    for (int i = 0; i < typeData.size(); i++)
    {
        Processes p;
        p.stopRange = stopRange;
        organizeData(p, processesData, stopRange);

        switch (typeData[i][0])
        {
        case 1:
            p.scheduleName = "FCFS"; // easy
            doFCFS(p);
            break;
        case 2:
            p.scheduleName = "RR-" + to_string(typeData[i][1]);
            doRR(p, typeData[i][1]);
            break;
        case 3:
            p.scheduleName = "SPN"; // easy
            doSPN(p);
            break;
        case 4:
            p.scheduleName = "SRT";
            doSRT(p);
            break;
        case 5:
            p.scheduleName = "HRRN"; // easy
            doHRR(p);
            break;
        case 6:
            p.scheduleName = "FB-1"; // finished
            doFBq1(p);
            break;
        case 7:
            p.scheduleName = "FB-2i"; // finished
            doFBq2i(p);
            break;
        default:
            break;
        }

        if (visualizationType == "trace")
            trace(p);
        if (visualizationType == "stats")
            stats(p);
    }
}
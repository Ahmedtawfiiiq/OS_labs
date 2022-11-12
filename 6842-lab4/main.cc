#include <bits/stdc++.h>

#include "tokenizer.hpp"
#include "process.hpp"

using namespace std;

int main()
{
    vector<int> typeData;
    typeTokenizer(typeData);

    string s = ""; // when to stop the simulation
    getline(cin, s);

    string n = ""; // number of processes
    getline(cin, n);

    vector<vector<string>> processesData;
    processesTokenizer(processesData, stoi(n));

    Processes p;
    organizeData(p, processesData);

    switch (typeData[0])
    {
    case 1:
        doFCFS(p);
        doSimulationFCFS(p, stoi(s));
        break;
    case 2:
        doRR(p, typeData[1]);
        break;
    default:
        break;
    }
}
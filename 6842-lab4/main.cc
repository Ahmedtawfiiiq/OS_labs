#include <bits/stdc++.h>

#include "tokenizer.hpp"
#include "process.hpp"

using namespace std;

int main()
{
    string s = "";
    getline(cin, s);

    string n = "";
    getline(cin, n);

    vector<vector<string>> processesData;
    inputTokenizer(processesData, stoi(n));
    displayData(processesData);

    Processes p;
    organizeData(p, processesData);

    doFCFS(p);

    p.doSimulation(stoi(s));
}
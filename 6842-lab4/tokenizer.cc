#include "tokenizer.hpp"

void typeTokenizer(vector<int> &typeData)
{
    string data = "";
    string intermediate = "";
    getline(cin, data);
    stringstream tokenizer(data);
    while (getline(tokenizer, intermediate, '-'))
    {
        typeData.push_back(stoi(intermediate));
    }
}

void processesTokenizer(vector<vector<string>> &data, int n)
{
    string line = "";
    string intermediate = "";
    for (int i = 0; i < n; i++)
    {
        vector<string> rowData;
        getline(cin, line);
        stringstream tokenizer(line);
        while (getline(tokenizer, intermediate, ','))
        {
            rowData.push_back(intermediate);
        }
        data.push_back(rowData);
    }
}

void displayData(vector<vector<string>> data)
{
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {
            cout << data[i][j];
        }
        cout << endl;
    }
}
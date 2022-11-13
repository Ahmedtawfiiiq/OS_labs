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

bool sortColumn(const vector<string> &v1, const vector<string> &v2)
{
    if (stoi(v1[1]) < stoi(v2[1]))
        return stoi(v1[1]) < stoi(v2[1]);
    else if (stoi(v1[1]) == stoi(v2[1]))
        return v1[2] < v2[2];
    else
        return false;
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

    sort(data.begin(), data.end(), sortColumn);
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
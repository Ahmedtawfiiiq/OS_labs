#include "tokenizer.hpp"

void typeTokenizer(vector<vector<int>> &typeData)
{
    string data = "";
    getline(cin, data);
    string intermediate1 = "";
    vector<string> type1;
    stringstream tokenizer1(data);

    while (getline(tokenizer1, intermediate1, ','))
    {
        type1.push_back(intermediate1);
    }

    string intermediate2 = "";
    for (int i = 0; i < type1.size(); i++)
    {
        stringstream tokenizer2(type1[i]);
        vector<int> row;
        while (getline(tokenizer2, intermediate2, '-'))
        {
            row.push_back(stoi(intermediate2));
        }
        typeData.push_back(row);
    }
}

bool sortColumn(const vector<string> &v1, const vector<string> &v2)
{
    if (stoi(v1[1]) < stoi(v2[1]))
        return stoi(v1[1]) < stoi(v2[1]);
    else if (stoi(v1[1]) == stoi(v2[1]))
        return v1[0] < v2[0];
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

void displayData(vector<vector<int>> data)
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
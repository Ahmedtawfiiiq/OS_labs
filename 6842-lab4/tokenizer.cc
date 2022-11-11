#include "tokenizer.hpp"

void inputTokenizer(vector<vector<string>> &data, int n)
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
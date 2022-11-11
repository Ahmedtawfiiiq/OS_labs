#include <bits/stdc++.h>

using namespace std;

void inputTokenizer(vector<vector<string>> &data)
{
    string numberOfProcess = "";
    getline(cin, numberOfProcess);

    string line = "";
    string intermediate = "";
    for (int i = 0; i < stoi(numberOfProcess); i++)
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

// void toInteger(vector<vector<string>> data, vector<tuple<string, int, int>> &intData)
// {
//     tuple<string, int, int> t;
//     for (int i = 0; i < data.size(); i++)
//     {
//         get<0>(intData[i]) = data[i][0];
//         get<1>(intData[i]) = stoi(data[i][1]);
//         get<2>(intData[i]) = stoi(data[i][2]);
//     }
// }

int main()
{

    vector<vector<string>> processData;
    inputTokenizer(processData);

    int x = stoi(processData[0][1]) - stoi(processData[0][2]);
    cout << to_string(x) << endl;
    string y = to_string(x);
    cout << stoi(y) << endl;

    // vector<tuple<string, int, int>> intData;
    // toInteger(processData, intData);

    // cout << get<0>(intData[0]) << endl;
    // displayData(processData);

    return 0;
}
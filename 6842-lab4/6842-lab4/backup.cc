#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

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

int main()
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

    vector<vector<int>> type2;
    string intermediate2 = "";
    for (int i = 0; i < type1.size(); i++)
    {
        stringstream tokenizer2(type1[i]);
        vector<int> row;
        while (getline(tokenizer2, intermediate2, '-'))
        {
            row.push_back(stoi(intermediate2));
        }
        type2.push_back(row);
    }

    // cout << type2[0][1];
    // cout << endl;
    // // displayData(type2);

    cout << type2.size();
    cout << endl;

    return 0;
}
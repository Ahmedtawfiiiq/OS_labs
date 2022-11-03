#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream fin("t1.txt");
    while (!fin.eof())
    {
        string line;
        getline(fin, line);
        cout << line << endl;
    }
    return 0;
}

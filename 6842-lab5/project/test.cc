#include <fstream>

using namespace std;

int main()
{
    ofstream f("data.txt");

    string text = "testiiing";

    f << text << endl;

    return 0;
}

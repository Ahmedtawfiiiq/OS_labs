#include <iostream>
#include <queue>

using namespace std;

int main()
{
    int x = 6;
    queue<int> q[x];
    q[0].push(4);
    cout << q[0].front() << endl;

    q[5].push(10);
    cout << q[5].front() << endl;
    return 0;
}
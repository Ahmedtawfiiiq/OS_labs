#include <iostream>
#include <stdio.h>

using namespace std;

// 1  [0;31m	Red
// 2  [1;31m	Bold Red
// 3  [0;32m	Green
// 4  [1;32m	Bold Green
// 5  [0;33m	Yellow
// 6  [01;33m	Bold Yellow
// 7  [0;34m	Blue
// 8  [1;34m	Bold Blue
// 9  [0;35m	Magenta
// 10 [1;35m	Bold Magenta
// 11 [0;36m	Cyan
// 12 [1;36m	Bold Cyan
// 13 [0m	Reset

int main()
{
    cout << "\033[0;31mHello world" << endl;

    cout << "\033[1;31mHello world" << endl;

    cout << "\033[0;32mHello world" << endl;

    cout << "\033[1;32mHello world" << endl;

    cout << "\033[0;33mHello world" << endl;

    cout << "\033[01;33mHello world" << endl;

    cout << "\033[0;34mHello world" << endl;

    cout << "\033[1;34mHello world" << endl;

    cout << "\033[0;35mHello world" << endl;

    cout << "\033[1;35mHello world" << endl;

    cout << "\033[0;36mHello world" << endl;

    cout << "\033[1;36mHello world" << endl;

    return 0;
}
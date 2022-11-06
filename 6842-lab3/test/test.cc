#include <iostream>
#include <stdio.h>

using namespace std;

// 1  \033[0;31m	Red
// 2  \033[1;31m	Bold Red
// 3  \033[0;32m	Green
// 4  \033[1;32m	Bold Green
// 5  \033[0;33m	Yellow
// 6  \033[01;33m	Bold Yellow
// 7  \033[0;34m	Blue
// 8  \033[1;34m	Bold Blue
// 9  \033[0;35m	Magenta
// 10 \033[1;35m	Bold Magenta
// 11 \033[0;36m	Cyan
// 12 \033[1;36m	Bold Cyan
// 13 \033[0m	Reset

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
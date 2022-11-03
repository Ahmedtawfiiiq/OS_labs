#include <iostream>
#include <dirent.h>
#include <algorithm>

using namespace std;
int main()
{
    string files[50];
    static int i = 0;
    struct dirent *d;
    DIR *dr;
    dr = opendir("/home/ahmed/Documents");
    if (dr != NULL)
    {
        cout << "List of Files & Folders:-\n";
        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            files[i] = d->d_name;
            i++;
        }
        closedir(dr);
    }
    else
        cout << "\nError Occurred!";

    for (int j = 0; j < i; j++)
    {
        cout << files[j] << endl;
    }
    return 0;
}
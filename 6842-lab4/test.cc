#include <bits/stdc++.h>

using namespace std;

int main()
{
    cout << fixed << setprecision(5);

    double x = 1.345;

    cout << "hello" << endl;
    cout << x << endl;
}

// void stats(Processes p)
// {
//     cout << "FCFS" << endl;

//     cout << "Process    ";
//     for (int i = 0; i < p.processes.size(); i++)
//     {
//         cout << "|  "
//              << p.processes[i].pid
//              << "  ";
//     }
//     cout << "|" << endl;

//     cout << "Arrival    ";
//     for (int i = 0; i < p.processes.size(); i++)
//     {
//         cout << "|";
//         printf("%3d", p.processes[i].at);
//         cout << "  ";
//     }
//     cout << "|" << endl;

//     cout << "Service    ";
//     for (int i = 0; i < p.processes.size(); i++)
//     {
//         cout << "|";
//         printf("%3d", p.processes[i].bt);
//         cout << "  ";
//     }
//     cout << "| Mean|" << endl;

//     cout << "Finish     ";
//     for (int i = 0; i < p.processes.size(); i++)
//     {
//         cout << "|";
//         printf("%3d", p.processes[i].ct);
//         cout << "  ";
//     }
//     cout << "|-----|" << endl;

//     cout << "Turnaround ";
//     for (int i = 0; i < p.processes.size(); i++)
//     {
//         cout << "|";
//         printf("%3d", p.processes[i].tat);
//         cout << "  ";
//     }
//     cout << "|";
//     printf("%5.2lf", p.tatMean);
//     cout << "|" << endl;
// }

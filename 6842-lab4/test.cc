#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstring>

using namespace std;

class process
{
public:
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

int main()
{

    int n = 5;
    int tq = 1;
    process p[5];
    int burst_remaining[5];
    int idx;

    p[0].pid = 1;
    p[0].arrival_time = 0;
    p[0].burst_time = 3;

    p[1].pid = 2;
    p[1].arrival_time = 2;
    p[1].burst_time = 6;

    p[2].pid = 3;
    p[2].arrival_time = 4;
    p[2].burst_time = 4;

    p[3].pid = 4;
    p[3].arrival_time = 6;
    p[3].burst_time = 5;

    p[4].pid = 5;
    p[4].arrival_time = 8;
    p[4].burst_time = 2;

    for (int i = 0; i < n; i++)
    {
        burst_remaining[i] = p[i].burst_time;
    }

    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while (completed != n)
    {
        idx = q.front();
        q.pop();

        if (burst_remaining[idx] == p[idx].burst_time)
        {
            p[idx].start_time = max(current_time, p[idx].arrival_time);
            current_time = p[idx].start_time;
        }

        if (burst_remaining[idx] - tq > 0)
        {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }
        else
        {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
        }

        for (int i = 1; i < n; i++)
        {
            if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && mark[i] == 0)
            {
                q.push(i);
                mark[i] = 1;
            }
        }
        if (burst_remaining[idx] > 0)
        {
            q.push(idx);
        }

        if (q.empty())
        {
            for (int i = 1; i < n; i++)
            {
                if (burst_remaining[i] > 0)
                {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }

    cout << endl;
    cout << "#P\t"
         << "AT\t"
         << "BT\t"
         << "ST\t"
         << "CT\t"
         << "TAT\t"
         << "WT\t"
         << "\n"
         << endl;

    for (int i = 0; i < n; i++)
    {
        cout << p[i].pid
             << "\t" << p[i].arrival_time
             << "\t" << p[i].burst_time
             << "\t" << p[i].start_time
             << "\t" << p[i].completion_time
             << "\t" << p[i].turnaround_time
             << "\t" << p[i].waiting_time
             << "\n"
             << endl;
    }
}

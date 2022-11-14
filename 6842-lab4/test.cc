// non-preemptive scheduling

#include <bits/stdc++.h>
#include <limits>

using namespace std;

// Defining process details
struct process
{
    char name;
    int at, bt, ct, wt, tt;
    int completed;
} p[10];

int n;

int main()
{
    int i, j, sum_bt = 0;
    char c;
    float t;
    n = 5;

    // predefined arrival times
    int arrivalTime[] = {0, 2, 4, 6, 8};

    // predefined burst times
    int burst[] = {3, 6, 4, 5, 2};

    // Initializing the structure variables
    for (i = 0, c = 'A'; i < n; i++, c++)
    {
        p[i].name = c;
        p[i].at = arrivalTime[i];
        p[i].bt = burst[i];

        // Variable for Completion status
        // Pending = 0
        // Completed = 1
        p[i].completed = 0;

        // Variable for sum of all Burst Times
        sum_bt += p[i].bt;
    }

    for (t = p[0].at; t < sum_bt;)
    {

        // Set lower limit to response ratio
        float ratio = numeric_limits<int>::min();

        // Response Ratio Variable
        float currentHighestRatio;

        // Variable to store next process selected
        int currentProcess;
        for (i = 0; i < n; i++)
        {
            // Checking if process has arrived and is
            // Incomplete
            if (p[i].at <= t && p[i].completed != 1)
            {

                // Calculating Response Ratio
                currentHighestRatio = (p[i].bt + (t - p[i].at)) / p[i].bt;

                // Checking for Highest Response Ratio
                if (ratio < currentHighestRatio)
                {

                    // Storing Response Ratio
                    ratio = currentHighestRatio;

                    // Storing current process location
                    currentProcess = i;
                }
            }
        }

        // Updating time value
        t += p[currentProcess].bt;

        // Calculation of waiting time
        p[currentProcess].wt = t - p[currentProcess].at - p[currentProcess].bt;

        // Calculation of Turn Around Time
        p[currentProcess].tt = t - p[currentProcess].at;

        // Updating Completion Status
        p[currentProcess].completed = 1;
    }
}

#include <limits>

#include "process.hpp"

using namespace std;

void organizeData(Processes &p, vector<vector<string>> inputData, int)
{
    Process px;
    for (int i = 0; i < inputData.size(); i++)
    {
        px.pid = inputData[i][0];
        px.at = stoi(inputData[i][1]);
        px.bt = stoi(inputData[i][2]);
        p.processes.push_back(px);
    }

    for (int i = 0; i < p.processes.size(); i++)
    {
        p.processes[i].output.second = p.stopRange;
    }

    // simulate the part of arrive time
    for (int i = 0; i < p.processes.size(); i++)
    {
        for (int j = 0; j < p.processes[i].at; j++)
        {
            p.processes[i].output.first += "| ";
            p.processes[i].output.second--;
        }
    }
}

void stats(Processes p)
{
    cout << p.scheduleName << endl;

    cout << "Process    ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|  "
             << p.processes[i].pid
             << "  ";
    }
    cout << "|" << endl;

    cout << "Arrival    ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|";
        printf("%3d", p.processes[i].at);
        cout << "  ";
    }
    cout << "|" << endl;

    cout << "Service    ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|";
        printf("%3d", p.processes[i].bt);
        cout << "  ";
    }
    cout << "| Mean|" << endl;

    cout << "Finish     ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|";
        printf("%3d", p.processes[i].ct);
        cout << "  ";
    }
    cout << "|-----|" << endl;

    cout << "Turnaround ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|";
        printf("%3d", p.processes[i].tat);
        cout << "  ";
    }
    cout << "|";
    printf("%5.2lf", p.tatMean);
    cout << "|" << endl;

    cout << "NormTurn   ";
    for (int i = 0; i < p.processes.size(); i++)
    {
        cout << "|";
        printf("%5.2lf", (double)p.processes[i].tat / (double)p.processes[i].bt);
    }
    cout << "|";
    double averageNormalizedTurnaroundTime = 0.0;
    for (int i = 0; i < p.processes.size(); i++)
    {
        averageNormalizedTurnaroundTime += (double)p.processes[i].tat / (double)p.processes[i].bt;
    }
    printf("%5.2lf", averageNormalizedTurnaroundTime / p.processes.size());
    cout << "|" << endl;
    cout << endl;
}

void trace(Processes p)
{
    if (p.scheduleName.size() == 3)
        cout << p.scheduleName << "   ";
    else if (p.scheduleName.size() == 5)
        cout << p.scheduleName << " ";
    else
        cout << p.scheduleName << "  ";

    for (int i = 0; i <= p.stopRange; i++)
    {
        cout << i % 10 << " ";
    }
    cout << endl;

    cout << "------------------------------------------------" << endl;
    for (int q = 0; q < p.processes.size(); q++)
    {
        cout << p.processes[q].pid << "     ";
        cout << p.processes[q].output.first;
        for (int v = 0; v < p.processes[q].output.second; v++)
        {
            cout << "| ";
        }
        cout << "| " << endl;
    }
    cout << "------------------------------------------------" << endl;
    cout << endl;
}

void doFCFS(Processes &p)
{
    p.processes[0].ct = p.processes[0].at + p.processes[0].bt;
    p.processes[0].tat = p.processes[0].ct - p.processes[0].at;
    p.processes[0].wt = p.processes[0].tat - p.processes[0].bt;
    p.tatMean += p.processes[0].tat;

    for (int i = 1; i < p.processes.size(); i++)
    {
        if (p.processes[i].at < p.processes[i - 1].ct)
        {
            p.processes[i].ct = p.processes[i - 1].ct + p.processes[i].bt;
        }
        else
        {
            p.processes[i].ct = p.processes[i].at + p.processes[i].bt;
        }
        p.processes[i].tat = p.processes[i].ct - p.processes[i].at;
        p.processes[i].wt = p.processes[i].tat - p.processes[i].bt;
        p.tatMean += p.processes[i].tat;
    }

    for (int i = 0; i < p.processes.size(); i++)
    {
        for (int j = 0; j < p.processes[i].wt; j++)
        {
            p.processes[i].output.first += "|.";
            p.processes[i].output.second--;
        }

        for (int j = 0; j < p.processes[i].bt; j++)
        {
            p.processes[i].output.first += "|*";
            p.processes[i].output.second--;
        }
    }

    p.tatMean /= p.processes.size();
}

void doRR(Processes &p, int quantum)
{
    vector<int> burstRemaining(p.processes.size());

    for (int i = 0; i < p.processes.size(); i++)
    {
        burstRemaining[i] = p.processes[i].bt;
    }

    int currentTime = p.processes[0].at;

    queue<int> q;
    int currentProcess = 0;
    q.push(currentProcess);

    vector<int> trace(p.processes.size());
    fill(trace.begin(), trace.end(), 0);
    trace[currentProcess] = 1;

    vector<int> last(p.processes.size());
    fill(last.begin(), last.end(), 0);

    int completed = 0;

    while (completed != p.processes.size())
    {
        currentProcess = q.front();
        q.pop();

        if (burstRemaining[currentProcess] == p.processes[currentProcess].bt)
        {
            for (int w = 0; w < abs(currentTime - p.processes[currentProcess].at); w++)
            {
                p.processes[currentProcess].output.first += "|.";
                p.processes[currentProcess].output.second--;
            }
            currentTime = max(currentTime, p.processes[currentProcess].at);
            last[currentProcess] = currentTime;
        }

        for (int d = 0; d < abs(last[currentProcess] - currentTime); d++)
        {
            p.processes[currentProcess].output.first += "|.";
            p.processes[currentProcess].output.second--;
        }

        if (burstRemaining[currentProcess] - quantum > 0)
        {
            burstRemaining[currentProcess] -= quantum;
            currentTime += quantum;
            last[currentProcess] = currentTime;

            for (int k = 0; k < quantum; k++)
            {
                p.processes[currentProcess].output.first += "|*";
                p.processes[currentProcess].output.second--;
            }
        }
        else
        {
            currentTime += burstRemaining[currentProcess];
            last[currentProcess] = currentTime;

            for (int k = 0; k < burstRemaining[currentProcess]; k++)
            {
                p.processes[currentProcess].output.first += "|*";
                p.processes[currentProcess].output.second--;
            }

            burstRemaining[currentProcess] = 0;
            completed++;

            p.processes[currentProcess].ct = currentTime;
            p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
            p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
            p.tatMean += p.processes[currentProcess].tat;
        }

        for (int i = 1; i < p.processes.size(); i++)
        {
            if (burstRemaining[i] > 0 && p.processes[i].at <= currentTime && trace[i] == 0)
            {
                q.push(i);
                trace[i] = 1;
            }
        }
        if (burstRemaining[currentProcess] > 0)
        {
            q.push(currentProcess);
        }

        if (q.empty())
        {
            for (int i = 1; i < p.processes.size(); i++)
            {
                if (burstRemaining[i] > 0)
                {
                    q.push(i);
                    trace[i] = 1;
                    break;
                }
            }
        }
    }

    p.tatMean /= p.processes.size();
}

void doSPN(Processes &p)
{
    int currentTime = p.processes[0].at;
    int completed = 0;
    int prev = 0;

    int size = p.processes.size();

    vector<int> isCompleted(size);
    fill(isCompleted.begin(), isCompleted.end(), 0);

    while (completed != size)
    {
        int currentProcess = -1;
        int minimum = numeric_limits<int>::max();
        for (int i = 0; i < size; i++)
        {
            if (p.processes[i].at <= currentTime && isCompleted[i] == 0)
            {
                if (p.processes[i].bt < minimum)
                {
                    minimum = p.processes[i].bt;
                    currentProcess = i;
                }
                if (p.processes[i].bt == minimum)
                {
                    if (p.processes[i].at < p.processes[currentProcess].at)
                    {
                        minimum = p.processes[i].bt;
                        currentProcess = i;
                    }
                }
            }
        }
        if (currentProcess != -1)
        {
            p.processes[currentProcess].ct = currentTime + p.processes[currentProcess].bt;
            p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
            p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
            p.tatMean += p.processes[currentProcess].tat;

            isCompleted[currentProcess] = 1;
            completed++;
            currentTime = p.processes[currentProcess].ct;
            prev = currentTime;
        }
        else
        {
            currentTime++;
        }
    }

    for (int i = 0; i < p.processes.size(); i++)
    {
        for (int j = 0; j < p.processes[i].wt; j++)
        {
            p.processes[i].output.first += "|.";
            p.processes[i].output.second--;
        }

        for (int j = 0; j < p.processes[i].bt; j++)
        {
            p.processes[i].output.first += "|*";
            p.processes[i].output.second--;
        }
    }

    p.tatMean /= size;
}

void doSRT(Processes &p)
{
    int currentTime = p.processes[0].at;
    int completed = 0;

    int size = p.processes.size();

    vector<int> isCompleted(size);
    fill(isCompleted.begin(), isCompleted.end(), 0);

    vector<int> burstRemaining(p.processes.size());

    for (int i = 0; i < p.processes.size(); i++)
    {
        burstRemaining[i] = p.processes[i].bt;
    }

    vector<int> last(p.processes.size());
    fill(last.begin(), last.end(), 0);

    while (completed != size)
    {
        int currentProcess = -1;
        int minimum = numeric_limits<int>::max();

        vector<int> wasMinimum(size);
        fill(wasMinimum.begin(), wasMinimum.end(), 0);

        // loop to choose the process with minimum remaining burst time
        for (int i = 0; i < size; i++)
        {
            if (p.processes[i].at <= currentTime && isCompleted[i] == 0)
            {
                if (burstRemaining[i] < minimum)
                {
                    minimum = burstRemaining[i];
                    currentProcess = i;
                    wasMinimum[i] = 1;
                }
                else if (isCompleted[i] == 0)
                {
                    p.processes[i].output.first += "|.";
                    p.processes[i].output.second--;
                }
            }
        }

        for (int y = 0; y < size; y++)
        {
            if (y != currentProcess && wasMinimum[y] == 1)
            {
                p.processes[y].output.first += "|.";
                p.processes[y].output.second--;
            }
        }

        fill(wasMinimum.begin(), wasMinimum.end(), 0);

        currentTime++;
        last[currentProcess] = currentTime;

        if (currentProcess != -1)
        {
            burstRemaining[currentProcess] -= 1;
            p.processes[currentProcess].output.first += "|*";
            p.processes[currentProcess].output.second--;

            if (burstRemaining[currentProcess] == 0)
            {
                p.processes[currentProcess].ct = currentTime;
                p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
                p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
                p.tatMean += p.processes[currentProcess].tat;

                isCompleted[currentProcess] = 1;
                completed++;
            }
        }
    }

    p.tatMean /= size;
}

void doHRR(Processes &p)
{
    double currentTime = p.processes[0].at;
    int totalBurstTime = 0;
    int size = p.processes.size();

    vector<int> completed(size);
    fill(completed.begin(), completed.end(), 0);

    for (int i = 0; i < p.processes.size(); i++)
    {
        totalBurstTime += p.processes[i].at;
    }

    for (currentTime = p.processes[0].at; currentTime < totalBurstTime;)
    {
        float ratio = numeric_limits<int>::min();
        float currentHighestRatio;

        int currentProcess;
        for (int i = 0; i < size; i++)
        {
            if (p.processes[i].at <= currentTime && completed[i] != 1)
            {
                currentHighestRatio = (p.processes[i].bt + (currentTime - p.processes[i].at)) / p.processes[i].bt;

                if (ratio < currentHighestRatio)
                {
                    ratio = currentHighestRatio;
                    currentProcess = i;
                }
            }
        }

        currentTime += p.processes[currentProcess].bt;
        p.processes[currentProcess].ct = currentTime;
        p.processes[currentProcess].wt = currentTime - p.processes[currentProcess].at - p.processes[currentProcess].bt;
        p.processes[currentProcess].tat = currentTime - p.processes[currentProcess].at;
        p.tatMean += p.processes[currentProcess].tat;
        completed[currentProcess] = 1;
    }

    for (int i = 0; i < p.processes.size(); i++)
    {
        for (int j = 0; j < p.processes[i].wt; j++)
        {
            p.processes[i].output.first += "|.";
            p.processes[i].output.second--;
        }

        for (int j = 0; j < p.processes[i].bt; j++)
        {
            p.processes[i].output.first += "|*";
            p.processes[i].output.second--;
        }
    }

    p.tatMean /= size;
}

vector<int> checkArrivedProcesses(Processes &p, int currentTime, vector<int> &isCompleted, vector<int> &isArrived)
{
    int size = p.processes.size();
    vector<int> q1;

    for (int i = 0; i < size; i++)
    {
        if (p.processes[i].at <= currentTime && isCompleted[i] == 0 && isArrived[i] == 0)
        {
            q1.push_back(i);
            isArrived[i] = 1;
        }
    }
    return q1;
}

void doFBq1(Processes &p)
{
    int currentTime = p.processes[0].at;
    int currentProcess = -1;
    int quantum = 1;
    int size = p.processes.size();

    vector<int> remainingBurstTime(size);
    fill(remainingBurstTime.begin(), remainingBurstTime.end(), 0);

    for (int i = 0; i < size; i++)
    {
        remainingBurstTime[i] = p.processes[i].bt;
    }

    vector<int> arrivedProcesses;

    vector<int> isCompleted(size);
    fill(isCompleted.begin(), isCompleted.end(), 0);

    vector<int> isArrived(size);
    fill(isArrived.begin(), isArrived.end(), 0);

    queue<int> q[5];

    vector<int> last(size);
    for (int v = 0; v < size; v++)
    {
        last[v] = p.processes[v].at;
    }

    // we need to call it every time we change the current time
    arrivedProcesses = checkArrivedProcesses(p, currentTime, isCompleted, isArrived);
    for (int i = 0; i < arrivedProcesses.size(); i++)
    {
        q[0].push(arrivedProcesses[i]);
    }

    int completedProcesses = 0;
    int numberOfQueues = 5;

    string lastProcess = "";

    while (completedProcesses < size)
    {
        for (int l = 0; l < numberOfQueues; l++)
        {
            if (!q[l].empty())
            {
                currentProcess = q[l].front();

                if (lastProcess == p.processes[currentProcess].pid)
                    continue;

                for (int h = 0; h < abs(last[currentProcess] - currentTime); h++)
                {
                    p.processes[currentProcess].output.first += "|.";
                    p.processes[currentProcess].output.second--;
                }

                currentTime += quantum;
                last[currentProcess] = currentTime;

                remainingBurstTime[currentProcess]--;
                p.processes[currentProcess].output.first += "|*";
                p.processes[currentProcess].output.second--;

                arrivedProcesses = checkArrivedProcesses(p, currentTime, isCompleted, isArrived);
                for (int i = 0; i < arrivedProcesses.size(); i++)
                {
                    q[0].push(arrivedProcesses[i]);
                }

                // check if the current process is the only process
                // in all of the queues
                int numberOfUnemptyQueues = 0;
                for (int g = 0; g < numberOfQueues; g++)
                {
                    if (g == l)
                        continue;
                    else
                    {
                        if (!q[g].empty())
                            numberOfUnemptyQueues++;
                    }
                }

                bool flag = false;
                if (numberOfUnemptyQueues > 0 || q[l].size() > 1)
                {
                    lastProcess = p.processes[currentProcess].pid;
                    q[l].pop();
                }

                else
                    flag = true;

                if (remainingBurstTime[currentProcess] > 0)
                {
                    if (flag == false)
                    {
                        if (l == numberOfQueues - 1)
                            q[l].push(currentProcess);
                        else
                            q[l + 1].push(currentProcess);
                    }
                }

                if (remainingBurstTime[currentProcess] == 0)
                {
                    p.processes[currentProcess].ct = currentTime;
                    p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
                    p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
                    p.tatMean += p.processes[currentProcess].tat;
                    completedProcesses++;
                    isCompleted[currentProcess] = 1;
                }

                break;
            }
        }
    }
    p.tatMean /= size;
}

void doFBq2i(Processes &p)
{
    int currentTime = p.processes[0].at;
    int currentProcess = -1;

    int quantum[5];

    quantum[0] = 1;
    quantum[1] = 2;
    quantum[2] = 4;
    quantum[3] = 8;
    quantum[4] = 16;

    int size = p.processes.size();

    vector<int> remainingBurstTime(size);
    fill(remainingBurstTime.begin(), remainingBurstTime.end(), 0);

    for (int i = 0; i < size; i++)
    {
        remainingBurstTime[i] = p.processes[i].bt;
    }

    vector<int> arrivedProcesses;

    vector<int> isCompleted(size);
    fill(isCompleted.begin(), isCompleted.end(), 0);

    vector<int> isArrived(size);
    fill(isArrived.begin(), isArrived.end(), 0);

    queue<int> q[5];

    // we need to call it every time we change the current time
    arrivedProcesses = checkArrivedProcesses(p, currentTime, isCompleted, isArrived);
    for (int i = 0; i < arrivedProcesses.size(); i++)
    {
        q[0].push(arrivedProcesses[i]);
    }

    int completedProcesses = 0;
    int numberOfQueues = 5;

    string lastProcess = "";

    vector<int> last(size);
    for (int v = 0; v < size; v++)
    {
        last[v] = p.processes[v].at;
    }

    while (completedProcesses < size)
    {
        for (int l = 0; l < numberOfQueues; l++)
        {
            if (!q[l].empty())
            {
                currentProcess = q[l].front();

                if (lastProcess == p.processes[currentProcess].pid)
                    continue;

                for (int h = 0; h < abs(last[currentProcess] - currentTime); h++)
                {
                    p.processes[currentProcess].output.first += "|.";
                    p.processes[currentProcess].output.second--;
                }

                if (remainingBurstTime[currentProcess] > quantum[l])
                {
                    currentTime += quantum[l];
                    last[currentProcess] = currentTime;
                    remainingBurstTime[currentProcess] -= quantum[l];
                    for (int o = 0; o < quantum[l]; o++)
                    {
                        p.processes[currentProcess].output.first += "|*";
                        p.processes[currentProcess].output.second--;
                    }
                }
                else
                {
                    currentTime += remainingBurstTime[currentProcess];
                    last[currentProcess] = currentTime;
                    for (int o = 0; o < remainingBurstTime[currentProcess]; o++)
                    {
                        p.processes[currentProcess].output.first += "|*";
                        p.processes[currentProcess].output.second--;
                    }
                    remainingBurstTime[currentProcess] = 0;
                }

                arrivedProcesses = checkArrivedProcesses(p, currentTime, isCompleted, isArrived);
                for (int i = 0; i < arrivedProcesses.size(); i++)
                {
                    q[0].push(arrivedProcesses[i]);
                }

                // check if the current process is the only process
                // in all of the queues
                int numberOfUnemptyQueues = 0;
                for (int g = 0; g < numberOfQueues; g++)
                {
                    if (g == l)
                        continue;
                    else
                    {
                        if (!q[g].empty())
                            numberOfUnemptyQueues++;
                    }
                }

                bool flag = false;
                if (numberOfUnemptyQueues > 0 || q[l].size() > 1)
                {
                    lastProcess = p.processes[currentProcess].pid;
                    q[l].pop();
                }

                else
                    flag = true;

                if (remainingBurstTime[currentProcess] > 0)
                {
                    if (flag == false)
                    {
                        if (l == numberOfQueues - 1)
                            q[l].push(currentProcess);
                        else
                            q[l + 1].push(currentProcess);
                    }
                }

                if (remainingBurstTime[currentProcess] == 0)
                {
                    p.processes[currentProcess].ct = currentTime;
                    p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
                    p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
                    p.tatMean += p.processes[currentProcess].tat;
                    completedProcesses++;
                    isCompleted[currentProcess] = 1;
                }

                break;
            }
        }
    }
    p.tatMean /= size;
}

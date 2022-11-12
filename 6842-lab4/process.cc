#include "process.hpp"

void organizeData(Processes &p, vector<vector<string>> inputData)
{
    Process px;
    for (int i = 0; i < inputData.size(); i++)
    {
        px.pid = inputData[i][0];
        px.at = stoi(inputData[i][1]);
        px.bt = stoi(inputData[i][2]);
        p.processes.push_back(px);
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

    p.tatMean /= p.processes.size();
}

void traceFCFS(Processes p)
{
    cout << p.scheduleName << "  ";

    for (int i = 0; i <= p.stopRange; i++)
    {
        cout << i % 10 << " ";
    }

    cout << endl;
    cout << "------------------------------------------------";
    cout << endl;

    for (Process process : p.processes)
    {
        int stopSimulation = p.stopRange;

        cout << process.pid << "     ";

        for (int k = 0; k < process.at; k++)
        {
            cout << "| ";
            stopSimulation--;
        }

        for (int i = 0; i < process.wt; i++)
        {
            cout << "|.";
            stopSimulation--;
        }

        for (int j = 0; j < process.bt; j++)
        {
            cout << "|*";
            stopSimulation--;
        }
        for (int l = 0; l < stopSimulation; l++)
        {
            cout << "| ";
        }

        cout << '|';
        cout << endl;
    }

    cout << "------------------------------------------------" << endl;
}

void doRR(Processes &p, int quantum)
{
    vector<int> burstRemaining(p.processes.size());

    for (int i = 0; i < p.processes.size(); i++)
    {
        burstRemaining[i] = p.processes[i].bt;
    }

    int currentTime = 0;

    queue<int> q;
    int currentProcess = 0;
    q.push(currentProcess);

    vector<int> trace(p.processes.size());
    fill(trace.begin(), trace.end(), 0);
    trace[currentProcess] = 1;

    vector<int> last(p.processes.size());
    fill(last.begin(), last.end(), 0);

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

void traceRR(Processes p)
{
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
        cout << "|" << endl;
    }
    cout << "------------------------------------------------" << endl;
}
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

void doFCFS(Processes &p)
{
    p.processes[0].ct = p.processes[0].at + p.processes[0].bt;
    p.processes[0].tat = p.processes[0].ct - p.processes[0].at;
    p.processes[0].wt = p.processes[0].tat - p.processes[0].bt;
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
    }
}

void doSimulationFCFS(Processes p, int stopRange)
{
    cout << "------------------------------------------------" << endl;

    for (Process process : p.processes)
    {
        int stopSimulation = stopRange;

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
    int burst_remaining[100];

    for (int i = 0; i < p.processes.size(); i++)
    {
        burst_remaining[i] = p.processes[i].bt;
    }

    queue<int> q;
    int currentTime = 0;
    int currentProcess = 0;
    q.push(currentProcess);
    int completed = 0;
    int mark[100];
    memset(mark, 0, sizeof(mark));
    mark[currentProcess] = 1;

    vector<int> last = {0, 0, 0, 0, 0};
    vector<pair<string, int>> output = {make_pair("", 20), make_pair("", 20), make_pair("", 20), make_pair("", 20), make_pair("", 20)};

    // simulate the part of arrive time
    for (int i = 0; i < p.processes.size(); i++)
    {
        for (int j = 0; j < p.processes[i].at; j++)
        {
            output[i].first += "| ";
            output[i].second--;
        }
    }

    while (completed != p.processes.size())
    {
        currentProcess = q.front();
        q.pop();

        if (burst_remaining[currentProcess] == p.processes[currentProcess].bt)
        {
            for (int w = 0; w < abs(currentTime - p.processes[currentProcess].at); w++)
            {
                output[currentProcess].first += "|.";
                output[currentProcess].second--;
            }
            currentTime = max(currentTime, p.processes[currentProcess].at);
            last[currentProcess] = currentTime;
        }

        for (int d = 0; d < abs(last[currentProcess] - currentTime); d++)
        {
            output[currentProcess].first += "|.";
            output[currentProcess].second--;
        }

        if (burst_remaining[currentProcess] - quantum > 0)
        {
            burst_remaining[currentProcess] -= quantum;
            currentTime += quantum;
            last[currentProcess] = currentTime;

            for (int k = 0; k < quantum; k++)
            {
                output[currentProcess].first += "|*";
                output[currentProcess].second--;
            }
        }
        else
        {
            currentTime += burst_remaining[currentProcess];
            last[currentProcess] = currentTime;

            for (int k = 0; k < burst_remaining[currentProcess]; k++)
            {
                output[currentProcess].first += "|*";
                output[currentProcess].second--;
            }

            burst_remaining[currentProcess] = 0;
            completed++;

            p.processes[currentProcess].ct = currentTime;
            p.processes[currentProcess].tat = p.processes[currentProcess].ct - p.processes[currentProcess].at;
            p.processes[currentProcess].wt = p.processes[currentProcess].tat - p.processes[currentProcess].bt;
        }

        for (int i = 1; i < p.processes.size(); i++)
        {
            if (burst_remaining[i] > 0 && p.processes[i].at <= currentTime && mark[i] == 0)
            {
                q.push(i);
                mark[i] = 1;
            }
        }
        if (burst_remaining[currentProcess] > 0)
        {
            q.push(currentProcess);
        }

        if (q.empty())
        {
            for (int i = 1; i < p.processes.size(); i++)
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

    cout << "------------------------------------------------" << endl;
    for (int q = 0; q < p.processes.size(); q++)
    {
        cout << p.processes[q].pid << "     ";
        cout << output[q].first;
        for (int v = 0; v < output[q].second; v++)
        {
            cout << "| ";
        }
        cout << "|" << endl;
    }
    cout << "------------------------------------------------" << endl;
}
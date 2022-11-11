#include "process.hpp"

void Processes::doSimulation(int stopRange)
{
    cout << "------------------------------------------------" << endl;

    for (Process &process : Processes::processes)
    {
        int stopSimulation = stopRange;

        cout << process["id"] << "     ";

        for (int k = 0; k < stoi(process["at"]); k++)
        {
            cout << "| ";
            stopSimulation--;
        }

        for (int i = 0; i < stoi(process["wt"]); i++)
        {
            cout << "|.";
            stopSimulation--;
        }

        for (int j = 0; j < stoi(process["bt"]); j++)
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

void organizeData(Processes &p, vector<vector<string>> inputData)
{
    Process px;
    for (int i = 0; i < inputData.size(); i++)
    {
        px["id"] = inputData[i][0];
        px["at"] = inputData[i][1];
        px["bt"] = inputData[i][2];
        p.processes.push_back(px);
    }
}

void doFCFS(Processes &p)
{
    p.processes[0]["ct"] = to_string(stoi(p.processes[0]["at"]) + stoi(p.processes[0]["bt"]));
    p.processes[0].update();
    for (int i = 1; i < p.processes.size(); i++)
    {
        if (stoi(p.processes[i]["at"]) < stoi(p.processes[i - 1]["ct"]))
        {
            p.processes[i]["ct"] = to_string(stoi(p.processes[i - 1]["ct"]) + stoi(p.processes[i]["bt"]));
        }
        else
        {

            p.processes[i]["ct"] = to_string(stoi(p.processes[i]["at"]) + stoi(p.processes[i]["bt"]));
        }
        p.processes[i].update();
    }
}

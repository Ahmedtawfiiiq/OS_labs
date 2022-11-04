#include "utilities.h"
#include <iostream>

using namespace std;

int main()
{
    while (true)
    {
        cout << "myshell> ";

        Command currentCommand;

        tokenize(currentCommand);

        currentCommand.print();

        bool lsFlag = false;
        for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
        {
            forkProcces(currentCommand.simpleCommands[i]);
        }
    }
    return 0;
}

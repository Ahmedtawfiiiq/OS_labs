#include "utilities.h"
#include <iostream>

using namespace std;

void handle_sigint(int sig) {}

int main()
{
    while (true)
    {
        cout << "myshell> ";

        Command currentCommand;

        tokenize(currentCommand);

        currentCommand.print();
        classifyCommand(currentCommand);

        if (currentCommand.commandType == "single")
        {
            forkProcces(currentCommand);
        }
        if (currentCommand.commandType == "pipe")
        {
            forkProcessPipe(currentCommand);
        }
    }
    return 0;
}

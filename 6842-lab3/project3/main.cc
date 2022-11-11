#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "utilities.hpp"
#include "system.hpp"

using namespace std;

void prompt()
{
    cout << "\033[1;31mmyshell> ";
    cout << "\033[1;36m";
}

void handle_sigint(int sig)
{
    int currentDescriptor = dup(1);
    int log = open("log.txt", O_RDWR | O_CREAT | O_APPEND);
    dup2(log, 1);
    cout << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << "ctrl-c signal is ignored" << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << endl;
    dup2(currentDescriptor, 1);
}

void proc_exit(int sig)
{
    int currentDescriptor = dup(1);
    int log = open("log.txt", O_RDWR | O_CREAT | O_APPEND);
    dup2(log, 1);
    cout << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << "  A Child is terminated" << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << endl;
    dup2(currentDescriptor, 1);
}

int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, proc_exit);

    int defaultout = dup(1);

    while (true)
    {
        dup2(defaultout, 1);
        close(defaultout);

        prompt();

        Command currentCommand;

        currentCommand.defaultin = dup(0);
        currentCommand.defaultout = dup(1);
        currentCommand.defaulterr = dup(2);
        currentCommand.defaultlog = "log.txt";

        tokenize(currentCommand);
        classifyCommand(currentCommand);
        modifyCommand(currentCommand);

        if (currentCommand.commandType == "single")
        {
            if (currentCommand.simpleCommands[0].command == "cd")
                changeDirectory(currentCommand);

            else if (currentCommand.simpleCommands[0].command == "echo")
                wildcard(currentCommand.simpleCommands[0].arguments[0]);

            else
                forkProcces(currentCommand);
        }
        if (currentCommand.commandType == "pipe")
        {
            forkProcessPipe(currentCommand);
        }

        if (currentCommand.commandType == "doublePipe")
        {
            forkProcessDoublePipe(currentCommand);
        }
    }
    return 0;
}

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#include "system.hpp"

#define REWRITE O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR
#define APPEND O_RDWR | O_CREAT | O_APPEND

void setFileDescriptor(Command &currentCommand, int index, int mode)
{
    if (!mode)
        currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), REWRITE);
    if (mode)
        currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), APPEND);
    dup2(currentCommand.simpleCommands[index].file_desc, 1);
    close(currentCommand.simpleCommands[index].file_desc);
}

void forkProcces(Command &currentCommand)
{
    cout << "\033[0;35m  --------------------------------------------------------------\033[1;32m" << endl;
    cout << endl;

    int mode;

    if (currentCommand.simpleCommands[0].simpleCommandType == "rewrite")
        mode = 0;
    if (currentCommand.simpleCommands[0].simpleCommandType == "append")
        mode = 1;

    setFileDescriptor(currentCommand, 0, mode);

    pid_t pid;

    int currentDescriptor = dup(1);
    int log = open(currentCommand.defaultlog.c_str(), O_RDWR | O_CREAT | O_APPEND);

    pid = fork();

    if (pid < 0)
    {
        cout << "A fork error has occurred" << endl;
        exit(-1); // fork failed
    }
    else if (pid == 0)
    {

        dup2(log, 1);
        cout << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << "  New child process using execlp" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << endl;
        close(log);
        dup2(currentDescriptor, 1);
        execvp(currentCommand.simpleCommands[0].execArguments[0], currentCommand.simpleCommands[0].execArguments);
        cout << "execlp() call failed" << endl;
        exit(127);
    }
    else
    {
        if (currentCommand.simpleCommands[0].background)
        {
            dup2(log, 1);
            cout << endl;
            cout << "  --------------------------------------------------------------" << endl;
            cout << "  Parent process is waiting for its child" << endl;
            cout << "  --------------------------------------------------------------" << endl;
            cout << endl;
            close(log);
            waitpid(pid, 0, 0);
        }

        dup2(log, 1);
        cout << endl;
        cout << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << "  Parent process: Child process ended" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << endl;
        cout << endl;
        close(log);
    }
}

void forkProcessPipe(Command &currentCommand)
{
    cout << "\033[1;32m";

    int mode[2];

    for (int i = 0; i < 2; i++)
    {
        if (currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
            mode[i] = 0;
        if (currentCommand.simpleCommands[i].simpleCommandType == "append")
            mode[i] = 1;
    }

    setFileDescriptor(currentCommand, 0, mode[0]);
    setFileDescriptor(currentCommand, 1, mode[1]);

    int pipefd[2];
    int pid;

    int currentDescriptor = dup(1);
    int log = open(currentCommand.defaultlog.c_str(), O_RDWR | O_CREAT | O_APPEND);

    pipe(pipefd);

    pid = fork();

    if (pid == 0)
    {
        dup2(log, 1);
        cout << "  --------------------------------------------------------------" << endl;
        cout << "  New child process 2 using pipe" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        dup2(currentDescriptor, 1);
        dup2(pipefd[0], 0);
        close(pipefd[1]);
        execvp(currentCommand.simpleCommands[1].execArguments[0], currentCommand.simpleCommands[1].execArguments);
    }
    else
    {
        dup2(log, 1);
        cout << "  --------------------------------------------------------------" << endl;
        cout << "  New child process 1 using pipe" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        dup2(currentDescriptor, 1);
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        execvp(currentCommand.simpleCommands[0].execArguments[0], currentCommand.simpleCommands[0].execArguments);
    }
}

void changeDirectory(Command currentCommand)
{
    string str;
    char s[100];

    cout << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << "Currrent working directory: " << getcwd((char *)str.c_str(), 100) << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << endl;

    str = currentCommand.simpleCommands[0].arguments[0];
    chdir(str.c_str());

    cout << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << "New working directory: " << getcwd((char *)str.c_str(), 100) << endl;
    cout << "  --------------------------------------------------------------" << endl;
    cout << endl;
}

void forkProcessDoublePipe(Command &currentCommand)
{
    cout << "\033[1;32m";

    int mode[3];

    for (int i = 0; i < 3; i++)
    {
        if (currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
            mode[i] = 0;
        if (currentCommand.simpleCommands[i].simpleCommandType == "append")
            mode[i] = 1;
    }

    setFileDescriptor(currentCommand, 0, mode[0]);
    setFileDescriptor(currentCommand, 1, mode[1]);
    setFileDescriptor(currentCommand, 2, mode[2]);

    int status;
    int i;

    int currentDescriptor = dup(1);
    int log = open(currentCommand.defaultlog.c_str(), O_RDWR | O_CREAT | O_APPEND);

    // 2 pipes each has 2 fds

    int pipes[4];
    pipe(pipes);     // sets up 1st pipe
    pipe(pipes + 2); // sets up 2nd pipe

    // pipes[0] = read  end of command 2
    // pipes[1] = write end of command 1
    // pipes[2] = read  end of command 3
    // pipes[3] = write end of command 2

    // fork the first child to execute first command

    cout << "\033[1;32m";

    if (fork() == 0)
    {
        dup2(log, 1);
        cout << "  --------------------------------------------------------------" << endl;
        cout << "  New child process 1 using double pipe" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        dup2(currentDescriptor, 1);

        // replace first command stdout with write part of pipe 1

        dup2(pipes[1], 1);

        // close all pipes

        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);

        execvp(currentCommand.simpleCommands[0].execArguments[0], currentCommand.simpleCommands[0].execArguments);
    }
    else
    {
        // fork second child to execute second command

        if (fork() == 0)
        {
            dup2(log, 1);
            cout << "  --------------------------------------------------------------" << endl;
            cout << "  New child process 2 using double pipe" << endl;
            cout << "  --------------------------------------------------------------" << endl;
            dup2(currentDescriptor, 1);

            // replace second command stdin with read end of pipe 1

            dup2(pipes[0], 0);

            // replace second command stdout with write end of pipe 2

            dup2(pipes[3], 1);

            // close all ends of pipes

            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);

            execvp(currentCommand.simpleCommands[1].execArguments[0], currentCommand.simpleCommands[1].execArguments);
        }
        else
        {
            // fork third child to execute third command

            if (fork() == 0)
            {
                dup2(log, 1);
                cout << "  --------------------------------------------------------------" << endl;
                cout << "  New child process 3 using pipe" << endl;
                cout << "  --------------------------------------------------------------" << endl;
                dup2(currentDescriptor, 1);

                // replace third command stdin with input read of pipe 2

                dup2(pipes[2], 0);

                // close all ends of pipes

                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);

                execvp(currentCommand.simpleCommands[2].execArguments[0], currentCommand.simpleCommands[2].execArguments);
            }
        }
    }

    // only the parent gets here and waits for 3 children to finish

    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);

    for (i = 0; i < 3; i++)
        wait(&status);
}

void wildcard(string s)
{
    char **found;
    glob_t gstruct;
    int r;

    r = glob(s.c_str(), GLOB_ERR, NULL, &gstruct);

    // check for errors
    if (r != 0)
    {
        if (r == GLOB_NOMATCH)
            fprintf(stderr, "No matches\n");
        else
            fprintf(stderr, "Error\n");
        exit(1);
    }

    // success
    found = gstruct.gl_pathv;
    cout << "\033[0;35m  --------------------------------------------------------------" << endl;
    while (*found)
    {
        printf("  \033[1;32m%s\n", *found);
        found++;
    }
    cout << "\033[0;35m  --------------------------------------------------------------" << endl;
    cout << "\033[0m";
}
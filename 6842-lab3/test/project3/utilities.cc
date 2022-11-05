#include "utilities.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace boost::algorithm;

void tokenize(Command &currentCommand)
{
    string command = "";
    string token = "";
    vector<string> commands;
    vector<vector<string>> tokens;

    getline(cin, command);
    cout << "\033[01;33m" << endl;

    if (command == "exit")
    {
        cout << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << "Good bye!!" << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << endl;
        exit(0);
    }

    stringstream pipeTokenizer(command);

    // pipe tokenizing
    while (getline(pipeTokenizer, token, '|'))
    {
        trim(token);
        commands.push_back(token);
    }

    // space tokenizing
    for (int i = 0; i < commands.size(); i++)
    {
        vector<string> row;
        stringstream tokenizer(commands[i]);
        while (getline(tokenizer, token, ' '))
        {
            trim(token);
            row.push_back(token);
        }
        tokens.push_back(row);
    }

    // arguments
    for (int i = 0; i < tokens.size(); i++)
    {
        currentCommand.simpleCommand.command = tokens[i][0];
        for (int j = 1; j < tokens[i].size(); j++)
        {
            currentCommand.simpleCommand.arguments.push_back(tokens[i][j]);
        }
        currentCommand.simpleCommands.push_back(currentCommand.simpleCommand);
        currentCommand.simpleCommand.arguments.clear();
    }
}

void execArguments(SimpleCommand &currentSimpleCommand, int cutPosition)
{
    // execArguments
    currentSimpleCommand.execArguments[0] = &currentSimpleCommand.command[0];

    int k = -1;
    for (int j = 0; j < currentSimpleCommand.arguments.size() - cutPosition; j++)
    {
        currentSimpleCommand.execArguments[j + 1] = &currentSimpleCommand.arguments[j][0];
        k++;
    }

    currentSimpleCommand.execArguments[k + 2] = NULL;
}

void Command::print()
{
    cout << endl;
    cout << endl;
    cout << "                         COMMAND TABLE                " << endl;
    cout << endl;
    cout << "  #   Simple Commands" << endl;

    for (int i = 0; i < simpleCommands.size(); i++)
    {
        cout << "  --- ----------------------------------------------------------" << endl;
        cout << "  " << i << "   " << simpleCommands[i].command;
        for (int j = 0; j < simpleCommands[i].arguments.size(); j++)
        {
            cout << " " << simpleCommands[i].arguments[j];
        }
        cout << endl;
        cout << "  --------------------------------------------------------------" << endl;
        cout << endl;
        if (simpleCommands[i].outputFile != "")
            cout << "  Output: " << simpleCommands[i].outputFile;
        else
            cout << "  Output: default"; // default output file descriptor
        cout << endl;
        if (simpleCommands[i].inputFile.size() != 0)
        {
            cout << endl;
            for (int k = 0; k < simpleCommands[i].inputFile.size(); k++)
                cout << "  " << k << " Input : " << simpleCommands[i].inputFile[k] << endl;
        }
        else
            cout << "  Input : default"; // default input file descriptor
        cout << endl;
        if (simpleCommands[i].errorFile != "")
            cout << "  Error : " << simpleCommands[i].errorFile;
        else
            cout << "  Error : default"; // default error file descriptor
        cout << endl;
    }
    cout << endl;
}

void classifyCommand(Command &currentCommand)
{
    if (currentCommand.simpleCommands.size() == 1)
        currentCommand.commandType = "single";
    else if (currentCommand.simpleCommands.size() == 2)
        currentCommand.commandType = "pipe";
    else if (currentCommand.simpleCommands.size() == 3)
        currentCommand.commandType = "doublePipe";

    for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
    {
        int sizeOfArguments = currentCommand.simpleCommands[i].arguments.size();

        if (currentCommand.simpleCommands[i].arguments[sizeOfArguments - 1] == "&")
            currentCommand.simpleCommands[i].background = true;

        for (int j = 0; j < sizeOfArguments; j++)
        {
            if (currentCommand.simpleCommands[i].arguments[j] == ">")
                currentCommand.simpleCommands[i].simpleCommandType = "rewrite";
            if (currentCommand.simpleCommands[i].arguments[j] == ">>")
                currentCommand.simpleCommands[i].simpleCommandType = "append";
        }
    }
}

void modifyCommand(Command &currentCommand)
{
    int defaultout = dup(1);

    for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
    {
        if (currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 4; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j]);
            }
            currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2];
            modifyFileDescriptorRewrite(currentCommand, i, 2);
            execArguments(currentCommand.simpleCommands[i], 3);
            continue;
        }
        else if (!currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 3; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j]);
            }
            currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1];
            modifyFileDescriptorRewrite(currentCommand, i, 1);
            execArguments(currentCommand.simpleCommands[i], 2);
            continue;
        }

        if (currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "append")
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 4; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j]);
            }
            currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2];
            modifyFileDescriptorAppend(currentCommand, i, 2);
            execArguments(currentCommand.simpleCommands[i], 3);
            continue;
        }
        else if (!currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "append")
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 3; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j]);
            }
            currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1];
            modifyFileDescriptorAppend(currentCommand, i, 1);
            execArguments(currentCommand.simpleCommands[i], 2);
            continue;
        }

        if (currentCommand.simpleCommands[i].background)
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 2; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2 - j]);
            }
            execArguments(currentCommand.simpleCommands[i], 1);

            continue;
        }
        else if (!currentCommand.simpleCommands[i].background)
        {
            for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 1; j++)
            {
                if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1 - j][0] == '-')
                    continue;
                currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1 - j]);
            }
            execArguments(currentCommand.simpleCommands[i], 0);

            continue;
        }
    }
    int currentDescriptor = dup(1);
    dup2(defaultout, 1);
    close(defaultout);
    currentCommand.print();
    dup2(currentDescriptor, 1);
}

void modifyFileDescriptorRewrite(Command &currentCommand, int index, int position)
{
    currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    dup2(currentCommand.simpleCommands[index].file_desc, 1);
    close(currentCommand.simpleCommands[index].file_desc);
}

void modifyFileDescriptorAppend(Command &currentCommand, int index, int position)
{
    currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), O_RDWR | O_CREAT | O_APPEND);
    dup2(currentCommand.simpleCommands[index].file_desc, 1);
    close(currentCommand.simpleCommands[index].file_desc);
}

void forkProcces(Command &currentCommand)
{
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

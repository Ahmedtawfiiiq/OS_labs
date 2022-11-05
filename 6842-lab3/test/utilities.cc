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
        if (simpleCommands[i].outputFile != "")
            cout << "  Output: " << simpleCommands[i].outputFile;
        else
            cout << "  Output: default"; // default output file descriptor
        cout << endl;
        if (simpleCommands[i].inputFile != "")
            cout << "  Input : " << simpleCommands[i].inputFile;
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
    for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
    {
        if (currentCommand.simpleCommands.size() == 1)
            currentCommand.commandType = "single";
        else
            currentCommand.commandType = "pipe";

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
    for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
    {
        if (currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
        {
            modifyFileDescriptorRewrite(currentCommand, i, 2);
            execArguments(currentCommand.simpleCommands[i], 3);
            continue;
        }
        else if (!currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
        {
            modifyFileDescriptorRewrite(currentCommand, i, 1);
            execArguments(currentCommand.simpleCommands[i], 2);
            continue;
        }

        if (currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "append")
        {
            modifyFileDescriptorAppend(currentCommand, i, 2);
            execArguments(currentCommand.simpleCommands[i], 3);
            continue;
        }
        else if (!currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "append")
        {
            modifyFileDescriptorAppend(currentCommand, i, 1);
            execArguments(currentCommand.simpleCommands[i], 2);
            continue;
        }

        if (currentCommand.simpleCommands[i].background)
        {
            execArguments(currentCommand.simpleCommands[i], 1);
            currentCommand.simpleCommands[i].inputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2];
            continue;
        }
        else if (!currentCommand.simpleCommands[i].background)
        {
            execArguments(currentCommand.simpleCommands[i], 0);
            currentCommand.simpleCommands[i].inputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1];
            continue;
        }
    }
}

void modifyFileDescriptorRewrite(Command &currentCommand, int index, int position)
{
    currentCommand.simpleCommands[index].outputFile = currentCommand.simpleCommands[index].arguments[currentCommand.simpleCommands[index].arguments.size() - position];
    currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    dup2(currentCommand.simpleCommands[index].file_desc, 1);
    close(currentCommand.simpleCommands[index].file_desc);
}

void modifyFileDescriptorAppend(Command &currentCommand, int index, int position)
{
    currentCommand.simpleCommands[index].outputFile = currentCommand.simpleCommands[index].arguments[currentCommand.simpleCommands[index].arguments.size() - position];
    currentCommand.simpleCommands[index].file_desc = open(currentCommand.simpleCommands[index].outputFile.c_str(), O_RDWR | O_CREAT | O_APPEND);
    dup2(currentCommand.simpleCommands[index].file_desc, 1);
    close(currentCommand.simpleCommands[index].file_desc);
}

void forkProcces(Command &currentCommand)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        cout << "A fork error has occurred" << endl;
        exit(-1); // fork failed
    }
    else if (pid == 0)
    {
        cout << "New child process using execlp" << endl;
        modifyCommand(currentCommand);
        execvp(currentCommand.simpleCommands[0].execArguments[0], currentCommand.simpleCommands[0].execArguments);
        cout << "execlp() call failed" << endl;
        exit(127);
    }
    else
    {
        if (currentCommand.simpleCommands[0].background)
            wait(0);
        cout << endl;
        cout << "Parent process: Child process ended" << endl;
        cout << endl;
    }
}

void forkProcessPipe(Command &currentCommand)
{
    int pipefd[2];
    int pid;

    pipe(pipefd);

    pid = fork();

    modifyCommand(currentCommand);

    if (pid == 0)
    {
        dup2(pipefd[0], 0);
        close(pipefd[1]);
        execvp(currentCommand.simpleCommands[1].execArguments[0], currentCommand.simpleCommands[1].execArguments);
    }
    else
    {
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        execvp(currentCommand.simpleCommands[0].execArguments[0], currentCommand.simpleCommands[0].execArguments);
    }
}
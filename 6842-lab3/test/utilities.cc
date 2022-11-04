#include "utilities.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
    {
        currentCommand.simpleCommands[i].execArguments[0] = &currentCommand.simpleCommands[i].command[0];

        int k = -1;
        for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size(); j++)
        {
            if (currentCommand.simpleCommands[i].arguments[j] != "&")
            {
                currentCommand.simpleCommands[i].execArguments[j + 1] = &currentCommand.simpleCommands[i].arguments[j][0];
                k++;
            }
            else
                currentCommand.simpleCommands[i].background = true; // set background flag
        }

        currentCommand.simpleCommands[i].execArguments[k + 2] = NULL;
    }
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

void forkProcces(SimpleCommand currentSimpleCommand)
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
        execvp(currentSimpleCommand.execArguments[0], currentSimpleCommand.execArguments);
        cout << "execlp() call failed" << endl;
        exit(127);
    }
    else
    {
        if (currentSimpleCommand.background)
            wait(0);

        cout << endl;
        cout << "Parent process: Child process ended" << endl;
        cout << endl;
    }
}

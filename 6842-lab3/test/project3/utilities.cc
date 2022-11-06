#include "utilities.hpp"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

void Command::print()
{
    cout << endl;
    cout << endl;
    cout << "\033[1;32m                         COMMAND TABLE                " << endl;
    cout << endl;
    cout << "  \033[1;32m#   \033[1;36mSimple Commands" << endl;

    for (int i = 0; i < simpleCommands.size(); i++)
    {
        cout << "  \033[0;35m--------------------------------------------------------------" << endl;
        cout << "  \033[1;32m" << i + 1 << "   \033[1;36m" << simpleCommands[i].command;
        for (int j = 0; j < simpleCommands[i].arguments.size(); j++)
        {
            cout << " " << simpleCommands[i].arguments[j];
        }
        cout << endl;
        cout << "  \033[0;35m--------------------------------------------------------------" << endl;
        cout << endl;
        if (simpleCommands[i].outputFile != "")
            cout << "  \033[1;31mOutput: \033[01;33m" << simpleCommands[i].outputFile;
        else
            cout << "  \033[1;31mOutput: \033[01;33mdefault"; // default output file descriptor
        cout << endl;
        if (simpleCommands[i].inputFile.size() != 0)
        {
            cout << endl;
            for (int k = 0; k < simpleCommands[i].inputFile.size(); k++)
                cout << "\033[1;31m  " << k + 1 << " Input : \033[01;33m" << simpleCommands[i].inputFile[k] << endl;
        }
        else
            cout << "\033[1;31m  Input : \033[01;33mdefault"; // default input file descriptor
        cout << endl;
        if (simpleCommands[i].errorFile != "")
            cout << "\033[1;31m  Error : \033[01;33m" << simpleCommands[i].errorFile;
        else
            cout << "\033[1;31m  Error : \033[01;33mdefault"; // default error file descriptor
        cout << endl;
    }
    cout << endl;
}

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

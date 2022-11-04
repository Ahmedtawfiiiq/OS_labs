#include "utilities.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <cstring>
#include <dirent.h>

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
            cout << currentCommand.simpleCommand.arguments[0];
        }
        currentCommand.simpleCommands.push_back(currentCommand.simpleCommand);
        currentCommand.simpleCommand.arguments.clear();
    }
}

void listDirectory(string path)
{
    if (path == "")
        path = ".";

    struct dirent *d;
    DIR *dr;
    dr = opendir(path.c_str());
    if (dr != NULL)
    {
        cout << "  List of files and folders in \"" << path << "\"" << endl;
        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            cout << "  " << d->d_name << endl;
        }
        closedir(dr);
    }
    else
    {
        cout << endl;
        cout << "Error Occurred!";
    }
    cout << endl;
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
            cout << "  Output: default";
        cout << endl;
        if (simpleCommands[i].inputFile != "")
            cout << "  Input : " << simpleCommands[i].inputFile;
        else
            cout << "  Input : default";
        cout << endl;
        if (simpleCommands[i].errorFile != "")
            cout << "  Error : " << simpleCommands[i].errorFile;
        else
            cout << "  Error : default";
        cout << endl;
    }
    cout << endl;
}
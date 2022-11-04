#include "utilities.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <cstring>
#include <dirent.h>
#include <fstream>

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
}

vector<string> readFile(string path)
{
    vector<string> fileContent;
    ifstream fin(path);
    while (!fin.eof())
    {
        string line;
        getline(fin, line);
        fileContent.push_back(line);
    }
    return fileContent;
}

vector<string> listDirectory(string path)
{
    vector<string> directoryDescription;
    string s = "";

    struct dirent *d;
    DIR *dr;
    dr = opendir(path.c_str());
    if (dr != NULL)
    {
        s = "List of files and folders in \"" + path + "\"";
        directoryDescription.push_back(s);

        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            s = d->d_name;
            directoryDescription.push_back(s);
        }
        closedir(dr);
    }
    else
    {
        s = "\nError Occurred!";
        directoryDescription.push_back(s);
    }
    s = "\n";
    directoryDescription.push_back(s);
    return directoryDescription;
}

void printContent(Command &currentCommand, int position)
{
    vector<string> description;
    for (int i = 0; i < currentCommand.simpleCommands[position].arguments.size(); i++)
    {
        if (currentCommand.simpleCommands[position].command == "ls")
        {
            description = listDirectory(currentCommand.simpleCommands[position].arguments[i]);
            currentCommand.simpleCommands[position].output = description;
            for (int j = 0; j < description.size(); j++)
            {
                cout << description[j] << endl;
            }
            cout << "  --------------------------------------------------------------" << endl;
        }
        if (currentCommand.simpleCommands[position].command == "cat")
        {
            description = readFile(currentCommand.simpleCommands[position].arguments[i]);
            currentCommand.simpleCommands[position].output = description;
            for (int j = 0; j < description.size(); j++)
            {
                cout << description[j] << endl;
            }
            cout << "  --------------------------------------------------------------" << endl;
        }
        if (currentCommand.simpleCommands[position].command == "grep")
        {
            vector<string> fileDescription;
            string s = currentCommand.simpleCommands[position].arguments[i];
            description = currentCommand.simpleCommands[position - 1].output;
            size_t found;
            for (int j = 0; j < description.size(); j++)
            {
                found = description[j].find(s);
                if (found != string::npos)
                {
                    fileDescription.push_back(description[j]);
                    cout << description[j] << endl;
                }
            }
            currentCommand.simpleCommands[position].output = fileDescription;
            cout << "  --------------------------------------------------------------" << endl;
        }
    }
}

void fileRewrite(string sourcePath, string targetPath)
{
    vector<string> description;
    description = listDirectory(sourcePath);

    ofstream fout(targetPath);

    for (int i = 0; i < description.size(); i++)
    {
        fout << description[i] << endl;
    }
    fout.close();
}

void fileAppend(string sourcePath, string targetPath)
{
    vector<string> description;
    description = listDirectory(sourcePath);

    ofstream fout(targetPath, ios::app);

    for (int i = 0; i < description.size(); i++)
    {
        fout << description[i] << endl;
    }
    fout.close();
}

bool singleAngle(Command currentCommand, int position)
{
    for (int j = 0; j < currentCommand.simpleCommands[position].arguments.size(); j++)
    {
        if (currentCommand.simpleCommands[position].arguments[j] == ">") // append
        {
            if (currentCommand.simpleCommands[position].command == "ls")
            {
                for (int k = 0; k < j; k++)
                {
                    fileRewrite(currentCommand.simpleCommands[position].arguments[k], currentCommand.simpleCommands[position].arguments[j + 1]);
                }
            }
            return true;
        }
    }
    return false;
}

bool doubleAngle(Command currentCommand, int position)
{
    for (int j = 0; j < currentCommand.simpleCommands[position].arguments.size(); j++)
    {
        if (currentCommand.simpleCommands[position].arguments[j] == ">>") // append
        {
            if (currentCommand.simpleCommands[position].command == "ls")
            {
                for (int k = 0; k < j; k++)
                {
                    fileAppend(currentCommand.simpleCommands[position].arguments[k], currentCommand.simpleCommands[position].arguments[j + 1]);
                }
            }
            return true;
        }
    }
    return false;
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
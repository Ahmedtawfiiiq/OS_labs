#include "utilities.h"
#include <iostream>

using namespace std;

void prompt()
{
    cout << "myshell> ";
}

int main()
{

    while (true)
    {
        prompt();

        Command currentCommand;

        tokenize(currentCommand);

        currentCommand.print();

        for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
        {
            if (currentCommand.simpleCommands[i].command == "ls")
            {
                for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size(); j++)
                {
                    listDirectory(currentCommand.simpleCommands[i].arguments[j]);
                }
            }
        }
    }
    return 0;
}

// Command c1;
// SimpleCommand s1;
// s1.command = "ls";
// s1.arguments.push_back("/file1.txt");
// s1.arguments.push_back("/directory1");
// s1.inputFile = "/home";
// s1.errorFile = "/ahmed";

// SimpleCommand s2;
// s2.command = "touch";
// s2.arguments.push_back("/file2.txt");
// s2.arguments.push_back("/directory2");
// s2.errorFile = "/home/desktop";
// s2.outputFile = "/ahmed/documents";
// c1.simpleCommands.push_back(s1);
// c1.simpleCommands.push_back(s2);

// c1.print();
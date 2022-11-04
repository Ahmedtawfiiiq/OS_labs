#ifndef command_h
#define command_h

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class SimpleCommand
{
public:
    string command;
    vector<string> arguments;
    string outputFile;
    string inputFile;
    string errorFile;
    SimpleCommand() : outputFile(""), inputFile(""), errorFile("") {}
};

class Command
{
public:
    SimpleCommand simpleCommand;
    vector<SimpleCommand> simpleCommands;

public:
    void print();
};

void tokenize(Command &);

void listDirectory(string);

#endif

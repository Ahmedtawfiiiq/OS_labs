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

    vector<string> output;
    vector<string> input;
    vector<string> error;

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

vector<string> readFile(string);

vector<string> listDirectory(string);

void fileRewrite(string, string);

void fileAppend(string, string);

bool singleAngle(Command, int);

bool doubleAngle(Command, int);

void printContent(Command &, int);

#endif

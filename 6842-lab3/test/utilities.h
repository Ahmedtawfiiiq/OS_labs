#ifndef command_h
#define command_h

#include <vector>
#include <string>

#define SIZE 10

using namespace std;

class SimpleCommand
{
public:
    string command;
    char *execArguments[SIZE];
    vector<string> arguments;

    int file_desc;
    string outputFile;
    string inputFile;
    string errorFile;
    bool background;

    SimpleCommand() : outputFile(""), inputFile(""), errorFile(""), background(false) {}
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

void forkProcces(SimpleCommand);

#endif

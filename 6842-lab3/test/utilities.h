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
    string simpleCommandType;
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
    string commandType;
    SimpleCommand simpleCommand;
    vector<SimpleCommand> simpleCommands;

public:
    void print();
};

void tokenize(Command &);

void classifyCommand(Command &);

void modifyCommand(Command &);

void modifyFileDescriptorRewrite(Command &, int, int);

void modifyFileDescriptorAppend(Command &, int, int);

void execArguments(Command &, int, int cutPosition);

void forkProcces(Command &);

void forkProcessPipe(Command &);

#endif

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

        bool lsFlag = false;
        for (int i = 0; i < currentCommand.simpleCommands.size(); i++)
        {
            if (!lsFlag)
                lsFlag = doubleAngle(currentCommand, i);
            if (!lsFlag)
                lsFlag = singleAngle(currentCommand, i);
            if (!lsFlag)
                printContent(currentCommand, i);
        }
    }
    return 0;
}

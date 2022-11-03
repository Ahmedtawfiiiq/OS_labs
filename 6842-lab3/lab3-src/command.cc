
/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

#include "command.h"

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **)malloc(_numberOfAvailableArguments * sizeof(char *));
}

void SimpleCommand::insertArgument(char *argument)
{
	if (_numberOfAvailableArguments == _numberOfArguments + 1)
	{
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **)realloc(_arguments,
									  _numberOfAvailableArguments * sizeof(char *));
	}

	_arguments[_numberOfArguments] = argument;

	// Add NULL argument at the end
	_arguments[_numberOfArguments + 1] = NULL;

	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc(_numberOfSimpleCommands * sizeof(SimpleCommand *));

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
	flagList = 1;
	flagRewrite = 0;
	flagAppend = 0;
	flagPipe = 0;
	flagCat = 1;
}

void Command::insertSimpleCommand(SimpleCommand *simpleCommand)
{
	if (_numberOfAvailableSimpleCommands == _numberOfSimpleCommands)
	{
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **)realloc(_simpleCommands,
													_numberOfAvailableSimpleCommands * sizeof(SimpleCommand *));
	}

	_simpleCommands[_numberOfSimpleCommands] = simpleCommand;
	_numberOfSimpleCommands++;
}

void Command::clear()
{
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			free(_simpleCommands[i]->_arguments[j]);
		}

		free(_simpleCommands[i]->_arguments);
		free(_simpleCommands[i]);
	}

	if (_outFile)
	{
		free(_outFile);
	}

	if (_inputFile)
	{
		free(_inputFile);
	}

	if (_errFile)
	{
		free(_errFile);
	}

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
	flagList = 1;
	flagRewrite = 0;
	flagAppend = 0;
	flagPipe = 0;
	flagCat = 1;
}

void Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		printf("  %-3d ", i);
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[j]);
		}
	}

	printf("\n\n");
	printf("  Output       Input        Error        Background\n");
	printf("  ------------ ------------ ------------ ------------\n");
	printf("  %-12s %-12s %-12s %-12s\n", _outFile ? _outFile : "default",
		   _inputFile ? _inputFile : "default", _errFile ? _errFile : "default",
		   _background ? "YES" : "NO");
	printf("\n\n");
}

void Command::execute_ls()
{
	string files[50];
	static int i = 0;
	struct dirent *d;
	DIR *dr;

	if (_simpleCommands[0]->_numberOfArguments == 1)
	{
		dr = opendir(".");

		if (dr != NULL)
		{
			cout << "List of Files & Folders in current directory\n\n";
			for (d = readdir(dr); d != NULL; d = readdir(dr))
			{
				files[i] = d->d_name;
				i++;
			}
			closedir(dr);
		}
		else
			cout << "\nError Occurred!";

		for (int j = 0; j < i; j++)
		{
			cout << files[j] << endl;
		}
		i = 0;
		cout << endl;
	}

	else if (_simpleCommands[0]->_numberOfArguments > 1)
	{
		for (int k = 0; k < _simpleCommands[0]->_numberOfArguments - 1; k++)
		{
			dr = opendir(_simpleCommands[0]->_arguments[k + 1]);
			if (dr != NULL)
			{
				cout << "List of Files & Folders in " << _simpleCommands[0]->_arguments[k + 1] << "\n\n";
				for (d = readdir(dr); d != NULL; d = readdir(dr))
				{
					files[i] = d->d_name;
					i++;
				}
				closedir(dr);
			}
			else
				cout << "\nError Occurred!";

			for (int j = 0; j < i; j++)
			{
				cout << files[j] << endl;
			}
			i = 0;
			cout << endl;
		}
	}
}

void Command::execute_ls_write()
{
	string files[50];
	static int i = 0;
	struct dirent *d;
	DIR *dr;

	dr = opendir(_simpleCommands[0]->_arguments[1]);

	if (dr != NULL)
	{
		for (d = readdir(dr); d != NULL; d = readdir(dr))
		{
			files[i] = d->d_name;
			i++;
		}
		closedir(dr);
	}
	else
		cout << "\nError Occurred!";

	ofstream fout(_outFile);
	for (int j = 0; j < i; j++)
	{
		fout << files[j] << endl;
	}
	fout.close();
	i = 0;
	cout << endl;
}

void Command::execute_ls_append()
{
	string files[50];
	static int i = 0;
	struct dirent *d;
	DIR *dr;

	dr = opendir(_simpleCommands[0]->_arguments[1]);

	if (dr != NULL)
	{
		for (d = readdir(dr); d != NULL; d = readdir(dr))
		{
			files[i] = d->d_name;
			i++;
		}
		closedir(dr);
	}
	else
		cout << "\nError Occurred!";

	ofstream fout(_outFile, ios::app);
	for (int j = 0; j < i; j++)
	{
		fout << files[j] << endl;
	}
	fout.close();
	i = 0;
	cout << endl;
}

void Command::execute_cat()
{
	ifstream fin(_simpleCommands[0]->_arguments[1]);
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		cout << line << endl;
	}
}

void Command::execute_grep()
{
	ifstream fin(_simpleCommands[0]->_arguments[1]);
	int i  = 0;
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		size_t found = line.find(_pipeText);
		if (found != string::npos){
			cout << line << endl;
		}
	}
}

void Command::execute()
{
	// Don't do anything if there are no simple commands
	if (_numberOfSimpleCommands == 0)
	{
		prompt();
		return;
	}

	// Print contents of Command data structure
	print();

	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec

	const char *c1 = "ls";
	const char *c2 = "cat";
	const char *c3 = "grep";

	if (string(_simpleCommands[0]->_arguments[0]) == string(c1))
	{
		if (flagList == 1)
			execute_ls();

		if (flagRewrite == 1)
			execute_ls_write();

		if (flagAppend == 1)
			execute_ls_append();
	}
	else if (string(_simpleCommands[0]->_arguments[0]) == string(c2) && flagCat == 1)
	{
		execute_cat();
	}
	else if (flagPipe == 1)
	{
		execute_grep();
	}

	// Clear to prepare for next command
	clear();

	// Print new prompt
	prompt();
}

// Shell implementation

void Command::prompt()
{
	printf("myshell>");
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand *Command::_currentSimpleCommand;

int yyparse(void);

int main()
{
	Command::_currentCommand.prompt();
	yyparse();
	return 0;
}

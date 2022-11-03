
#ifndef command_h
#define command_h

#include <string>

// Command Data Structure
struct SimpleCommand
{
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;

	// Number of arguments
	int _numberOfArguments;
	char **_arguments;

	SimpleCommand();
	void insertArgument(char *argument);
};

struct Command
{
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand **_simpleCommands;
	char *_outFile;
	char *_inputFile;
	char *_errFile;
	int _background;
	int flagList;
	int flagRewrite;
	int flagAppend;
	char * _pipeText;
	int flagPipe;
	int flagCat;

	void prompt();
	void print();
	void execute();
	void clear();
	void execute_ls();
	void execute_ls_write();
	void execute_ls_append();
	void execute_cat();
	void execute_grep();

	Command();
	void insertSimpleCommand(SimpleCommand *simpleCommand);

	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;
};

#endif

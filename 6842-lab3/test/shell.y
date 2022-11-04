
/*
 * CS-413 Spring 98
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%union	{char *string_val;}

%token	<string_val> WORD

%token NOTOKEN GREAT NEWLINE 

%{
extern "C" 
{
	int yylex();
	void yyerror (char const *s);
}
#define yylex yylex
#include <stdio.h>
#include <string>
#include "command.h"
%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	command_and_args iomodifier_opt NEWLINE {
		printf("   Yacc: Execute command\n");
		Command::execute();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word arg_list {}
	;

arg_list:
	arg_list argument
	| /* can be empty */
	;

argument:
	WORD {
        printf("   Yacc: insert argument \"%s\"\n", $1);
		Command::currentCommand.arguments.push_back(string($1));\
	}
	;

command_word:
	WORD {
        printf("   Yacc: insert command \"%s\"\n", $1);
		Command::currentCommand.command = string($1);
	}
	;

iomodifier_opt:
	GREAT WORD {
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::currentCommand.outputFile = string($2);
	}
	| /* can be empty */ 
	;

%%

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif

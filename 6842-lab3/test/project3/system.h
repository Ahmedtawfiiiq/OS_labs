#ifndef system_h
#define system_h

#include "utilities.h"


void setFileDescriptor(Command &, int, int);

void forkProcces(Command &);

void forkProcessPipe(Command &);

void changeDirectory(Command);

void forkProcessDoublePipe(Command &);

#endif

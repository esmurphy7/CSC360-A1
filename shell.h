#include "parser.h"
#include "linkedlist.h"

char cwd[1024];

void printPrompt();
void executeCmdLine(char**);
static void signalHandler (int);
void broadcastTermination(int, int);

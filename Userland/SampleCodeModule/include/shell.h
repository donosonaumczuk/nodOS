#ifndef _shell_h
#define _shell_h

#include <stdint.h>

#define MAX_CMD_LONG                    100
#define MAX_VALID_CMD_LONG              18
#define MAX_PIPES_QUANTITY_ON_COMMAND   20
#define MAX_PIPE_LENGTH_NAME            100


#define	EXIT_CMD	         -1
#define	VALID_CMD 		0
#define   INVALID_CMD		1
#define	ERROR_CMD		     2
#define	ARGS_ERROR		3
#define   HIST_LONG           100

#define UP_ARROW              133
#define DOWN_ARROW            134
#define LEFT_ARROW            135
#define RIGHT_ARROW           136

#define EXIT_CMD             -1
#define VALID_CMD             0
#define INVALID_CMD           1
#define ERROR_CMD             2
#define ARGS_ERROR            3

#define PROMPT                                    "nodOS:$    "
#define INVALID_COMMAND_STR             "Error: Invalid command\n"
#define COMMAND_EXECUTED_WITH_ERROR_STR "Error: Command executed with error\n"
#define COMMAND_NOT_EXECUTED_ARGS_STR   "Error: Command not executed because of arguments error\n"


int shell();
void receiveTest();
void sendTest();
void sendTest2();
void receiveTest2();
void sendTest3();
void receiveTest3();
int lookForPipes(unsigned char *buffer, unsigned int index, unsigned int *exitFlag);
int callWithPipes(unsigned char *command, unsigned int index, unsigned int *exitFlag,
                    char *stdin, char *stdout);
void validateCommand(unsigned char *buffer, unsigned int index, unsigned int *exitFlag,
                         char *stdin, char *stdout);
void generatePipeName(char pipes[MAX_PIPES_QUANTITY_ON_COMMAND][MAX_PIPE_LENGTH_NAME],
                          int pipesFound);
void terminateAllPipes(char pipes[MAX_PIPES_QUANTITY_ON_COMMAND][MAX_PIPE_LENGTH_NAME],
                          int pipesFound);
#endif
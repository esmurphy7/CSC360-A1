#include "parser.h"

//======= Constants ==========
static int const MAX_LINE_LENGTH = 100;
//============================

// init cmd array[] and a cmdline array[][]
// iterate through user_input, storing each character into cmd array
// if space character is found
//		stop and store cmd array in cmdline array
// continue iterating until end of user_input
char** parse(char* input)
{
	char cmdSeps[4] = {' ', '\n', '\t', '\0'};
	char **cmdline = (char**)malloc(sizeof(char**)*MAX_LINE_LENGTH);		
	char* cmd = getNewArray();
	
	int i=0;
	while(*input != '\0')
	{
		//printf("current char: %c\n",*input);
		cmd = charAppend(cmd, *input);
		// current character is a whitespace character
		if(charInSet(*input, cmdSeps))
		{
			//printf("cmd: %s\n", cmd);
			// if multiple cmdSeps are entered in a row, cmd[0] will be a sep
			if(!charInSet(cmd[0], cmdSeps))
			{
				// replace the newline added at the beginning with null character
				cmd[strlen(cmd)-1] = '\0';
				cmdline[i] = cmd;
				//printf("cmdline[%d]: %s\n",i,cmdline[i]);
				i++;
			}
			// clear and re-init the cmd variable
			cmd=NULL;
			cmd = getNewArray();
		}
		input++;
	}
	free(cmd);
	
	return cmdline;
}
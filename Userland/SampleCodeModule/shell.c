#include	<lib.h>
#include	<string.h>
#include	<commandHandler.h>


void clearLine(unsigned int lineLong) {
	int i;
	for(i = 0;i < lineLong; i++){
		printf("\b");
	}
}

void clearBuffer(unsigned char buffer[]) {
	int i;
	for(i = 0; i < MAX_CMD_LONG; i++) {
		buffer[i] = 0;
	}
}

void clearHist(unsigned char hist[HIST_LONG][MAX_CMD_LONG]) {
	int i,j;
	i = j = 0;
	for(j = 0; j < HIST_LONG; j++) {
		for(i = 0; i<MAX_CMD_LONG; i++) {
			hist[j][i] = 0;
		}
	}
}

int shell() {
	unsigned int exitFlag = 0;
	unsigned char buffer[MAX_CMD_LONG];
	unsigned char currentChar;
	unsigned int index = 0;
	unsigned char hist[HIST_LONG][MAX_CMD_LONG];
	unsigned int histCurrentIndex = 0;
	unsigned int histAccessIndex = 0;
	unsigned int histSize = 0;

	clearBuffer(buffer);
	clearHist(hist);

	while(!exitFlag) {
		changeFontColor(LIGHT_GREY);
		printf(PROMPT);
		changeFontColor(WHITE);
		while((currentChar = getchar()) != '\n' && currentChar != '|') {

			if(currentChar == '\b') {	/* BACKSPACE */
			
				if(index > 0) {
					buffer[--index] = 0;
					printf("\b");
				}
			}
			
			else if(currentChar == UP_ARROW || currentChar == DOWN_ARROW) {		/* ANY ARROW */

				if(currentChar == UP_ARROW) {
			
					if(histAccessIndex > 0)	{
						histAccessIndex--;
					}

					else if(histSize > 0) {
						histAccessIndex = histSize - 1;
					}
				}

				else {
				
					if(histAccessIndex < (histSize - 1)) {
						histAccessIndex++;
					}

					else	{
						histAccessIndex = 0;
					}
				}
				
				clearLine(index);
				clearBuffer(buffer);
				index = 0;
				while(hist[histAccessIndex][index] != 0) {
					index++;
				}
				
				strcpy(buffer,hist[histAccessIndex]);
				printf("%s",buffer);
			}

			else {	//ANY CHARACTER
				
				if(index < MAX_CMD_LONG) {
					buffer[index++] = currentChar;
					putChar(currentChar);
				}
			}
		}
		if(currentChar == '\n') {
			printf("\n");
		}
		else {
			putChar(currentChar);
			printf("\n");


			if(index >= 1 && buffer[index - 1] != ' '){
				//printf(INVALID_COMMAND_STR);
			}
			index--;
			buffer[index] = 0;
			//makePipe();
		}
		switch (commandInterpreter(buffer, index)) {
			case INVALID_CMD: 	printf(INVALID_COMMAND_STR);	break;
			case ERROR_CMD: 	printf(COMMAND_EXECUTED_WITH_ERROR_STR);	break;
			case ARGS_ERROR:	printf(COMMAND_NOT_EXECUTED_ARGS_STR);	break;
			case EXIT_CMD:		exitFlag = 1;	break;
			case VALID_CMD:		break;
		}

		if(histCurrentIndex >= HIST_LONG) {
			histCurrentIndex = 0;
		}
		
		strcpy(hist[histCurrentIndex],buffer);
		histCurrentIndex++;
		
		if(histSize != HIST_LONG) {
			histSize++;
		}
		
		histAccessIndex = histCurrentIndex;
		clearBuffer(buffer);
		index = 0;
	}

	return 0;
}

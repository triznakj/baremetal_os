#define TRUE 1
#define FALSE 0


void handleFunction(char *input);

int stringEqual(char* cmd, char *input);

void parseCommand(char *input, char args[][10]);

/* Start shell commands */

void type(char * file);

void execute(char * file);

/* End shell commands */

void main() {
	char input[80];
	while(1) {
		interrupt(0x21, 0, "SHELL> \0", 0, 0);	
		interrupt(0x21, 1, input, 0, 0);
		interrupt(0x21, 0, input, 0, 0);
		handleFunction(input);	
	}
}

int stringEqual(char* cmd, char *input) {
	int i = 0;
	int result = TRUE;
	while(cmd[i] != '\0') {
		if (cmd[i] != input[i])	{
			result = FALSE;
			break;
		}
		i++;
	}
	if (cmd[i] != input[i]) {
		result = FALSE;
	}
	return result;
}

void handleFunction(char *input) {
	char arguments[10][10];
	parseCommand(input, arguments);
	if (stringEqual("type\0", arguments[0]) == TRUE) {
		type(arguments[1]);
	}
	else if (stringEqual("execute\0", arguments[0]) == TRUE) {
		execute(arguments[1]);
	}
	else {
		interrupt(0x21, 0, "Command not recognized!\r\n\0", 0, 0);	
	}
}


void parseCommand(char *input, char args[][10]) {
	int row = 0;
	int col = 0;
	int count = 0;
	while(input[count] != '\r') {
		if(input[count] != ' ') {
			args[row][col] = input[count];
			col++;
		} else {
			args[row][col] = '\0';
			row++;
			col = 0;
		}
		count++;
	}
	args[row][col] = '\0';
}

void type(char *file) {
	char buffer[13312];
	interrupt(0x21, 3, file, buffer, 0);
	interrupt(0x21, 0, buffer, 0, 0);
}

void execute(char *file) {
	interrupt(0x21, 4, file, 0x2000, 0);
}

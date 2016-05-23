#define TRUE 1
#define FALSE 0
#define MAX_FILE_BYTES 13312
#define MAX_NUM_LINES 100
#define MAX_CHARS_IN_LINE 100

void handleFunction(char *input);

int stringEqual(char* cmd, char *input);

void parseCommand(char *input, char args[][10]);

/* Start shell commands */

void type(char * file);

void execute(char * file);

void delete(char *file);

void copy(char *src, char *dst);

void createTextFile(char* file);

void dir();

/* End shell commands */

void main() {
	char input[80];
	while(1) {
		interrupt(0x21, 0, "SHELL> \0", 0, 0);	
		interrupt(0x21, 1, input, 0, 0);
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
	char types[5];
	char executes[10];
	char deletes[10];
	char creates[10];

	types[0] = 't';
	types[1] = 'y';
	types[2] = 'p';
	types[3] = 'e';
	types[4] = '\0';

	executes[0] = 'e';
	executes[1] = 'x';
	executes[2] = 'e';
	executes[3] = 'c';
	executes[4] = 'u';
	executes[5] = 't';
	executes[6] = 'e';
	executes[7] = '\0';

	deletes[0] = 'd';
	deletes[1] = 'e';
	deletes[2] = 'l';
	deletes[3] = 'e';
	deletes[4] = 't';
	deletes[5] = 'e';
	deletes[6] = '\0';

	creates[0] = 'c';
	creates[1] = 'r';
	creates[2] = 'e';
	creates[3] = 'a';
	creates[4] = 't';
	creates[5] = 'e';
	creates[6] = '\0';

	parseCommand(input, arguments);
	if (stringEqual(types, arguments[0]) == TRUE) {
		type(arguments[1]);
	} else if (stringEqual(executes, arguments[0]) == TRUE) {
		execute(arguments[1]);
	} else if (stringEqual(deletes, arguments[0]) == TRUE) {
		delete(arguments[1]);
	} else if (stringEqual(creates, arguments[0]) == TRUE) {
		createTextFile(arguments[1]);
	} else if (stringEqual("copy\0", arguments[0]) == TRUE) {
		copy(arguments[1], arguments[2]);
	} else if (stringEqual("dir\0" , arguments[0]) == TRUE){
		dir();
	} else {
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
	char buffer[MAX_FILE_BYTES];
	interrupt(0x21, 3, file, buffer, 0);
	interrupt(0x21, 0, buffer, 0, 0);
}

void execute(char *file) {
	interrupt(0x21, 4, file, 0x2000, 0);
}

void delete(char *file) {
	interrupt(0x21, 7, file, 0, 0);
}

void copy(char *src, char* dst) {
	char buffer[MAX_FILE_BYTES];
	int n;
	interrupt(0x21, 3, src, buffer, &n);
	interrupt(0x21, 8, dst, buffer, n);		
}

void createTextFile(char *file){
	char input[80];
	char buffer[MAX_FILE_BYTES];
	int index;
	int bufferIndex; /*Doubles as file size*/
	char giveLine[80];
	giveLine[0] = 'I';
	giveLine[1] = 'n';
	giveLine[2] = 'p';
	giveLine[3] = 'u';
	giveLine[4] = 't';
	giveLine[5] = ' ';
	giveLine[6] = 'L';
	giveLine[7] = 'i';
	giveLine[8] = 'n';
	giveLine[9] = 'e';
	giveLine[10] = ' ';
	giveLine[11] = '(';
	giveLine[12] = 'E';
	giveLine[13] = 'n';
	giveLine[14] = 't';
	giveLine[15] = 'e';
	giveLine[16] = 'r';
	giveLine[17] = ' ';
	giveLine[18] = 't';
	giveLine[19] = 'o';
	giveLine[20] = ' ';
	giveLine[21] = 'q';
	giveLine[22] = 'u';
	giveLine[23] = 'i';
	giveLine[24] = 't';
	giveLine[25] = ')';
	giveLine[26] = ' ';
	giveLine[27] = '\0';

	bufferIndex = 0;
	while(1) {
		interrupt(0x21, 0, giveLine, 0, 0);	
		interrupt(0x21, 1, input, 0, 0);

		/*If we have a blank string, quit*/
		if(input[0] == 0xd){
			break;
		}
		for(index = 0; index<80;index++){
			if(input[index] == '\0'){
				break;
			}else{
				buffer[bufferIndex] = input[index];
				bufferIndex = bufferIndex+1;
			}
		}

		/*interrupt(0x21, 0, buffer, 0, 0);*/
	}
	buffer[bufferIndex] = '\0';
	interrupt(0x21,8,file,buffer,1);
}

void dir(){
	interrupt(0x21,9,0,0,0);
}
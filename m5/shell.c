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

void clear();

void quitOS();

void kill(char* process);

void colorChange();



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
	char killcmd[10];
	char help[10];
	char clears[10];
	char quit[10];
	char colors[10];

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

	killcmd[0] = 'k';
	killcmd[1] = 'i';
	killcmd[2] = 'l';
	killcmd[3] = 'l';
	killcmd[4] = '\0';
	help[0] = 'h';
	help[1] = 'e';
	help[2] = 'l';
	help[3] = 'p';
	help[4] = '\0';

	quit[0] = 'q';
	quit[1] = 'u';
	quit[2] = 'i';
	quit[3] = 't';
	quit[4] = '\0';

	clears[0] = 'c';
	clears[1] = 'l';
	clears[2] = 'e';
	clears[3] = 'a';
	clears[4] = 'r';
	clears[5] = '\0';

	colors[0] = 'c';
	colors[1] = 'o';
	colors[2] = 'l';
	colors[3] = 'o';
	colors[4] = 'r';
	colors[5] = '\0';

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
	} else if (stringEqual("dir\0", arguments[0]) == TRUE){
		dir();
	} else if (stringEqual(killcmd, arguments[0]) == TRUE){
		kill(arguments[1]);
	} else if (stringEqual(help, arguments[0]) == TRUE){
		type("help.t\0");
	} else if (stringEqual(clears, arguments[0]) == TRUE){
		clear();
	} else if (stringEqual(quit, arguments[0]) == TRUE){
		quitOS();
	} else if(stringEqual(colors, arguments[0]) == TRUE){
                colorChange();
    } else if(stringEqual("execfg\0", arguments[0]) == TRUE){
                interrupt(0x21,12,arguments[1],0,0);	
	} else {
		interrupt(0x21, 0, "404\r\n\0", 0, 0);	
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

void clear(){
	interrupt(0x21,11,0,0,0);
}

void quitOS(){
	interrupt(0x15,0x5307,1,3,0);
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

void kill(char *process) {
	interrupt(0x21,10,process[0] - 0x30,0,0);
}

void colorChange(){
        int ah = 0x06;
        int al = 0x00;
        int bh = 0x14;
        int bl = 0;
        int ch = 0x0;
        int cl = 0x0;
        int dh = 0x50;
        int dl = 0x50;
        int ax;
        int bx;
        int cx;
        int dx;
       
        clear();
       
        ax = ah*256 + al;
        bx = bh*256+bl;
        cx = ch*256+cl;
        dx = dh*256+dl;
        interrupt(0x10,ax,bx,cx,dx);
}

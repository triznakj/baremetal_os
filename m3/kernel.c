/* Tyler Rockwood, Kevin Trizna, and Eric Downing */
/* Group 1-E */
void printString(char *string);

void readString(char *buffer);

void readSector(char *buffer, int sector);

void readFile(char *filename, char *buffer);

void executeProgram(char* name, int segment);

void terminate();

void handleInterrupt21(int ax, int bx, int cx, int dx);

int mod(int a, int b);

int div(int a, int b);

#define TRUE 1
#define FALSE 0

void main()
{
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	/* Make interrupt 21 be our function */
	makeInterrupt21();
	/* Test our function by calling interrupt 21 */
	executeProgram(shell, 0x2000);
}

void printString(char *string) {
	char current;
	int i = 0;
	current = string[i];
	while(current != '\0'){
		interrupt(0x10, 0xe*256+current, 0, 0, 0);	
		i = i + 1;
		current = string[i];
	}
}

void readString(char *buffer){
	int i = 0;
	char lastCharRead;
	do {
		lastCharRead = interrupt(0x16, 0, 0, 0, 0);
		if(lastCharRead != 0x8) {
			buffer[i] = lastCharRead;
			i = i + 1;
		} else if(i > 0) {
			i = i - 1;
		}
	} while(lastCharRead != 0xd);
	buffer[i] = 0xa;
	buffer[i + 1] = 0x0;
}

void readSector(char *buffer, int sector) {
	int ah = 2;
	int al = 1;
	int ch = div(sector, 36);
	int cl = mod(sector, 18) + 1;
	int dh = mod(div(sector, 18), 2);
	int ax = (ah * 256) + al;
	int cx = (ch * 256) + cl;
	int dx = (dh * 256) + 0;
	interrupt(0x13, ax, buffer, cx, dx);
}

void readFile(char *filename, char *buffer) {
	char dirList[512];
	int i = 0;
	int j = 0;
	int fileFound = FALSE;
	readSector(dirList, 2);
	for(i = 0; i < 16; i++) {
		for (j = 0; j < 6; j++)	{
			if (dirList[(32*i)+j]==filename[j]) {
				fileFound = TRUE;
			}
			else {
				fileFound = FALSE;
				break;
			}
		}
		if (fileFound == TRUE) {
			for (j = 6; j < 32; j++) {
				if (dirList[(32*i)+j] == '\0') {		
					break;
				}
				readSector(buffer + (512 * (j - 6)), dirList[(32*i)+j]);
			}
			break;
		}
	}
	if (fileFound == FALSE) {
		terminate();
	}
}

void executeProgram(char *name, int segment) {
	char buffer[13312];
	int address;
	readFile(name, buffer);
	for (address = 0; address < 0x1000; address++) {
		putInMemory(segment, address, buffer[address]);
	}
	launchProgram(segment);
}

void terminate() {
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	executeProgram(shell, 0x2000);
	while(1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else if (ax == 3) {
		readFile(bx, cx);
	} else if (ax == 4) {
		executeProgram(bx, cx);
	} else if (ax == 5) {
		terminate();
	} else {
		printString("404\0");
	}
}

int mod(int a, int b) {
	while(a >= b) {
		a = a - b;
	}
	return a;
}

int div(int a, int b) {
	int quotient = 0;
	while((quotient + 1) * b <= a) {
		quotient = quotient + 1;
	}
	return quotient;
}

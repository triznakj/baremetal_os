/* Tyler Rockwood, Kevin Trizna, and Eric Downing */
/* Group 1-E */
void printString(char *string);

void readString(char *buffer);

void readSector(char *buffer, int sector);

void handleInterrupt21(int ax, int bx, int cx, int dx);

int mod(int a, int b);

int div(int a, int b);

char line[80];

char buffer[512];

void main()
{
	printString("Hello\r\nKevin!\r\n\0");
	/* Make interrupt 21 be our function */
	makeInterrupt21();
	/* Test our function by calling interrupt 21 */
	interrupt(0x21, 2, buffer, 30, 0);
	printString(buffer);
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	/* Enter read-print loop */
	while(1) {
		printString("Enter a line: \0");
		readString(line);
		printString(line);
	}
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

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else {
		printString("What are you doing with 21!?!?!?\r\n\0");
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

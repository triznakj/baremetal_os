/* Tyler Rockwood, Kevin Trizna, and Eric Downing */
/* Group 1-E */
void printString(char *string);

void printChar(char c);

void readString(char *buffer);

void readSector(char *buffer, int sector, int *sectorsRead);

void writeSector(char *buffer, int sector);

void readFile(char *filename, char *buffer);

void deleteFile(char *filename);

void writeFile(char *name, char *buffer, int numberOfSectors);

void executeProgram(char* name, int segment);

void atoi(int myInt);

void terminate();

void handleInterrupt21(int ax, int bx, int cx, int dx);

int mod(int a, int b);

int div(int a, int b);

void sectorsBig();

void directoryList();



#define TRUE 1
#define FALSE 0
#define MAX_FILE_BYTES 13312

void main()
{
	char shell[6];
	char file[MAX_FILE_BYTES];
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

void printChar(char c) {
	char str[2];
	str[0] = c;
	str[1] = '\0';
	printString(str);
}

void readString(char *buffer){
	int i = 0;
	char lastCharRead;
	do {
		lastCharRead = interrupt(0x16, 0, 0, 0, 0);
		if(lastCharRead != 0x8) {
			buffer[i] = lastCharRead;
			i = i + 1;
			printChar(lastCharRead);
		} else if(i > 0) {
			i = i - 1;
			printChar(lastCharRead);
			printChar(' ');
			printChar(lastCharRead);
		}
	} while(lastCharRead != 0xd);
	buffer[i] = 0xa;
	printChar(0xa);
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

void writeSector(char *buffer, int sector) {
	int ah = 3;
	int al = 1;
	int ch = div(sector, 36);
	int cl = mod(sector, 18) + 1;
	int dh = mod(div(sector, 18), 2);
	int ax = (ah * 256) + al;
	int cx = (ch * 256) + cl;
	int dx = (dh * 256) + 0;
	interrupt(0x13, ax, buffer, cx, dx);
}

void readFile(char *filename, char *buffer, int *sectorsRead) {
	char dirList[512];
	int i = 0;
	char done[3];
	int j = 0;
	int fileFound = FALSE;
	readSector(dirList, 2);
	for(i = 0; i < 16; i++) {
		for (j = 0; j < 6; j++)	{
			if (dirList[(32*i)+j]==filename[j]) {
				fileFound = TRUE;
				if (filename[j] == '\0') {
					break;
				}
			}
			else {
				fileFound = FALSE;
				break;
			}
		}
		if (fileFound == TRUE) {
			for (j = 6; j < 32; j++) {
				if (dirList[(32*i)+j] == '\0') {		
					*sectorsRead = j - 6;
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

void deleteFile(char *filename) {
	char dirList[512];
	char map[512];
	int i = 0;
	int j = 0;
	int fileFound = FALSE;
	readSector(dirList, 2);
	readSector(map, 1);
	for(i = 0; i < 16; i++) {
		for (j = 0; j < 6; j++)	{
			if (dirList[(32*i)+j]==filename[j]) {
				fileFound = TRUE;
				if (filename[j] == '\0') {
					break;
				}
			}
			else {
				fileFound = FALSE;
				break;
			}
		}
		if (fileFound == TRUE) {
			dirList[32*i] = '\0';
			for (j = 6; j < 32; j++) {
				if (dirList[(32*i)+j] == '\0')
				{
					break;
				}
				map[dirList[(32*i) + j]] = '\0';
			}
			writeSector(dirList, 2);
			writeSector(map, 1);
			break;
		}
	}
	if (fileFound == FALSE) {
		terminate();
	}
}

void writeFile(char *name, char *buffer, int numberOfSectors) {
	/* Buffer should be the size of MAX_FILE_BYTES */
	char dirList[512];
	char map[512];
	int i = 0;
	int j = 0;
	int k = 0;	
	int fileFound = FALSE;
	readSector(dirList, 2);
	readSector(map, 1);
	for(i = 0; i < 16; i++) {
		for (j = 0; j < 6; j++)	{
			if (dirList[(32*i)]=='\0') {
				fileFound = TRUE;
			}
			else {
				fileFound = FALSE;
				break;
			}
		}
		if (fileFound == TRUE) {
			for (j = 0; j < 6; j++)	{
				dirList[(32*i)+j] = name[j];
				if (name[j] == '\0')
				{
					break;
				}
			}
			for (; j < 6; j++)	{
				dirList[(32*i)+j] = '\0';
			}
			for (j = 6; j < 32; j++) {
				if(j - 6 >= numberOfSectors) {
					break;
				}
				for (k = 0; k < 512; k++)
				{
					if (map[k] == '\0')
					{
						break;
					}
				}
				map[k] = 0xFF;
				dirList[(32*i)+j] = k;
				writeSector(buffer + (512 * (j - 6)), k);
			}
			for (; j < 32; j++) {
				dirList[(32*i)+j] = '\0';
			}
			writeSector(dirList, 2);
			writeSector(map, 1);
			break;
		}
	}
	if (fileFound == FALSE) {
		/* Error string no room */
		terminate();
	}
}

void executeProgram(char *name, int segment) {
	char buffer[MAX_FILE_BYTES];
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

void writeTextFile(char *name, char* buffer, int numberOfSectors){
	return;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else if (ax == 3) {
		readFile(bx, cx, dx);
	} else if (ax == 4) {
		executeProgram(bx, cx);
	} else if (ax == 5) {
		terminate();
	} else if (ax == 6) {
		writeSector(bx, cx);
	} else if (ax == 7) {
		deleteFile(bx);
	} else if (ax == 8) {
		writeFile(bx, cx, dx);
	} else if (ax == 9) {
		directoryList();
	} else {
		printString("404\0");
	}
}


void directoryList(){
	int i;
	int j;
	char dirList[512];
	char character;
	
	readSector(dirList, 2);
	for(i = 0; i < 16; i++) {
		if(dirList[(32*i)] != '\0'){
			for (j = 0; j < 6; j++)	{
				character = dirList[(32*i+j)];
				printChar(character);
			}
			printChar(' ');
			for(j = 6; j < 32; j++){
				if(dirList[(32*i+j)] == '\0') {
					atoi(j - 6);
					sectorsBig();
					break;
				}
			}
			printChar('\r');
			printChar('\n');
		}
	}

}

void sectorsBig(){
	char returnString[13];
	returnString[0] = ' ';
	returnString[1] = 's';
	returnString[2] = 'e';
	returnString[3] = 'c';
	returnString[4] = 't';
	returnString[5] = 'o';
	returnString[6] = 'r';
	returnString[7] = 's';
	returnString[8] = ' ';
	returnString[9] = 'b';
	returnString[10] = 'i';
	returnString[11] = 'g';
	returnString[12] = '\0';
	printString(returnString);
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

void atoi(int myInt){
    char myString[100];
    char reversedString[100];
    int index = 0;
    int moda = 0;
    int count = 0;
    int i;
    while(myInt != 0){
        moda = mod(myInt,10);
        myInt = (myInt-moda)/10;
        if(moda == 0){
            myString[count] = '0';
        }else if(moda == 1){
            myString[count] = '1';
        }else if(moda == 2){
            myString[count] = '2';
        }else if(moda == 3){
            myString[count] = '3';
        }else if(moda == 4){
            myString[count] = '4';
        }else if(moda == 5){
            myString[count] = '5';
        }else if(moda == 6){
            myString[count] = '6';
        }else if(moda==7){
            myString[count] = '7';
        }else if(moda==8){
            myString[count] = '8';
        }else if(moda == 9){
            myString[count] = '9';
        }
        count++;
    }
    count--;
    for(i = count;i>=0;i--){
        reversedString[index] = myString[i];
        index = index+1;
    }
    reversedString[index] = ' ';
    reversedString[index + 1] = '\0';
    printString(reversedString);
}

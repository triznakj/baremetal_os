#include <stdio.h>
#include <stdlib.h>

void parseCommand(char* input, char args[][10]);


int main(int argc, char* argv[]){
	char argument[10][10];
	char *input;
	input = "type message\r\n\0";
	parseCommand(input, argument);
	printf("%s\n",argument[0]);
	printf("%s\n",argument[1]);
	return 0;
}


void parseCommand(char *input, char args[][10]){
	

}

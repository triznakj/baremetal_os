#include <stdio.h>

int main(int argc,char *argv[]){
	char myString[100];
	int myInt = 12348765;
	int mod = 0;
	int count = 0;
	int i;
	while(myInt != 0){
		mod = myInt %10;
		myInt = (myInt-mod)/10;
		if(mod == 0){
			myString[count] = '0';
		}else if(mod == 1){
			myString[count] = '1';
		}else if(mod == 2){
			myString[count] = '2';
		}else if(mod == 3){
			myString[count] = '3';
		}else if(mod == 4){
			myString[count] = '4';
		}else if(mod == 5){
			myString[count] = '5';
		}else if(mod == 6){
			myString[count] = '6';
		}else if(mod==7){
			myString[count] = '7';
		}else if(mod==8){
			myString[count] = '8';
		}else if(mod == 9){
			myString[count] = '9';
		}
		count++;
	}
	count--;
	for(i = count;i>=0;i--){
		printf("%c",myString[i]);
	}
	printf("\n");
	return 0;
}

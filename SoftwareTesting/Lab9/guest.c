#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *sec = "???????";

void error() {
	printf("error\n");
	exit(0);
}

void success() {
	printf("success\n");
}

int main(int argc, char *argv[]){
  char* mySecret = "???????";
  int i;
  if(argc != 2) {
	printf("give somethongs\n");
	exit(0);
  }

  for(i = 0 ; i < 28 ; i ++) {
	if( sec[i] != ( (argv[1][i]  ) ^ (mySecret[(i + 2) % strlen(mySecret)] ^ 0x30 ))) {
		error();
	}
  }
  success();
  return 0;
}


#include <stdio.h>

void reverseString(void)
{
    char c;
    if((c = getchar()) != '\n'){
	   if(c == EOF)
		  return; 
	   reverseString();
	   printf("%c", c); 
    }
    return;
}

int main(int argc, char* argv[]){
    printf("Enter your text:\n");
    char c;
    while(1){
    	if((c = getchar()) == EOF)
		return 0;
    	reverseString();
	printf("%c", c);
    	printf("\n");
    }
}

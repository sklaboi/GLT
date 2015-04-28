#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include "flagi.h"
#include "generator.h"
*/

int main(int argc, char* argv[]){
	int n = -2;
	int s = -2;
	int a = -2;
	FILE *in[100];
	FILE *out;
	FILE *open;
	char nazwa[101][100];
	
	int ls = 0;
	int la = 0;
	int ln = 0;
	int lin = 0;
	int lout = -4;
	int lopen = 0;
	int index = 0;
	int status_in = 0;
	int i;
	int j;
	
	
	for(i=0;i<argc;i++){
		if(strcmp(argv[i], "-n") == 0 && ln !=1 && lopen == 0){
			ln = 1;
			n = atoi(argv[++i]);
			printf("Wykryto flage -n, ustawiono rzadn-gramu %d\n", n); 	
		}
		else if(strcmp(argv[i], "-a") == 0 && ln !=1 && la != 1){
			la = 1;
			a = atoi(argv[++i]);
			printf("Wykryto flage -a, ustawiono akapitow %d\n", a);
		}
		else if(strcmp(argv[i], "-s") == 0 && ls != 1){
                        a = atoi(argv[++i]);
                        printf("Wykryto flage -a, ustawiono akapitow %d\n", a);
                }
		else if(strcmp(argv[i], "-out") == 0 && lout != 1) {
			lout = spr_out(argv[i+1]);
			if (lout == -3)
				lout = i+1;
				
		}
		else if(strcmp(argv[i], "-open") == 0 && lopen != 1 && ln == 0 && lin == 0) {
			lopen == spr_open(argv[i+1]);
		}
		else if(strcmp(argv[i], "-in") == 0 && lin >-1){
			index = i + 1;

			
		}

	}
		
	printf("Program skonczyl dzialanie %d\n",n);
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXARRSIZE 30000
#define MAXCODESIZE 65536
#define PRINTSIZE 32
short int arr[MAXARRSIZE], ptr = 0;
int stack[MAXCODESIZE], sp = 0, i = 0;
int targets[MAXCODESIZE]; // save locations of matching brackets
char code[MAXCODESIZE]; int codelen, cp = 0;
FILE *fp;

int main(int argc, char** argv)
{
	if (argc > 2) fprintf(stderr, "too many arguments."), exit(1);
	if (argc < 2) fprintf(stderr, "bfic needs a filename."), exit(1);

	if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening the file."), exit(1);
	codelen = fread(code, 1, MAXCODESIZE, fp);
	fclose(fp);

	memset(arr, 0, sizeof(arr));

	// first round parse for loop locations
	for (cp = 0; cp < codelen; ++cp) {
		if (code[cp] == '[') stack[sp++] = cp; // push location of '[' onto stack
		if (code[cp] == ']') {
			if (sp == 0)
				fprintf(stderr, "Unmatched '[' at byte %d", cp), exit(1);
			else {
				--sp; // pop '[' from stack
				targets[cp] = stack[sp]; // save ']' 
				targets[stack[sp]] = cp;
			}
		}
	}

	if (sp > 0)
		fprintf(stderr, "Unmatched '[' at byte %d", stack[--sp]), exit(1);

	// eval
	for (cp = 0; cp < codelen; ++cp) {
		switch(code[cp]) {
			case '+': arr[ptr]++; break;
			case '-': arr[ptr]--; break;
			case '>': ptr++; break;
			case '<': ptr--; break;
			case '[': if (!arr[ptr]) cp = targets[cp]; break;
			case ']': if(arr[ptr]) cp = targets[cp]; break;
			case '.': putchar(arr[ptr]); fflush(stdout); break;
			case ',': printf("\n> "); scanf("%d", &ptr); break;
			case ' ': case '\t': case '\r': case '\n': /* skip */ break;
			case 'a'...'z': case 'A'...'Z': /* skip */ break;
			case '#': for(i=0;i<PRINTSIZE;++i) printf("%2d", (signed char)arr[i]); break;
		}
	}
	return 0;    
}

// Alexandra Montgomery
// y86emul.h


#ifndef Y86EMUL_H_
#define Y86EMUL_H_
#include <math.h>


typedef enum {
	ADR,
	AOK,
	HLT,
	INS
} checker;

union converter {
	char byte[4];
	int int1;
};

char * append (char * str, char c);

int htd(char * num);

void printmem (int size);

void printcheck ();

void getargs(unsigned char * arg1, unsigned char * arg2);

char * hextobin(char c) ;

void executeprogram();

int bintodec(char * num);

char * createdup (char * str);

int gettwobytes(char * str, int position);

#endif

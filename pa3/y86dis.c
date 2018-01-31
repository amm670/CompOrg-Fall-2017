// Alexandra Montgomery
// PA3 y86dis.c

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "y86dis.h"

int comp;
const char *rgstr[32] = {"%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"};
int main (int argc, char ** argv)
{
	if (argc <= 1)
	{
		printf("Error: Not enough input arguements.\n");
		return 0;
	}
	char help[] = "-h\0";
	char * input = (char *)malloc(sizeof(argv[1]));
	strcpy(input, argv[1]);
	if (strlen(input) < 5)
	{
		printf("Error: Invalid input file: %s\n", input);
		free(input);
		return 0;
	}
	if (strcmp(input, help) == 0)
	{
		printf("Help Information: This emulator running Y86 related programs.\n");
		free(input);
		return 0;
	}
	int i = 0;
	int j = 0;
	for (; input[i] != '\0'; i++)
	{
		if (input[i] == '.')
		{
			break;
		}
	}
	char filetype[] = ".y86.txt";
	char * temp = &input[i];

	if (strcmp(temp, filetype) != 0)
	{
		printf("Error: Wrong file type: %s\n", temp);
		return 0;
	}
	char mode[] = "r\0";
	FILE * inputfile = fopen(input, mode);
	if (inputfile == NULL)
	{
		printf("Error: File not found: %s\n", input);
		return 0;
	}
	char c;
	char * prog = (char *) calloc(1, sizeof(char));
	prog[0] = '\0';
	do
	{
		c = fgetc(inputfile);
		if (feof(inputfile))
		{
			break;
		}
		prog = append(prog, c);
	} while(1);
	fclose(inputfile);
	char * dup = createdup(prog);
	char * token = strtok(dup,"\n\t\r");
	char * instructions;
	char * compstart;
	int count = 0;
	do
	{
		if (strcmp(token, ".text") == 0)
		{
			if (count == 0)
			{
				count++;
			}
			else
			{
				printf("Error:\n You can only use one .text directive.\n");
				return 0;
			}

			compstart = createdup(strtok (NULL, "\n\t\r"));
			instructions = createdup(strtok(NULL, "\n\t\r"));
		}
		//iterator until end of tokens
		token = strtok(NULL, "\n\t\r");
		if (token == NULL)
		{
			break;
		}
	} while (1);
	free(dup);
	comp = htd(compstart);
	int is = strlen(instructions);
	unsigned char memoryloc[is/2];
	i = j = 0;
	while (j < is)
	{
		memoryloc[i] = gettwobytes(instructions, j);
		i++;
		j += 2;
	}
	union converter converter2;
	int arg1, arg2, value;
	i = j = 0;
	while (i < is/2)
	{
		switch (memoryloc[i])
		{
			case 0x00:
				printf("0x%08x\tnop\n", comp);
				i++;
				j = 1;
			break;
			case 0x10:
				printf("0x%08x\thalt\n", comp);
				i++;
				j = 1;
			break;
			case 0x20:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\trrmovl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x30:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				printf("0x%08x\tirmovl\t$%0x\t%s\n", comp, converter2.int1, rgstr[arg2]);
				j = 6;
				i += 6;
				break;
			break;
			case 0x40:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\trmmovl\t%s\t%d(%s)\n", comp, rgstr[arg1], value, rgstr[arg2]);
				j = 6;
				i += 6;
			break;
			case 0x50:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\tmrmovl\t%d(%s)\t%s\n", comp, value, rgstr[arg2], rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			case 0x60:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\taddl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x61:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\tsubl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x62:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\tandl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x63:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\txorl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x64:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\tmull\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x65:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				printf("0x%08x\tcmpl\t%s\t%s\n", comp, rgstr[arg1], rgstr[arg2]);
				j = 2;
				i += 2;
			break;
			case 0x70:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjmp\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x71:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjle\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x72:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjl\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x73:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tje\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x74:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjne\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x75:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjge\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x76:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tjg\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x80:
				converter2.byte[0] = memoryloc[i + 1];
				converter2.byte[1] = memoryloc[i + 2];
				converter2.byte[2] = memoryloc[i + 3];
				converter2.byte[3] = memoryloc[i + 4];
				value = converter2.int1;
				printf("0x%08x\tcall\t$0x%x\n", comp, value);
				j = 5;
				i += 5;
			break;
			case 0x90:
				printf("0x%08x\tret\n", comp);
				j = 1;
				i += 1;
			break;
			case 0xA0:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				printf("0x%08x\tpushl\t%s\n", comp, rgstr[arg1]);
				j = 2;
				i += 2;
			break;
			case 0xB0:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				printf("0x%08x\tpopl\t%s\n", comp, rgstr[arg1]);
				j = 2;
				i += 2;
			break;
			case 0xC0:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\treadb\t%d(%s)\n", comp, value, rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			case 0xC1:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\treadl\t%d(%s)\n", comp, value, rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			case 0xD0:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\twriteb\t%d(%s)\n", comp, value, rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			case 0xD1:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\twritel\t%d(%s)\n", comp, value, rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			case 0xE0:
				arg1 = (memoryloc[i + 1] & 0xf0) >> 4;
				arg2 = (memoryloc[i + 1] & 0x0f);
				converter2.byte[0] = memoryloc[i + 2];
				converter2.byte[1] = memoryloc[i + 3];
				converter2.byte[2] = memoryloc[i + 4];
				converter2.byte[3] = memoryloc[i + 5];
				value = converter2.int1;
				printf("0x%08x\tmovsbl\t%d(%s)\t%s\n", comp, value, rgstr[arg2], rgstr[arg1]);
				j = 6;
				i += 6;
			break;
			default:
			break;
		}
		comp += j;
	}
	free(input);
	free(prog);
	return 0;
}

char * append (char * str, char c)
{
//append to string
	int len = strlen(str) + 2;
	char * ret = (char *)calloc(len, sizeof(char));
	strcpy(ret, str);
	free(str);
	ret[len-1] = '\0';
	ret[len-2] = c;
	return ret;
}

int htd(char * num)
{
//hex to decimal conversion
	int size = strlen(num);
	char * binstr = (char *) malloc((4*size + 1)*sizeof(char));
	int i;
	for (i = 0; i < 4*size + 1; i++)
	{
		binstr[i] = '\0';
	}
	for (i = 0; i < size; i++)
	{
		strcat(binstr, hextobin(num[i]));
	}
	int ret = bintodec(binstr);
	free(binstr);
	return ret;
}

char * hextobin(char c)
{
	//hex to binary conversion
	switch(c)
	{
		case '0':
		return "0000";
		break;

		case '1':
		return "0001";
		break;

		case '2':
		return "0010";
		break;

		case '3':
		return "0011";
		break;

		case '4':
		return "0100";
		break;

		case '5':
		return "0101";
		break;

		case '6':
		return "0110";
		break;

		case '7':
		return "0111";
		break;

		case '8':
		return "1000";
		break;

		case '9':
		return "1001";
		break;

		case 'A':
		case 'a':
		return "1010";
		break;

		case 'B':
		case 'b':
		return "1011";
		break;

		case 'C':
		case 'c':
		return "1100";
		break;

		case 'D':
		case 'd':
		return "1101";
		break;

		case 'E':
		case 'e':
		return "1110";
		break;

		case 'F':
		case 'f':
		return "1111";
		break;

		case '\0':
		break;

		default:
		printf("Not a hex value: %c \n", c);
		break;
	}
	return "";
}

int bintodec(char * num)
{
//Binary to decimal conversion
	int power = strlen(num) - 1;
	int i, ret = 0;
	for (i = 0; num[i] != '\0'; i++)
	{
		int temp = num[i] - '0';
		ret += temp * (int)pow(2, power);
		power--;
	}
	return ret;
}



char * createdup (char * str)
//to create duplicates
{
	char * ret = (char *) malloc((strlen(str) + 1) * sizeof(char));
	strcpy(ret, str);
	return ret;
}



int gettwobytes(char * str, int position)
{
	char * twobytes = (char *) malloc(3*sizeof(char));
	twobytes[2] = '\0';
	twobytes[0] = str[position];
	twobytes[1] = str[position + 1];
	int ret = htd(twobytes);
	free(twobytes);
	return ret;
}

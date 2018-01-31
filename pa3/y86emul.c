// Alexandra Montgomery
// y86emul.c

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "y86emul.h"

unsigned char * memoryloc;
int counter, memsize, OF, ZF, SF;
int rgstr[32];
checker check = AOK;

int main (int argc, char ** argv)
{
	if (argc <= 1)
	{
		printf("Error: Not enough input arguements.\n");
		//complain if not enough args
		return 0;
	}
	//allocate memory and copy arg1 contents to input
	char * input = (char *)malloc(sizeof(argv[1]));
	strcpy(input, argv[1]);
	if (strlen(input) < 5)
	{
		printf("Error: Invalid input file: %s\n", input);
		//complain if over string is too long
		free(input);
		return 0;
	}
	char help[] = "-h\0";
	if (strcmp(input, help) == 0)
		//provide help
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
	//make sure we are getting a y86 text file
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
	char * memory;
	int count = 0;
	do
	{
		if (strcmp(token, ".size") == 0)
		{
			if (count == 0)
			{
				count++;
			}
			else
			{
				printf("Error:\n You can only use one .size directive.\n");
				return 0;
			}
			memory = strtok(NULL, "\n\t\r");
		}
		token = strtok(NULL, "\n\t\r");

		if (token == NULL)
		{
			break;
		}
	} while (1);

	if (count == 0)
	{
		printf("Error:\n You must use one .size directive.\n");
		return 0;
	}
	int size = htd(memory);
	memsize = size;
	memoryloc = (unsigned char *) malloc((size) * sizeof(unsigned char));
	counter = -1;

	for (i = 0; i <size; i++)
	{
		memoryloc[i] = 0;
	}
	//free our memory
	free(dup);
	dup = createdup(prog);
	token = strtok(dup, "\n\t\r");
	//directive arg
	char * arg;
	char * address;
	int ai = 0;
	do
	{
	//check which directive our token holds
		if (strcmp(token, ".size") == 0)
		{
			token = strtok(NULL, "\n\t\r");
		}
		//if .text
		else if (strcmp(token, ".text") == 0)
		{
			address = createdup(strtok(NULL, "\n\t\r"));
			arg = createdup(strtok(NULL, "\n\t\r"));
			ai = htd(address);

			if (counter == -1)
			{
				counter = ai;
			}
			else if (counter != -1)
			{
				printf("\nError: Can't use more then one .text directive\n");
				return 0;
			}
			j = 0;
			while (j < strlen(arg))
			{
				memoryloc[ai] = (unsigned char) gettwobytes(arg, j);
				ai++;
				j += 2;
			}
			free(address);
			free(arg);
		}
		//if .byte
		else if (strcmp(token, ".byte") == 0)
		{
			address = createdup(strtok(NULL, "\n\t\r"));
			arg = createdup(strtok(NULL, "\n\t\r"));
			memoryloc[htd(address)] = (unsigned char) htd(arg);
			free(address);
			free(arg);
		}
		//if .long
		else if (strcmp(token, ".long") == 0)
		{
			address = createdup(strtok(NULL, "\n\t\r"));
			arg = createdup(strtok(NULL, "\n\t\r"));
			union converter Converter;
			Converter.int1 = atoi(arg);
			for(i = 0; i < 4; i++)
			{
				memoryloc[i+htd(address)] = Converter.byte[i];
			}
			free(address);
			free(arg);
		}
		//if .string
		else if (strcmp(token, ".string") == 0)
		{
			address = createdup(strtok(NULL, "\n\t\r"));
			arg = createdup(strtok(NULL, "\n\t\r"));
			int len = strlen(arg);
			i = htd(address);
			for(j = 1; j < len - 1; j++)
			{
				memoryloc[i] = (unsigned char)arg[j];
				i++;
			}
			free(arg);
			free(address);
		}
		else if (token[0] == '.')
		{
			check = INS;
		}
		token = strtok(NULL, "\n\t");
		if (token == NULL)
		{
			break;
		}
	} while (1);
	free(dup);
	if (check == INS)
	{
		//complain because of unknown directive
		printf("Error: An unreadable directive was detected: %s\n", token);
		return 0;
	}
	executeprogram();
	printcheck();
	free(memoryloc);
	free(input);
	free(prog);
	return 0;
}

void executeprogram()
{
	unsigned char arg1;
	unsigned char arg2;
	int offset, a, b;
	union converter Converter;
	check = AOK;
	int badscan;
	char inputchar;
	int inputword;
	//set all to 0
	rgstr[7] = rgstr[6] = rgstr[5] = rgstr[4] = rgstr[3] = rgstr[2] = rgstr[1] = rgstr[0] = 0;
	OF = ZF = SF = 0;
	do
	{
		switch (memoryloc[counter])
		{
			case 0x00:
				counter++;
			case 0x10:
				check = HLT;
                    break;
			case 0x20:
				getargs(&arg1, &arg2);
				rgstr[arg2] = rgstr[arg1];
				counter += 2;
			break;
			case 0x30:
				getargs(&arg1, &arg2);
				if (arg1 < 0x08)
				{
					check = ADR;
					printf("IRMOVL error at location: %x\n", counter);
					break;
				}
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				rgstr[arg2] = offset;
				counter += 6;
			break;
			case 0x40:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				Converter.int1 = rgstr[arg1];
				if ((offset + rgstr[arg2] + 3) > memsize)
				{
					check = ADR;
					printf("RMMOVL error at location: %x\n", counter);
				}
				memoryloc[offset + rgstr[arg2] + 0] = Converter.byte[0];
				memoryloc[offset + rgstr[arg2] + 1] = Converter.byte[1];
				memoryloc[offset + rgstr[arg2] + 2] = Converter.byte[2];
				memoryloc[offset + rgstr[arg2] + 3] = Converter.byte[3];
				counter += 6;
			break;
			case 0x50:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				if ((offset + rgstr[arg2] + 3) >=memsize)
				{
					check = ADR;
					printf("MRMOVL error at location: %x\n", counter);
				}
				Converter.byte[0] = memoryloc[offset + rgstr[arg2] + 0];
				Converter.byte[1] = memoryloc[offset + rgstr[arg2] + 1];
				Converter.byte[2] = memoryloc[offset + rgstr[arg2] + 2];
				Converter.byte[3] = memoryloc[offset + rgstr[arg2] + 3];
				rgstr[arg1] = Converter.int1;
				counter += 6;
			break;
			case 0x60:
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = a + b;
				if (offset == 0)
				{
					ZF = 1;
				}
				else
				{
	                ZF = 0;
				}
				if (offset < 0)
				{
					SF = 1;
				}
				else
				{
					SF = 0;
				}
				if ((offset > 0 && a < 0 && b < 0) || (offset < 0 && a > 0 && b > 0))
				{
					OF = 1;
				}
				else
				{
 					OF = 0;
  				}
				rgstr[arg2] = offset;
				counter += 2;
			break;
			case 0x61:
				ZF = 0;
				SF = 0;
				OF = 0;
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = b - a;
				if (offset == 0)
				{
					ZF = 1;
				}
				if (offset < 0)
				{
					SF = 1;
				}
				if ((offset > 0 && a > 0 && b < 0) || (offset < 0 && a < 0 && b > 0))
				{
					OF = 1;
				}
				rgstr[arg2] = offset;
				counter += 2;
			break;
			case 0x62:
				SF = 0;
				ZF = 0;
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = a & b;
				rgstr[arg2] = offset;
				if (offset == 0)
				{
					ZF = 1;
				}
				if (offset < 0)
				{
					SF = 1;
				}
				counter += 2;
			break;
			case 0x63:
				ZF = 0;
				SF = 0;
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = a ^ b;
				rgstr[arg2] = offset;
				if (offset == 0)
				{
					ZF = 1;
				}
				if (offset < 0)
				{
					SF = 1;
				}

				counter += 2;
			break;
			case 0x64:
				ZF = 0;
				SF = 0;
				OF = 0;
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = a * b;
				if (offset == 0)
				{
					ZF = 1;
				}
				if (offset < 0)
				{
					SF = 1;
				}
				if ((offset < 0 && a < 0 && b < 0) ||
					(offset < 0 && a > 0 && b > 0) ||
					(offset > 0 && a < 0 && b > 0) ||
					(offset > 0 && a > 0 && b < 0))
				{
					OF = 1;
				}
				rgstr[arg2] = offset;
				counter += 2;
			break;
			// 65 CMPL
			case 0x65:
				ZF = 0;
				SF = 0;
				OF = 0;
				getargs(&arg1, &arg2);
				a = rgstr[arg1];
				b = rgstr[arg2];
				offset = b - a;
				if (offset == 0)
				{
					ZF = 1;
				}
				if (offset < 0)
				{
					SF = 1;
				}
				if ((offset > 0 && a > 0 && b < 0) || (offset < 0 && a < 0 && b > 0))
				{
					OF = 1;
				}
				counter += 2;
			break;
			case 0x70:
				//Jump
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				counter = offset;
			break;
			//JLE
			case 0x71:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (ZF == 1 || (SF ^ OF))
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}
			break;
			//JL
			case 0x72:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (ZF == 0 && (SF ^ OF))
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}

			break;
			//JE
			case 0x73:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (ZF == 1)
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}
			break;
			//JNE
			case 0x74:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (ZF == 0)
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}
			break;
			//JGE
			case 0x75:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (!(ZF == 0 && (SF ^ OF)))
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}
			break;
			//JG
			case 0x76:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				if (!(ZF == 1 || (SF ^ OF)))
				{
					counter = offset;
				}
				else
				{
					counter += 5;
				}
			break;
			case 0x80:
				Converter.byte[0] = memoryloc[counter + 1];
				Converter.byte[1] = memoryloc[counter + 2];
				Converter.byte[2] = memoryloc[counter + 3];
				Converter.byte[3] = memoryloc[counter + 4];
				offset = Converter.int1;
				rgstr[4] -= 4;
				Converter.int1 = counter + 5;
				memoryloc[rgstr[4] + 0] = Converter.byte[0];
				memoryloc[rgstr[4] + 1] = Converter.byte[1];
				memoryloc[rgstr[4] + 2] = Converter.byte[2];
				memoryloc[rgstr[4] + 3] = Converter.byte[3];

				counter = offset;
			break;
			case 0x90:
				Converter.byte[0] = memoryloc[rgstr[4] + 0];
				Converter.byte[1] = memoryloc[rgstr[4] + 1];
				Converter.byte[2] = memoryloc[rgstr[4] + 2];
				Converter.byte[3] = memoryloc[rgstr[4] + 3];
				counter = Converter.int1;
				rgstr[4] += 4;
			break;
			case 0xA0:
				getargs(&arg1, &arg2);
				rgstr[4] -= 4;
				Converter.int1 = rgstr[arg1];
				memoryloc[rgstr[4] + 0] = Converter.byte[0];
				memoryloc[rgstr[4] + 1] = Converter.byte[1];
				memoryloc[rgstr[4] + 2] = Converter.byte[2];
				memoryloc[rgstr[4] + 3] = Converter.byte[3];

				counter += 2;
			break;
			case 0xB0:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[rgstr[4] + 0];
				Converter.byte[1] = memoryloc[rgstr[4] + 1];
				Converter.byte[2] = memoryloc[rgstr[4] + 2];
				Converter.byte[3] = memoryloc[rgstr[4] + 3];
				offset = Converter.int1;
				rgstr[arg1] = offset;
				rgstr[4] += 4;
				counter += 2;
			break;
			case 0xC0:
				ZF = 0;
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				if (1 > scanf("%c", &inputchar))
				{
					ZF = 1;
				}
				memoryloc[rgstr[arg1] + offset] = inputchar;

				counter += 6;
			break;
			case 0xC1:
				ZF = 0;
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				badscan = scanf("%d", &inputword);
				if (badscan < 1)
				{
					ZF = 1;
				}

				Converter.int1 = inputword;
				memoryloc[rgstr[arg1]+ offset + 0] = Converter.byte[0];
				memoryloc[rgstr[arg1]+ offset + 1] = Converter.byte[1];
				memoryloc[rgstr[arg1]+ offset + 2] = Converter.byte[2];
				memoryloc[rgstr[arg1]+ offset + 3] = Converter.byte[3];
				counter += 6;
			break;
			case 0xD0:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				printf("%c", (char)memoryloc[rgstr[arg1] + offset]);
				counter += 6;
			break;
			case 0xD1:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				Converter.byte[0] = memoryloc[offset + rgstr[arg1] + 0];
				Converter.byte[1] = memoryloc[offset + rgstr[arg1] + 1];
				Converter.byte[2] = memoryloc[offset + rgstr[arg1] + 2];
				Converter.byte[3] = memoryloc[offset + rgstr[arg1] + 3];
				a = Converter.int1;
				printf("%d", a);
				counter += 6;
			break;
			case 0xE0:
				getargs(&arg1, &arg2);
				Converter.byte[0] = memoryloc[counter + 2];
				Converter.byte[1] = memoryloc[counter + 3];
				Converter.byte[2] = memoryloc[counter + 4];
				Converter.byte[3] = memoryloc[counter + 5];
				offset = Converter.int1;
				Converter.int1 = rgstr[arg2];
				inputchar = Converter.byte[3];
				if ((inputchar >> 7 & 1) == 0)
				{
					Converter.byte[0] = inputchar;
					Converter.byte[1] = 0x00;
					Converter.byte[2] = 0x00;
					Converter.byte[3] = 0x00;
				}
				else
				{
					Converter.byte[0] = inputchar;
					Converter.byte[1] = 0xff;
					Converter.byte[2] = 0xff;
					Converter.byte[3] = 0xff;
				}
				Converter.byte[0] = memoryloc[rgstr[arg2]+ offset + 0];
				Converter.byte[1] = memoryloc[rgstr[arg2]+ offset + 1];
				Converter.byte[2] = memoryloc[rgstr[arg2]+ offset + 2];
				Converter.byte[3] = memoryloc[rgstr[arg2]+ offset + 3];
				rgstr[arg1] = Converter.int1;
				counter += 6;
			break;
			default:
				check = INS;
			break;
		}
		offset = 0;
		arg1 = arg2 = 0;
	}while (check == AOK);
}

char * append (char * str, char c)
{
	int len = strlen(str) + 2;
	char * ret = (char *)calloc(len, sizeof(char));
	strcpy(ret, str);
	free(str);
	ret[len-1] = '\0';
	ret[len-2] = c;
	return ret;
}
//Conversion for hex to decimal
int htd(char * num)
{
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
//different cases for converting hex to binary
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

		case 'a':
		case 'A':
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
		printf("Invalid hex character: %c \n", c);
		break;
	}
	return "";
}

int bintodec(char * num)
{
	//save our string length
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
	//call the hex converter to decimal
	int ret = htd(twobytes);
	free(twobytes);
	return ret;
}

void getargs(unsigned char * arg1, unsigned char * arg2)
{
	*arg1 = (memoryloc[counter + 1] & 0xf0) >> 4;
	*arg2 = (memoryloc[counter + 1] & 0x0f);
}

void printcheck ()
{
	switch (check)
	{
		case AOK:
			printf("Checking: No Errors, AOK.\n");
		break;

		case INS:
			printf("Checking: Error: INS.\n");
		break;

		case ADR:
			printf("Checking: Error: ADR.\n");
		break;

		case HLT:
			printf("Checking: Error: HLT.\n");
		break;
	}
}

//Alexandra Montgomery
// pa1 calc.c


#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define BIT_SYS 32

long long strength(int base, int exp);
long long BinToDec(char* binary);
void DecToBin(long long decimal);
long long OctToDec(long long oct);
void DecToOct(long long decimal);
void DecToHex(long long decimal);
long long OctToTwo(char *oct);
long long HexToTwo(char* hex);
void BinToHex(char* binary);
void BinToOct(char* binary);
void DecToBin_Ret(char **binary_out, long long decimal);
char BinHexGroup(char* bin_dig);
char BinOctGroup(char* bin_dig);
long long checker(char* input_number);
void printer(long long res_num, char* base_op);





int main(int argc, char *argv[])
{
	long long first, second, theNum;
	if (*argv[1] == '+')
	{
		first = checker(argv[2]);
		second = checker(argv[3]);
		theNum = first + second;
		printer(theNum, argv[4]);
	}
	else if (*argv[1] == '-')
	{
		first = checker(argv[2]);
		second = checker(argv[3]);
		theNum = first - second;
		printer(theNum, argv[4]);
	}
	else if (*argv[1] == '*')
	{
		first = checker(argv[2]);
		second = checker(argv[3]);
		theNum = first * second;
		printer(theNum, argv[4]);
	}
        else if (*argv[1] == '/')
	{
		first = checker(argv[2]);
		second = checker(argv[3]);
		theNum = first / second;
		printer(theNum, argv[4]);
	}

	else
	{
		printf("%s NOT SUPPORTED or INVALID!\n", argv[1]);
	}
	return 0;
}




//Recognizes the power
long long strength(int base, int exp)
{
	long long result = 1;
	while (exp != 0)
	{
		result *= base;
		--exp;
	}
	return result;
}








// Converting Binary to Decimal
long long BinToDec(char* binary)
{
    int num_bits = strlen(binary)-1;
    long long power = strength(2, num_bits-1);
    long long sum = 0;
    int i;

    if (num_bits == BIT_SYS)
    {
		for (i=1; i<=num_bits; ++i)
		{
			if ( i==1 && binary[i]!='0')
			{
				sum = power * -1;
			}
			else
			{
				sum = sum + (binary[i]-'0')*power;
			}
			power /= 2;
		}
    }
    else
    {
    	for (i=1; i<=num_bits; ++i)
    	{
    		sum = sum + (binary[i]-'0')*power;
    		power /= 2;
    	}
    }

    return sum;
}




//Decimal to binary
void DecToBin(long long decimal)
{
   if (decimal == 0)
   {
	   printf("b0");
   }
   else if (decimal == 1)
   {
	   printf("b1");
   }
   else
   {
	   int bin[BIT_SYS];
	   int i;
	   int j, h;
	   long long temp;
	   printf("b");

	   if (decimal < 0)
	   {
		  temp = decimal* -1;
		  for (i=0; temp>0; i++)
		  {
			 if ((temp%2)==0)
			 {
				bin[i] = 1;
			 }
			 else
			 {
				bin[i] = 0;
			 }
			 temp = temp/2;
		  }
		  for (j=i; j<BIT_SYS; j++)
		  {
			 bin[j] = 1;
		  }
		  for (h=0; h<BIT_SYS; h++)
		  {
			if (bin[h] == 1)
			{
				bin[h] = 0;
				continue;
			}
			else
			{
				bin[h] = 1;
				break;
			}
		  }
		  for (j=j-1; j >= 0; j--)
		  {
			  printf("%d", bin[j]);
		  }
	   }
	   else
	   {
		   temp = decimal;
		   for (i=0; temp>0; i++)
		   {
			 if ((temp%2)==0)
			 {
				bin[i] = 0;
			 }
			 else
			 {
				bin[i] = 1;
			 }
			 temp = temp/2;
		   }
		   for (i=i-1; i >= 0; i--)
		   {
			   printf("%d", bin[i]);
		   }
	   }
	   printf("\n");
   }
}




//Octal to Decimal Conversion
long long OctToDec(long long oct)
{
	long long temp = oct, dec = 0;
	int j = 0;

	while (temp != 0)
	{
		dec = dec + (strength(8, j++)*(temp%10));
		temp /= 10;
	}
	return dec;
}





//Conversion from Decimal to Octal
void DecToOct(long long decimal)
{
	printf("o");
        char *dec_binary;
        DecToBin_Ret(&dec_binary, decimal);
	BinToOct(dec_binary);
	printf("\n");
        free(dec_binary);
}

void DecToHex(long long decimal)
{
	printf("x");
        char *dec_binary;
        DecToBin_Ret(&dec_binary, decimal);
	BinToHex(dec_binary);
	printf("\n");
        free(dec_binary);
}






//Conversion from Octal
long long OctToTwo(char* oct)
{
	int value = 0;
	int i;

	for (i=1; i < strlen(oct); i++)
	{
		switch(oct[i])
		{
			case '0': value = (value<<3); break;
			case '1': value = (value<<3) + 1; break;
			case '2': value = (value<<3) + 2; break;
			case '3': value = (value<<3) + 3; break;
			case '4': value = (value<<3) + 4; break;
			case '5': value = (value<<3) + 5; break;
			case '6': value = (value<<3) + 6; break;
			case '7': value = (value<<3) + 7; break;
			default: printf("ERROR: Invalid input for an octal\n");
		}
	}
	return value;
}





//Conversion from hexadecimal
long long HexToTwo(char* hex)
{
	int value = 0;
	int i;

	for (i=1; i < strlen(hex); i++)
	{
		switch(hex[i])
		{
			case '0': value = (value<<4); break;
			case '1': value = (value<<4) + 1; break;
			case '2': value = (value<<4) + 2; break;
			case '3': value = (value<<4) + 3; break;
			case '4': value = (value<<4) + 4; break;
			case '5': value = (value<<4) + 5; break;
			case '6': value = (value<<4) + 6; break;
			case '7': value = (value<<4) + 7; break;
			case '8': value = (value<<4) + 8; break;
			case '9': value = (value<<4) + 9; break;
			case 'a': case 'A': value = (value<<4) + 10; break;
			case 'b': case 'B': value = (value<<4) + 11; break;
			case 'c': case 'C': value = (value<<4) + 12; break;
			case 'd': case 'D': value = (value<<4) + 13; break;
			case 'e': case 'E': value = (value<<4) + 14; break;
			case 'f': case 'F': value = (value<<4) + 15; break;
			default: printf("ERROR: Invalid input for a hexadecimal\n");
		}
	}
	return value;
}








//Compares
long long checker(char* input_number)
{
	long long dec;
	if (input_number[0]=='b')
	{
		dec = BinToDec(input_number);
	}
	else if (input_number[0]=='o')
	{
		dec = OctToTwo(input_number);
	}
	else if (input_number[0]=='x')
	{
		dec = HexToTwo(input_number);
	}
	else if (input_number[0]=='d')
	{
		char dec_num[(strlen(input_number)-1)];
		int i, k, j=0, l=0;
		long long sum = 0;
		for (i=1; i <= strlen(input_number); i++)
		{
			dec_num[j] = input_number[i];
			j++;
		}
		for (k=(strlen(input_number)-2); k>=0; k--)
		{
			sum += (dec_num[k] - '0')*strength(10,l);
			l++;
		}
		dec = sum;
	}
	else if (input_number[0]=='-')
	{
		if (input_number[1] != 'd')
		{
			printf("Only decimal base numbers can be negative!\n");
			exit(1);
		}
		else
		{
			char dec_num[(strlen(input_number)-2)];
			int i, k, j=0, l=0;
			long long sum = 0;
			for (i=2; i <= strlen(input_number); i++)
			{
				dec_num[j] = input_number[i];
				j++;
			}
			for (k=(strlen(input_number)-3); k>=0; k--)
			{
				sum += (dec_num[k] - '0')*strength(10,l);
				l++;
			}
			dec = sum*(-1);
		}
	}
	else
	{
		printf("The base for one of the input numbers is not supported!\n");
		exit(1);
	}
	return dec;
}





// returns through printing
void printer(long long res_num, char* base_op)
{
	if (*base_op == 'b')
	{
		DecToBin(res_num);
	}
	else if (*base_op == 'x')
	{
		DecToHex(res_num);
	}
	else if (*base_op == 'o')
	{
		DecToOct(res_num);
	}
	else if (*base_op == 'd')
	{
		if (res_num > 0)
		{
			printf("d%lld\n", res_num);
		}
                else
                {
                       printf("-d%lld\n", (-1*res_num));
                }
	}
	else
	{
		printf("The output base %c is not supported. Use either of b/x/o/d\n", *base_op);
	}
}






// Conversion from Decimal to Binary
void DecToBin_Ret(char **binary_out, long long decimal)
{
   if (decimal == 0)
   {
	   *binary_out = malloc(BIT_SYS+1);
           strcpy(*binary_out, "00000000000000000000000000000000");
   }
   else if (decimal == 1)
   {
	   *binary_out = malloc(BIT_SYS+1);
           strcpy(*binary_out, "00000000000000000000000000000001");
   }
   else
   {
	   char bin[BIT_SYS] = {'\0'};
	   char bin_rev[BIT_SYS+1] = {'\0'};
           *binary_out = malloc(BIT_SYS+1);
	   int i, j, h;
	   int k=0;
	   long long temp;

	   if (decimal < 0)
	   {
		  temp = decimal*(-1);
		  for (i=0; temp>0; i++)
		  {
			 if ((temp%2)==0)
			 {
				bin[i] = '1';
			 }
			 else
			 {
				bin[i] = '0';
			 }
			 temp = temp/2;
		  }
		  for (j=i; j<BIT_SYS; j++)
		  {
			 bin[j] = '1';
		  }
		  for (h=0; h<BIT_SYS; h++)
		  {
			if (bin[h] == '1')
			{
				bin[h] = '0';
				continue;
			}
			else
			{
				bin[h] = '1';
				break;
			}
		  }
		  for (j=j-1; j >= 0; j--)
		  {
			  bin_rev[k] = bin[j];
			  k++;
		  }
		  strcpy(*binary_out, bin_rev);
	   }
	   else
	   {
		   temp = decimal;
		   for (i=0; temp>0; i++)
		   {
			 if ((temp%2)==0)
			 {
				bin[i] = '0';
			 }
			 else
			 {
				bin[i] = '1';
			 }
			 temp = temp/2;
		   }
		   for (j=i; j<BIT_SYS; j++)
		   {
			 bin[j] = '0';
		   }
		   for (j=j-1; j >= 0; j--)
		   {
			  bin_rev[k] = bin[j];
			  k++;
		   }
		   strcpy(*binary_out, bin_rev);
	   }
   }
}




// Recognizes the hexadecimal characters in groups of four
char BinHexGroup(char* bin_dig)
{
	if (strcmp(bin_dig, "0000") == 0)
	{
		return '0';
	}
	else if (strcmp(bin_dig, "0001") == 0)
	{
		return '1';
	}
	else if (strcmp(bin_dig, "0010") == 0)
	{
		return '2';
	}
	else if (strcmp(bin_dig, "0011") == 0)
	{
		return '3';
	}
	else if (strcmp(bin_dig, "0100") == 0)
	{
		return '4';
	}
	else if (strcmp(bin_dig, "0101") == 0)
	{
		return '5';
	}
	else if (strcmp(bin_dig, "0110") == 0)
	{
		return '6';
	}
	else if (strcmp(bin_dig, "0111") == 0)
	{
		return '7';
	}
	else if (strcmp(bin_dig, "1000") == 0)
	{
		return '8';
	}
	else if (strcmp(bin_dig, "1001") == 0)
	{
		return '9';
	}
	else if (strcmp(bin_dig, "1010") == 0)
	{
		return 'A';
	}
	else if (strcmp(bin_dig, "1011") == 0)
	{
		return 'B';
	}
	else if (strcmp(bin_dig, "1100") == 0)
	{
		return 'C';
	}
	else if (strcmp(bin_dig, "1101") == 0)
	{
		return 'D';
	}
	else if (strcmp(bin_dig, "1110") == 0)
	{
		return 'E';
    }
	else if (strcmp(bin_dig, "1111") == 0)
	{
		return 'F';
	}
	else
	{
		printf("This group can only contain ones and zeros, somethings wrong. \n");
		return '!';
	}
}





// Find the Binary Conversion for Octal
char BinOctGroup(char* bin_dig)
{
	if (strcmp(bin_dig, "000") == 0)
	{
		return '0';
	}
	else if (strcmp(bin_dig, "001") == 0)
	{
		return '1';
	}
	else if (strcmp(bin_dig, "010") == 0)
	{
		return '2';
	}
	else if (strcmp(bin_dig, "011") == 0)
	{
		return '3';
	}
	else if (strcmp(bin_dig, "100") == 0)
	{
		return '4';
	}
	else if (strcmp(bin_dig, "101") == 0)
	{
		return '5';
	}
	else if (strcmp(bin_dig, "110") == 0)
	{
		return '6';
	}
	else if (strcmp(bin_dig, "111") == 0)
	{
		return '7';
	}
	else
	{
		printf("Somethings wrong, this section can only have ones and zeros!!\n");
		return '!';
	}
}




// Binary To Hex
void BinToHex(char* binary)
{
	int i;
	int count = 0;
	for (i=0; i < BIT_SYS; i += 4)
	{
		char temp[5] = {binary[i], binary[i+1], binary[i+2], binary[i+3], '\0'};
		char hex_bit = BinHexGroup(temp);
		if (hex_bit == '0' && count == 0)
		{
			continue;
		}
		else
		{
		printf("%c", hex_bit);
		count++;
		}
	}
}




// Binary to Octal
void BinToOct(char* binary)
{
	int i, len;
	char bin[67];
	if (BIT_SYS == 32)
	{
		strcpy(bin, "0000");
		strcat(bin, binary);
		len = 36;
	}
	else
	{
		strcpy(bin, "00000");
		strcat(bin, binary);
		len = 66;
	}
	int count = 0;
	if (binary[0] == '0')
	{
		//printf("YES");
		for (i=0; i < len; i += 3)
		{
			char temp[4] = {bin[i], bin[i+1], bin[i+2], '\0'};
			//printf("%s\n", temp);
			char oct_bit = BinOctGroup(temp);
			if (oct_bit == '0' && count == 0)
			{
				continue;
			}
			else
			{
			printf("%c", oct_bit);
			count++;
			}
		}
	}
	else
	{
		for (i=0; i < len; i += 3)
		{
			char temp[4] = {bin[i], bin[i+1], bin[i+2], '\0'};
			char oct_bit = BinOctGroup(temp);
			printf("%c", oct_bit);
		}
	}
}

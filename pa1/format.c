//Alexandra Montgomery
// pa1 format.c


#define BIT_SYS 32
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

long long strength(int base, int exp);
long long TheTwoBinToDec(char* binary);
void IEE_To_Float(char* binary);
double calulator(char* cal);







int main(int argc, char *argv[])
{
	if ((strcmp(argv[2], "int") == 0))
	{
		long long int_output = TheTwoBinToDec(argv[1]);
		printf("%lld\n", int_output);
	}
	else if ((strcmp(argv[2], "float") == 0))
	{
                IEE_To_Float(argv[1]);
	}
	else
	{
		printf("The input type is invalid or is not supported!!!\n");
	}
	return 0;
}








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







long long TheTwoBinToDec(char* binary)
{
    double num_bits = strlen(binary);
    double power = pow(2, num_bits-1);
    long long sum = 0;
    int i;

    if (num_bits == BIT_SYS)
    {
		for (i=0; i < num_bits; ++i)
		{
			if ( i==0 && binary[i]!='0')
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
    	for (i=0; i < num_bits; ++i)
    	{
    		sum = sum + (binary[i]-'0')*power;
    		power /= 2;
    	}
    }

    return sum;
}








void IEE_To_Float(char* binary)
{
        int exp_ind = 9;
	int sign_bit =( binary[0] - '0');
	char exp[9] = {'\0'};
	char cal[24] = {'\0'};
	int i,j=0, k=0;
	for (i = 1; i < BIT_SYS; ++i)
        {
           if (i < exp_ind)
           {
	      exp[k] = binary[i];
	      k++;
	   }
	   else
	   {
              cal[j] = binary[i];
	      j++;
	   }
        }
        if ((strcmp(exp, "11111111") == 0))
        {
           if (cal[0] == '0')
           {
              if (sign_bit == 1)
              {
                 printf("ninf\n");
                 exit(1);
              }
              else if (sign_bit == 0)
              {
                 printf("pinf\n");
                 exit(1);
              }
              else
              {
                 printf("Sign bit and the entire input number should be in 1s and 0s\n");
                 exit(1);
              }
           }
           else if (cal[0] == '1')
           {
              printf("NaN\n");
              exit(1);
           }
           else
           {
              printf("should be in ones and zeros\n");
           }
        }
        long long exponent = TheTwoBinToDec(exp) - 127;
        double caltissa = calulator(cal);
        if (caltissa == 0.0)
        {
           if (sign_bit == 0)
           {
              printf("0.0e0\n");
           }
           else if (sign_bit == 1)
           {
              printf("-0.0e0\n");
           }
           else
           {
              printf("should be in ones and zeros\n");
           }
        }
        else
        {
           double sign_int = pow(-1, sign_bit);
           double value = sign_int * (1+caltissa) * (pow(2, exponent));
           printf("%e\n", value);
        }
}







double calulator(char* cal)
{
   int len = strlen(cal);
   int i;
   double calt = 0.0;
   for (i=0; i<len; i++)
   {
      if (cal[i] == '1')
      {
         calt += pow(2, (-1)*(i+1));
      }
      else
      {
         continue;
      }
   }
   return calt;
}

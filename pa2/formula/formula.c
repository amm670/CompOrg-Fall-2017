//Alexandra Montgomery
// formula.c


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "nCr.h"

int main(int argc, char *argv[])
{
  if (strcmp(argv[1], "-h") == 0)
  {
    printf("formula <positive integer>\n");
  }
  else
  {
    char *ptr;
    int n = (int) strtol(argv[1], &ptr, 10);
    if (n > 12)
    {
      printf("Factorial Number is Too Big, n < 13\n");
      exit(1);
    }
    else
    {
      int i=1;
      printf("(1+x)^%d = 1 + ", n);
      while (i < n)
      {
        printf("%d*x^%d + ", nCr(n, i), i);
        if (i%5 == 0)
        {
          printf("\n           ");
        }
        i++;
    }
    printf("1*x^%d\n", n);
    }
  }
  return 0;
}

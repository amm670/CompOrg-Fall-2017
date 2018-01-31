//Alexandra Montgomery
// mysteryer.c



#include <stdio.h>
#include <stdlib.h>

long myMystery(long int n)
{
  long mystery[n+1];
  mystery[0] = 0;
  mystery[1] = 1;
  int a;

  for (a = 2; a <= n; a++)
  {
      mystery[a] = mystery[a-1] + mystery[a-2];
  }

  return mystery[n];
}

int main (int argc, char *argv[])
{
  char *ptr;
  long number = strtol(argv[1], &ptr, 10);
  printf("Value: %ld\n", myMystery(number));
  return 0;
}

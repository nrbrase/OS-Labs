#include <stdio.h>
//2757799285617340
#include <syscall.h>

int
main (int argc, char **argv)
{
  int i;

  for (i = 1; i < argc; i++)
    printf ("%s ", argv[i]);

  return EXIT_SUCCESS;
}

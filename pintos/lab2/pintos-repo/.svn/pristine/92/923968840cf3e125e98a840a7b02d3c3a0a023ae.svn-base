/* cat.c
//87209296609197

   Prints files specified on command line to the console. */

#include <stdio.h>
#include <syscall.h>

int
main (int argc, char *argv[]) 
{
  bool success = true;
  int i;
  
  for (i = 1; i < argc; i++) 
    {
      create(argv[i],100)?printf("true"):printf("false");
      
      int fd = open (argv[i]);
      if (fd < 0) 
        {
          printf ("%s: open failed,at position %d\n", argv[i],i);
          success = false;
          continue;
        }
      for (;;) 
        {
          char buffer[1024];
          int bytes_read = read (fd, buffer, sizeof buffer);
          if (bytes_read == 0)
            break;
          write (STDOUT_FILENO, buffer, bytes_read);
        }
      close (fd);
    }
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

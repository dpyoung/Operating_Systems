/* touch.c
 *
 *	Name: Alisha Verma 
 *	Name: Dan Young
 *	Assignment: HW3 Question 4
 *		
 *	This program updates file timestamps.
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <fcntl.h>

#define COPYMODE        0644

int do_touch(char *);

int main(int ac, char *av[])
{
    int   in_fd, new_fd;
	if ( ac != 2 )
	{
                fprintf( stderr, "usage: %s filename\n", *av);
                exit(1);
        }
	if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
	{
		 if((new_fd=creat( av[1], COPYMODE)) == -1 )
			{
			    fprintf( stderr, "usage: %s touch filename\n", *av);
	                exit(1);
				
			}
		else
			{
				do_touch(av[1]);
			}
	}   
	else 
	{
		do_touch(av[1]);
		
	}	
}

int do_touch (char* file_path)
{
  struct stat infobuf;
  
  struct utimbuf new_times;
 
  if (stat(file_path, &infobuf) < 0) 
  {
    /*perror(file_path);*/
    fprintf( stderr, "Cannot stat file: %s \n", file_path);
       exit(1);
  }
  new_times.actime = time(NULL) ;  /* set  atime to current time */
  new_times.modtime = time(NULL);    /* set mtime to current time */
  if (utime(file_path, &new_times) < 0) 
  {
    fprintf( stderr, "Cannot modify contents of: %s \n", file_path);
       exit(1);
  }
  return 0;
}


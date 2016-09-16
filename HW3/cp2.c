/* cp2.c
 * 	
 *	Name: Daniel Young
 *	Name: Alisha Verma
 *	Assignment: HW3 Question 3
 *
 *	Notes: Copy-Pasta.
 *
 *
 */
#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<errno.h>

#define BUFFERSIZE	4096
#define COPYMODE	0644	// this is octal for 110 100 100
					// S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
void oops(char *, char *);
int writefile(int in_file_descriptor, int out_file_descriptor);

int main(int ac, char *av[])
{
	int	in_fd, out_fd, errtype;
	int 	iteration = 1; 
	int	do_not_clobber = 0;
	DIR		*dir_ptr;			/* the directory */
	struct dirent	*direntp;		/* entry */

						/* check args 	*/
	if ( ac < 3 )
	{
		fprintf( stderr, "usage: %s [-n] source destination\n", *av);
		exit(1);
	}
	/* open files	*/
	else if ( (ac >= 4) )
	{
		if (!(dir_ptr = opendir(av[ac-1])))
		{
			if (ac == 4 && av[1][1] != '\0' && ( av[1][1] == 'n' || av[1][1] == 'N' ) )
	  	 	{
				if ( (in_fd=open(av[2], O_RDONLY)) == -1 )
					oops("Cannot open ", av[2]);
				if ((out_fd=open(av[3], O_RDONLY)) > -1)
				{
					printf("Destination File Already Exists! Did Not Clobber.\n");
					if ( close(out_fd) == -1 )
						oops("Cannot close ", av[3]);
					return 0;
				}
				if ( (out_fd=creat( av[3], COPYMODE)) == -1 )
					oops( "Cannot creat", av[3]);
				errtype = writefile( in_fd, out_fd);
				if (errtype != 0)
				{
		 			if (errtype == 1)
						{oops("Write error to file.", av[3]);
						exit(1);}
					else
						{oops("Read error from file.", av[2]);
						exit(1);}
				}
		    		printf("File written: %s\n", av[3]);
 		    		if (close(in_fd) == -1 || close(out_fd) == -1 )
		  			oops("Error closing files","");
				return 0;
			}
			/*else  //my fruitless attempt at mkdir
			{
				if(!(mkdir(av[ac-1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )))
					oops("Error making directory", av[ac-1]);
				if (!(dir_ptr = opendir(av[ac-1])))
					oops("Error opening new directory", av[ac-1]);
			}*/
			else
			{ 
				printf("Error: destination must be a folder to copy multiple files.");
			}
		}
	   	if (!(direntp = readdir(dir_ptr)))
		{	
			printf("Cannot read copy destination.\n");	
			return 1;
		}
		if (direntp->d_type == 4)	/* 4 == DT_DIR; ISO99 error in compiler  */
		{				/* DT_DIR not defined in ISOC99 Standard */
		  while (iteration < (ac-1))
		  {
		    if( (do_not_clobber == 0) && (av[1][0] == '-') 
			&& ( (av[1][1] != 'n') || (av[1][1] != 'N') ) )
		    {
			iteration = 2;
			do_not_clobber = 1;
		    }
		    char path[1024];/*magic number, because I ~expect~ paths less than this
	 	 	 	 	 * and don't want a ridiculously large buffer */
		    int len = snprintf(path, sizeof(path)-1, "%s/%s", av[ac-1], av[iteration]);	 	 
		    path[len] = 0;
 	 	    if ( (in_fd=open(av[iteration], O_RDONLY)) == -1 )
		    {
			   printf("Could not open source file %s\n", av[iteration]); 
			   iteration++;
			   continue;
		    }
	 	    if ( (do_not_clobber == 1) && (out_fd=open( path, O_RDONLY)) > -1)
		    {
			  printf("Destination File Already Exists! Did Not Clobber.\n");
			  if ( close(out_fd) == -1 )
				oops("Cannot close ", path);
			  iteration++;
			  continue;
		    }
		    if ( (out_fd=creat( path, COPYMODE)) == -1 )
			  oops( "Cannot creat", path);
		    errtype = writefile( in_fd, out_fd); /* magic happens */
		    if (errtype != 0)
		    {
		  	  if (errtype == 1)
		 	 	 printf("Write error to file"/*, path*/);
		  	  else
		  	  	 printf("Read error from file"/*, av[iteration]*/);
		    }
		    printf("File written: %s\n", path);
		    /* close files */
 		    if ( close(in_fd) == -1 || close(out_fd) == -1 )
		  	  oops("Error closing files","");
	 	    iteration++;
		  }/*end while*/
		}
		else
		printf("Error: destination must be a folder to copy multiple files.");
		
	   /*else printf("Error: destination must be a folder to copy multiple files.");*/  
	}
	else if (ac == 3)
	{
		if (!(dir_ptr = opendir(av[ac-1])))
		{
			printf("Copy destination is not a folder. Continuing...\n");

			if ( (out_fd=creat( av[2], COPYMODE)) == -1 )
				oops( "Cannot creat: ", av[2]);
			if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
			{
				printf("Could not open source file %s\n", av[1]); 
				oops( "Cannot open: ", av[1]);
			}
			errtype = writefile( in_fd, out_fd); /* magic happens */
			if (errtype != 0)
			{
				if (errtype == 1)
					fprintf( stderr, "Cannot write new file: %s\n", av[2]);
				else
					fprintf( stderr, "Read error from file: %s\n", av[1]);
			}
			printf("File written: %s\n", av[2]);
			/* close files */
 			if ( close(in_fd) == -1 || close(out_fd) == -1 )
				oops("Error closing files","");
			return 0;
		}
	   	if (!(direntp = readdir(dir_ptr)))
		{	
			printf("Cannot read copy destination.\n");	
			return 1;
		}
		if (direntp->d_type == 4)
		{
			/* Copy-Paste; may lack elegance, but functions great! */
		    char path[1024];/*magic number, because I ~expect~ paths less than this
	 	 	 	 	 * and don't want a ridiculously large buffer */
		    int len = snprintf(path, sizeof(path)-1, "%s/%s", av[2], av[1]);	 	 
		    path[len] = 0;
 	 	    if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
		    {
			   printf("Could not open source file %s\n", av[1]); 
			   return 1;
		    }
		    if ( (out_fd=creat( path, COPYMODE)) == -1 )
			  oops( "Cannot creat", path);
		    errtype = writefile( in_fd, out_fd); /* magic happens */
		    if (errtype != 0)
		    {
		  	  if (errtype == 1)
		 		/*printf("Write error to file");*/
				fprintf( stderr, "Cannot write new file: %s\n", path);
		  	  else
		  	  	/*printf("Read error from file");*/
				fprintf( stderr, "Read error from file: %s\n", av[iteration]);
		    }
		    printf("File written: %s\n", path);
		    /* close files */
 		    if ( close(in_fd) == -1 || close(out_fd) == -1 )
		  	  oops("Error closing files","");
		}
	}
	else
	{
		fprintf( stderr, "usage: %s [-n] source, ..., destination\n", *av);
		exit(1);
	}
	return 0;
}

int writefile(int in_file_descriptor, int out_file_descriptor)
{
    int 	num_read, num_written;
    char	buf[BUFFERSIZE];
    /* Original version failed on short writes; this is one discovered solution*/
    while (num_read = read(in_file_descriptor, buf, sizeof buf), num_read > 0)
    {
        char *out_ptr = buf;
        while(num_read > 0)
	{
            num_written = write(out_file_descriptor, out_ptr, num_read);

            if (num_written >= 0)
            {
                num_read -= num_written;
                out_ptr += num_written;
            }
            else if (errno != EINTR)
            {
		fprintf( stderr, "Cannot write new file. %s\n", "?");
			return 1;
        	/* printf("Write error!"); */
            }
        }
    }
    if (num_read == 0)
    { /* Closing handled by Main */
        /* Success! */
        return 0;
    }
	return 1; /* mysterious error */
}

void oops(char *s1, char *s2)
{	// http://man7.org/linux/man-pages/man3/errno.3.html#NOTES
	fprintf(stderr,"Error: %s ", s1);
	perror(s2);
	exit(1);
}


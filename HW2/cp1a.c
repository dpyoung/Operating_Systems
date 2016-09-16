/* 	
	Course: CS 59000-01
  	Name: Daniel Young
  	Name: Alisha Verma
  	Assignment: 2
 */
#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<string.h>

#define BUFFERSIZE	4096
#define COPYMODE	0644	// this is octal for 110 100 100
					// S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
void oops(char *, char *);

int main(int ac, char *av[])
{
	int	in_fd, out_fd, n_chars;
	char	buf[BUFFERSIZE];
	
						/* check args 	*/
	if ( ac < 3 || ac > 4 ){
		fprintf( stderr, "usage: %s [-n] source destination\n", *av);
		exit(1);
	}
						/* open files	*/

	else if (ac == 3)
	{
		if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
			oops("Cannot open ", av[1]);
	
		if ( (out_fd=creat( av[2], COPYMODE)) == -1 )
			oops( "Cannot creat", av[2]);
	}
	/*NOTE: This is a workaround to a string argument. FUTURE FIX*/
	/* Check for null terminator necessary to avoid checking garbage*/
	else if ((ac == 4) && av[1][1] != '\0' && av[1][1] == 'n')
	{
		if ( (in_fd=open(av[2], O_RDONLY)) == -1 )
			oops("Cannot open ", av[2]);
		
		if ( (out_fd=open(av[3], O_RDONLY)) > -1)
			printf("Destination File Already Exists! Did Not Clobber.\n");
			return 1;

		if ( (out_fd=creat( av[3], COPYMODE)) == -1 )
			oops( "Cannot creat", av[3]);
	}
	else
	{
		fprintf( stderr, "usage: %s [-n] source destination\n", *av);
		exit(1);
	}
						/* copy files	*/

	while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
		if ( write( out_fd, buf, n_chars ) != n_chars )
			oops("Write error to ", av[2]);	/* fails on short writes */
	if ( n_chars == -1 )
			oops("Read error from ", av[1]);

						/* close files	*/

	if ( close(in_fd) == -1 || close(out_fd) == -1 )
		oops("Error closing files","");

	return 0;
}

void oops(char *s1, char *s2)
{	// http://man7.org/linux/man-pages/man3/errno.3.html#NOTES
	fprintf(stderr,"Error: %s ", s1);
	perror(s2);
	exit(1);
}


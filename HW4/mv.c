/**	
 **	Name: Alisha Verma
 **	Name: Daniel Young
 **	CS59000-01 - OS
 **	Assignment 4 - mv.c	
 **	
 **	mv.c
 **
 **		if last arg is a dir, then move all files into the directory
 **		if there are two args and last argument is a file, then move first to
 **		second.  So far it will clobber second if exists. We need to make it no clobber.
 **		
 **/

#include	<stdio.h>
#include	<limits.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<dirent.h>
#include	<errno.h>

#ifndef	PATH_MAX
	#define	PATH_MAX 1024
#endif

void mv(char *,char *);
void mv_to_newdir(int, char **, char *);
void show_usage(char *);
int  isadir(char *);
int getopt(int argc, char * const argv[], const char *optstring);

static int no_clobber = 0;

extern char *optarg;
extern int optind, optopt;

int	in_fd, out_fd;

int main(int ac, char *av[])
{
	if ((ac < 3)) 
		show_usage(av[0]);
	else
	{
		int opt;
		while((opt = getopt(ac, av, "Nn")) != -1)
		{
			switch (opt)
			{
			case 'n':
			case 'N':
				no_clobber = 1;
				printf("-n set; will not clobber!\n");
				break;
			default:
				printf("Argument invalid. Continuing with program.\n");
			}
		}
	/*
	 *	if last arg is a dir, then move args to there
	 */
		if (isadir( av[ac-1])) /*ac-1*/
		{
			/*printf("Trying the directory method....\n");*/ /*Debug*/
			mv_to_newdir(ac,av,av[ac-1]); /*ac-1, av, av[ac-1]*/
			return 0;
		}
	/*
	 *	last arg is not a dir, so must be two args only
	 */
		else
		{
			/*printf("Trying the rename method....\n");*/ /*DEBUG*/
			mv(av[optind], av[optind+1]);/*av[ac-2], av[ac-1]*/
			return 0;
		}			
	}
	return 0;
}

int isadir( char *name )
/*
 *	calls stat, then masks the st_mode word to obtain the
 *	filetype portion and sees if that bit pattern is the
 *	pattern for a directory
 */
{
	/*struct	stat info;*/
	DIR	*dir_ptr;
	struct dirent	*direntp;
	/*printf("Trying Directory stuffs...");*/
	if (!(dir_ptr = opendir(name)))
	{
		printf("Can't Open Directory\n");
		return 0;
	}
	if ((direntp = readdir(dir_ptr)))
	{
		if ((direntp->d_type == 4)) /*workaround*/
		{
			/*printf("Okay, that's a directory...\n");*/
			return 1; /*yes, a directory*/
		}
		return 1;
	}
	return 0;
	/*oh my goodness, clever*/
	/*return ( stat(name,&info)!=-1 && (info.st_mode & S_IFMT) == S_IFDIR );*/
}

void mv_to_newdir( int ac, char *av[], char *newdir )
/*
 *	move av[1], av[2], ... av[ac-1] into newdir
 */
{
	char	newpath[PATH_MAX];
	int	pos;
	for( pos = optind; pos<ac-1; pos++ )
	{
		if ( strlen(newdir)+strlen(av[pos])+2 <= PATH_MAX )
		{
			sprintf( newpath , "%s/%s", newdir, av[pos]);
			printf("%s\n", newpath);
			mv( av[pos] , newpath );
		}
	}
}

void mv( char *oldname, char *newname )
{
	if ((strcmp(oldname, newname) == 0))
	{
		printf("Destination is same as source. Stopping....");
		return;
	}
	/*check that file may be opened*/
	if ( (in_fd=open(oldname, O_RDONLY)) == -1 )
	{
		fprintf(stderr,"Cannot open %s\n", oldname);
		exit(1);
	}
	/*if file exists and -N, don't clobber...*/
	if ((out_fd=open(newname, O_RDONLY)) > -1)
	{
		if (no_clobber == 1)
		{
			printf("Destination File Already Exists! Did Not Clobber.\n");
			if ( close(out_fd) == -1 )
			{
				fprintf(stderr, "Cannot close %s\n", newname);
				exit(1);
			}
			return;
		}
		else
		{
			printf("Destination File Already Exists. Overwriting....\n");
			if ( close(out_fd) == -1 )
			{
				fprintf(stderr, "Cannot close %s\n", newname);
				exit(1);
			}
			unlink(newname);
		}
	}
	close(in_fd);
	/*Magic? Maybe?*/ 
	/*printf("Debug Statment: Got to the Link part...\n");*/
	if ( link(oldname, newname) == -1 || unlink(oldname) == -1 )
	{
		fprintf(stderr,"error mv'ing %s to %s\n", oldname, newname);
		exit(1);
	}
	return;
}

void show_usage(char * arg)
{
	printf("%s: [-n] source destination\n", arg);
}


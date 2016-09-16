/* 
	Course: CS 59000-01
  	Name: Daniel Young
  	Name: Alisha Verma
  	Assignment: 2

	Modified Version of File:
	utmplib.c  - functions to buffer reads from utmp file 
 *
 *	functions are
 *		utmp_open( filename )		open file
 *							returns -1 on error
 *		utmp_next( )			return pointer to next struct
 *							returns NULL on eof
 *		utmp_close() 			close file
 *
 *	reads NRECS per read and then doles them out from the buffer
 */
#include	<unistd.h>
#include	<fcntl.h>
#include	<utmp.h>

#define NRECS	16
#define NULLUT	((struct utmp *)NULL)
#define UTSIZE	(sizeof(struct utmp))

static char	utmpbuf[NRECS * UTSIZE];	/* storage		*/
static int		num_recs;					/* num stored	*/
static int		cur_rec;					/* next to go	*/
static int		fd_utmp = -1;				/* read from	*/

int	utmp_open( char *filename )
{
	fd_utmp = open( filename, O_RDONLY );	/* open it		*/
	cur_rec = num_recs = 0;				/* no recs yet 	*/
	return fd_utmp;					/* report		*/
}

int utmp_reload()
/*
 *	read next bunch of records into buffer
 */
{
	int     amt_read;

						/* read them in         */
	amt_read = read( fd_utmp , utmpbuf, NRECS * UTSIZE );   

						/* how many did we get? */
	num_recs = amt_read/UTSIZE;
						/* reset pointer	*/
	cur_rec  = 0;
	return num_recs;
}

struct utmp *utmp_next()
{
	struct utmp *recp;

	if ( fd_utmp == -1 )							/* error ?	*/
		return NULLUT;
	if ( cur_rec==num_recs && utmp_reload()==0 )	/* any more ?	*/
		return NULLUT;
   /* if ( cur_rec != num_recs && utbufp->ut_type != USER_PROCESS)  */
						/* get address of next record	*/

	recp = ( struct utmp *) &utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	/*Begin Student Additions*/
	if ( recp->ut_type == USER_PROCESS )
	     return recp; 
	else {return utmp_next();}
	/*End Student Additions*/
	return recp;
}

void utmp_close()
{
	if ( fd_utmp != -1 )		/* don't close if not	*/
		close( fd_utmp );	/* open	*/
}


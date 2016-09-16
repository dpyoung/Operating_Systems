/* 
	Course: CS 59000-01
  	Name: Daniel Young
  	Name: Alisha Verma
  	Assignment: 2
 */
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>
#include	<utmp.h>
#include	"utmplib.h"

#define	SHOWHOST

void show_info(struct utmp *);
void showtime(time_t);

/*Start Student Modifications*/
int main(int ac, char *av[])
{
	/*Start Student Additions*/
	struct utmp	*utbufp,		/* holds pointer to next rec	*/
				*utmp_next();	/* returns pointer to next	*/
	if (ac == 2)
	{
		if( utmp_open(av[1]) == -1)
		{
			perror(av[1]);
			exit(1);
		}
	}
	else if (ac == 1)
	{
		if ( utmp_open( UTMP_FILE ) == -1 )
		{
			perror(UTMP_FILE);
			exit(1);
		}
	}
	else
	{
		printf("Usage: %s [source_file]\n", *av);
		return 0;
	}
	
	printf("ut_type\tut_pid\tut_line\tut_id\tut_name\tut_exit\t\tut_session\tut_time\t\tut_host\n");
	/*End Student Additions and Modifications*/
	while ( ( utbufp = utmp_next() ) != ((struct utmp *) NULL) )
		show_info( utbufp );
	utmp_close( );
	return 0;
}
/*
 *	show info()
 *			displays the contents of the utmp struct
 *			in human readable form
 *			* displays nothing if record has no user name
 */
void show_info( struct utmp *utbufp )
{
/*	  if ( utbufp->ut_type != USER_PROCESS )*/
/*	  	  return;*/
	/*Begin Student Modifications*/
	printf("%i", utbufp->ut_type);	/* the type	 */
	printf("\t");						/* a space	*/
	printf("%d", utbufp->ut_pid);	/* the process ID	/
	printf("\t");						/* a space	*/
	printf("%s", utbufp->ut_line);	/* the line	 */
	printf("\t");						/* a space	*/
	printf("%s", utbufp->ut_id);	/* the ID*/	   
	printf("\t");						/* a space	*/
	printf("%s", utbufp->ut_name);	/* the log in name	*/
	printf("\t");						/* a space	*/
	struct exit_status temp = utbufp->ut_exit;
	int temp1 = temp.e_termination;
	int temp2 = temp.e_exit;
	printf("term=%i exit=%i", temp1, temp2);	/* the term and exit	
	printf("\t");						/* a space	*/
	printf("session=%i", utbufp->ut_session);	/* the session	*/
	printf("\t");						/* a space	*/
	showtime( utbufp->ut_time );		/* display time	*/
	printf("\t");
#ifdef SHOWHOST
	if ( utbufp->ut_host[0] != '\0' )
		printf(" (%s)", utbufp->ut_host);	/* the host	*/
#endif
	printf("\n");					/* newline for cleanliness	*/
	/*End Student Modifications*/
}

void showtime( time_t timeval )
/*
 *	displays time in a format fit for human consumption
 *	uses ctime to build a string then picks parts out of it
 *	Note: %12.12s prints a string 12 chars wide and LIMITS
 *	it to 12chars.
 */
{
	char	*ctime();		/* convert long to ascii	*/
	char	*cp;			/* to hold address of time	*/

	cp = ctime(&timeval);	/* convert time to string	*/
						/* string looks like		*/
						/* Mon Feb  4 00:46:40 EST 1991 */
						/* 0123456789012345.		*/
	printf("%12.12s", cp+4 );	/* pick 12 chars from pos 4	*/
}


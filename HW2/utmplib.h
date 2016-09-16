/* utmplib.h  - functions to buffer reads from utmp file 
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
 
 int	utmp_open( char *filename );
 
 struct utmp *utmp_next();
 
 void utmp_close();
 
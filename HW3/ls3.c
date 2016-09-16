/* ls3.c
 *	
 *	Name: Dan Young
 *	Name: Alisha Verma 
 *	Assignment: HW3 Question 1
 *		
 *	The bug in this program is its use of dostat(...)
 *	dostat() will, according to the stat() documentation,
 *	take in a symbolic link and gather data so long as the
 *	file exists. In this case, -1 is returned if a directory
 *	is analyzed by stat(), even if data exists in a stat
 *	struct. A conditional print option may be implemented to
 *	restrict struct analysis to work only for those 
 *	directory entries which may contain relevant struct data
 */
#include	<time.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>

void do_ls( char[] );
void dostat( char * );
void show_file_info( char *, struct stat * );
void mode_to_letters( int , char[] );
char *uid_to_name( uid_t );  /* uid_t is defined in sys/types.h */
char *gid_to_name( gid_t );  /* gid_t is defined in sys/types.h */

int main(int ac, char *av[])
{
	if ( ac == 1 )  /* NOTE: Compare this main() to more01.c from ch1 */
		do_ls( "." );
	else
		while ( --ac )
		{
			printf("%s:\n", *++av );
			do_ls( *av );
		}
	return 0;
}

/*
 *	list files in directory called dirname
 */
void do_ls( char dirname[] )
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls3: cannot open %s\n", dirname);
	else {
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
			dostat( direntp->d_name );
		closedir(dir_ptr);
	}
}

/*
 * get the info about 'filename'.
 */
void dostat( char *filename )
{
	struct stat	info;

	if ( stat(filename, &info) == -1 )	/* cannot stat	 */
		/* if ( ? ) */ 
		/*perror( filename );*/		/* say why	 */
		show_file_info( filename, &info );
		/* actually a dir? Simply has no 'name' in DS*/
	else					/* else show info	 */
		show_file_info( filename, &info );
}

/*
 * display the info about 'filename'.  The info is stored in struct at *info_p
 */
void show_file_info( char *filename, struct stat *info_p )
{
	char	modestr[11];

	mode_to_letters( info_p->st_mode, modestr );
	
	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );
}

/*
 * utility functions
 */

/*
 * This function takes a mode value and a char array
 * and puts into the char array the file type and the 
 * nine letters that correspond to the bits in mode.
 * NOTE: It does not code setuid, setgid, and sticky
 * codes
 */
void mode_to_letters( int mode, char str[] )
{
	strcpy( str, "----------" );           /* default=no perms	*/

	if ( S_ISDIR(mode) )  str[0] = 'd';	/* directory?	*/
	if ( S_ISCHR(mode) )  str[0] = 'c';	/* char devices	*/
	if ( S_ISBLK(mode) )  str[0] = 'b';	/* block device	*/

	if ( mode & S_IRUSR ) str[1] = 'r';	/* 3 bits for user	*/
	if ( mode & S_IWUSR ) str[2] = 'w';
	if ( mode & S_IXUSR ) str[3] = 'x';

	if ( mode & S_IRGRP ) str[4] = 'r';	/* 3 bits for group	*/
	if ( mode & S_IWGRP ) str[5] = 'w';
	if ( mode & S_IXGRP ) str[6] = 'x';

	if ( mode & S_IROTH ) str[7] = 'r';	/* 3 bits for other	*/
	if ( mode & S_IWOTH ) str[8] = 'w';
	if ( mode & S_IXOTH ) str[9] = 'x';
}

#include	<pwd.h>

/* 
 *	returns pointer to username associated with uid, uses getpw()
 */
char *uid_to_name( uid_t uid )
{
	struct passwd	*getpwuid(),	*pw_ptr;
	static char	numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ) {
		sprintf(numstr,"%d", (int)uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

#include	<grp.h>

/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
char *gid_to_name( gid_t gid )
{
	struct group	*getgrgid(),	*grp_ptr;
	static char	numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf(numstr,"%d", (int)gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}


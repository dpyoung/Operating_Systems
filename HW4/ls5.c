/* ls5.c
 *  Course: CS59000-01
 *  Name: Dan Young
 *  Name: Alisha Verma
 *  Assignment: HW4 Question 1
 *  	
 *  This program recursively lists folder contents
 * 	from a specified path. The recursion leads to
 * 	some messy print-out.  
 *  As well, the -i argument prints file inodes.
 */

#include	<unistd.h>
#include	<time.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>

void do_ls( char[] , int);
void dostat( char * );
void show_file_info( char *, struct stat * );
void show_minimal_file_info( char *filename, struct stat *info_p );
void mode_to_letters( int , char[] );
char *uid_to_name( uid_t );  /* uid_t is defined in sys/types.h */
char *gid_to_name( gid_t );  /* gid_t is defined in sys/types.h */

static int do_recursion = 0;
static int do_inode_list = 0;

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, optopt;

int main(int ac, char *av[])
{
	if ( ac == 1 )  /* NOTE: Compare this main() to more01.c from ch1 */
		do_ls( "." , 0);

	else 
	{
		int opt;
		while((opt = getopt(ac, av, "IRir")) != -1)
		{
			switch (opt)
			{
			case 'r':
			case 'R':
				do_recursion = 1;
				break;
			case 'i':
			case 'I':
				do_inode_list = 1;
				break;
			default:
				printf("?\n");
			}
		}
		
		if (optind >= ac)
		{
			do_ls(".", 0);
		}
		else
			do_ls( av[optind], 0);

	}
	return 0;
}

/*
 *	list files in directory called dirname
 */
void do_ls( char dirname[] , int call_level )
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
   	if (!(dir_ptr = opendir(dirname)))
		return;
	while ( (direntp = readdir( dir_ptr ) ) )
	{ 
		if ((direntp->d_type == 4))/* 4 == DT_DIR; ISO99 error in compiler  */
		{			  /* DT_DIR not defined in ISOC99 Standard */
			dostat( direntp->d_name );
			if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
				continue;
			char path[1024];/*magic number, because I ~expect~ paths less than this
					 * and don't want a ridiculously large buffer */
			int len = snprintf(path, sizeof(path)-1, "%s/%s", dirname, direntp->d_name);	
			path[len] = 0;
			/*if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
				continue;*/
			printf("%*s[%s]\n", call_level, "", direntp->d_name);
			if (do_recursion == 1)
			{
				do_ls(path, call_level + 1);
				printf("%*s[%s]\n", call_level, "", dirname);
				/*I realized I could not keep this in an orderly print-out
					without using a queue (future implementation!). So,
					This prints out the directory the recursion is currently
					working on. It is a little messier, but works great.*/
			}
        	}
       		else dostat(direntp->d_name); 
	} 
	closedir(dir_ptr);	
	return;
}

/*
 * get the info about 'filename'.
 */
void dostat( char *filename )
{ 
	struct stat	info;

	if ( stat(filename, &info) == -1 )	/* cannot stat	 */
	{
		show_minimal_file_info(filename,&info);
	}
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

	if(do_inode_list == 1)
	{
		printf( "%9.0f " , (double) info_p->st_ino);
	}
	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );
}

void show_minimal_file_info( char *filename, struct stat *info_p )
{
	char	modestr[11];

	mode_to_letters( info_p->st_mode, modestr );

	if(do_inode_list == 1)
	{
		printf( "%9.0f " , (double) info_p->st_ino);
	}
	printf( "%s"    , modestr ); /*Good for Directories*/
	printf( "%45.0s", " "); /*Proper placement in window*/
	printf( "%s\n"  , filename ); /*Good for Directories*/
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


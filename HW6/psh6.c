/*
	Name: Daniel Young
	Partner Name: Alisha Verma
	Course: CS59000-01 OS
	Assignment: HW6
	Notes: None. Getting exec to cooperate was difficult.
*/
/*
   prompting shell version 6
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

char * next_file(char *);
char ** next_cmd(char *);
char * makestring(char *);
void execute(char *, char *[], char *[], char *);

#define  MAXARGS   20   /* cmdline args */
#define  ARGLEN   100   /* token length */
#define  STDIN_PIPE   0  /* read from */
#define  STDOUT_PIPE  1  /* write to */
#define  oops(m,x){ perror(m); exit(x); }

int main()
{
   char *source = NULL, *dest = NULL;
   char **arglist1 = NULL, **arglist2 = NULL;

   while (1)
   {
      if ( (source = next_file("Source file")) != NULL )
         if ( (arglist1 = next_cmd("Cmd 1")) != NULL )
            if ( (arglist2 = next_cmd("Cmd 2")) != NULL )
               if ( (dest = next_file("Dest file")) != NULL )
               {
                  execute(source, arglist1, arglist2, dest);
                  /* free the argument lists */
                  char **cpp = arglist1;
                  while ( *cpp )
                     free( *cpp++ ); /* free cmd1's args */
                  cpp = arglist2;
                  while ( *cpp )
                     free( *cpp++ ); /* free cmd2's args */
                  free( arglist1 );
                  free( arglist2 );
                  free( source );
                  free( dest );
               }
               else
                  break;
            else
               break;
         else
            break;
      else
         break;
   }
   return 0;
}


/*
 * purpose: read next file name
 * returns: dynamically allocated string holding the file name
 *        : NULL at EOF or ENTER
 */
char * next_file(char *prompt)
{
   char *filename = NULL;
   char namebuf[ARGLEN]; /* read stuff here */

   /* user can type either a file name, ENTER, or CTRL-D */
   printf("%s? ", prompt);
   if ( fgets(namebuf, ARGLEN, stdin) && *namebuf != '\n' )
   {
      /* user typed a file name */
      filename = makestring(namebuf);
   }
   else /* user typed ENTER or CTRL-D (EOF) */
   {
      filename = NULL;
   }
   return filename;
}


/*
 * purpose: read next command line
 * returns: dynamically allocated array holding command line arguments
 *        : NULL at EOF
 */
char ** next_cmd(char *prompt)
{
   char **arglist;      /* an array of ptrs */
   int  numargs;        /* index into array */
   char argbuf[ARGLEN]; /* read stuff here */

   /* get arglist[0] */
   /* user can type either a command name, ENTER, or CTRL-D */
   numargs = 0;
   printf("%s Arg[%d]? ", prompt, numargs);
   if ( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
   {
      /* user typed a command name */
      arglist = malloc( sizeof(char*) * (MAXARGS+1) );
      arglist[numargs++] = makestring(argbuf);
   }
   else /* user typed ENTER or CTRL-D (EOF) */
   {
      return NULL;
   }
   /* get the rest of the arglist */
   while ( numargs < MAXARGS )
   {
      /* user can type either an argument, ENTER, or CTRL-D */
      printf("%s Arg[%d]? ", prompt, numargs);
      if ( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
      {
         /* user typed an argument */
         arglist[numargs++] = makestring(argbuf);
      }
      else /* user typed ENTER or CTRL-D (EOF) */
      {
         arglist[numargs] = NULL;   /* close list */
         return arglist;
      }
   }
   arglist[MAXARGS] = NULL;   /* close list */
   return arglist;
}


/*
 * trim off newline and create storage for the string
 */
char * makestring( char *buf )
{
   char  *cp;

   buf[strlen(buf)-1] = '\0';    /* trim newline */
   cp = malloc( strlen(buf)+1 ); /* get memory   */
   if ( cp == NULL )             /* or die       */
   {
      fprintf(stderr,"no memory\n");
      exit(1);
   }
   strcpy(cp, buf); /* copy chars */
   return cp;       /* return ptr */
}


/*
   Set up and execute the pipeline.
*/
void execute(char *source, char *arglist1[], char *arglist2[], char *dest)
{
   /*Initial Code borrowed from:
   http://stackoverflow.com/questions/19461744/make-parent-wait-for-all-child-processes */
   int childPid, secondChildPid, fd_in, fd_out, apipe[2]; /*Establish pipe, file descriptors for source/dest*/
   if ( pipe(apipe) == -1 )
     oops("Cannot Acquire a Pipe", 1);
   printf("Got a pipe! It is file descriptors: { %d %d }\n", apipe[0], apipe[1]);
   if((fd_in = open(source, O_RDONLY))==-1)
   {
     printf("Failure opening source file");
     return;
   }
   /*oops("Could not open source file", 1);*/
   if((fd_out = creat(dest, 00777))==-1)
   {
     printf("Failure creating dest file");
     return;
   }
   /*oops("Could not creat dest file", 1);*/ /*all permissions; was O_WRONLY*/
   if((childPid = fork()) == -1)
   {
     oops("Cannot Fork",1);
   }
   else if(childPid > 0)
   { /*parent*/
      if( (secondChildPid = fork()) == -1)
      {   oops("Cannot Fork",1);}
      else if( secondChildPid > 0) /*parent*/
      {
         int wait_rv;
         close(apipe[STDOUT_PIPE]);
         close(apipe[STDIN_PIPE]);
         close(fd_out);
         close(fd_in);
         wait_rv = wait(NULL);
         printf("Child %d returned: %d\n", childPid, wait_rv);
         wait_rv = wait(NULL);
         printf("Child %d returned: %d\n", secondChildPid, wait_rv);
      }
      else /*child 2 time*/
      {
         /*close(fd_in);*/         
         close(apipe[STDOUT_PIPE]);
         close(STDIN_FILENO); /* == 0 */
         dup(apipe[STDIN_PIPE]);
         close(apipe[STDIN_PIPE]);
         close(STDOUT_FILENO);
         if ( dup(fd_out) == -1 )
            oops("Child 2 could not redirect stdout...",4);
         close(fd_out);
         /*file, execlp time*/
         execvp(arglist2[0], arglist2);
         /*execlp(arglist2[0], arglist2[0], NULL);*/
         oops(arglist2[0], 4);
      }
   }
   else /*child 1 time*/
   {
      /*close(fd_out);*/
      close(apipe[STDIN_PIPE]);
      close(STDOUT_FILENO);
      dup(apipe[STDOUT_PIPE]);
      close(apipe[STDOUT_PIPE]);      
      close(STDIN_FILENO);
      if ( dup(fd_in) == -1 )
        oops("Child 1 could not redirect stdin", 3);
      close(fd_in);
      /*exec time*/
      execvp(arglist1[0], arglist1);
      /*execlp(arglist1[0], arglist1[0], NULL);*/
      oops(arglist1[0], 5);
   }
}

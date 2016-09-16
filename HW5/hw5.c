/*
	Name: Daniel Young
	Partner's Name: Alisha Verma
	Course: CS59000-01 Operating Systems
	Assignment: HW5
	Notes: This program was done in the simplest way possible.
		That said, the output from this program is more organized
		than was expected. Yay.
*/

/*
   This program does a kind of simple macro expansion on standard
   input and writes the results to standard output.

   The macros in the input stream are of the form #1# through #9#,
   that is, there are at most 9 macros. The value that a macro
   should expand to can come from either a command-line argument
   or from an environment variable.

   If a macro is of the form #n#, where n is a single digit integer
   not equal to 0, and if n < argc, then the macro #n# should expand
   to the n'th command-line argument. If n >= argc, then use the
   C Standard Library function getenv() to see if there is an environment
   variable named _n_ and if there is, use its value to expand the macro.
   If the macro does not have a definition, it should be passed unchanged
   to standard output.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
	int	ch, digity; /* input-provided char, digit */
	char * outenvvar; /* pointer to expanded Macro Environmental Variables */
	char envvar [4] = {'_', '0', '_', '\0'}; /* Environmental Variable Pattern*/
	ch = getchar(); /* start reading from stdin immediately */
	
	while ( (ch != EOF ) ) /* input exists */
	{
		if (ch == '#') /* signalling macro exists */
		{
			ch = getchar(); /* get next char, see if it's a digit */
			if (isdigit(ch) && (ch != 0))
			{
				digity = ch - '0'; /* convert digit to int */
				ch = getchar();
				if( ch == '#') /* Macro pattern complete! Replace time. */
				{
					if (argc > digity) /* user provided arguments */
					{
						printf("%s", argv[digity]);
						ch = getchar();
					}
					else /*Checks for Enviromental Variable Match for Macro*/
					{
						envvar[1] = (digity + '0');
						outenvvar = getenv(envvar);
						if (outenvvar == NULL) /* print macro, give up */
							printf("#%i%c", digity, ch);
						else	/* print expanded macro */
							printf("%s", outenvvar);
						ch = getchar();
					}	 	 	 	 	 
				}
				else /* print pound sign and number, like '#1' */
				{
					printf("#%i", digity);
				}
			}
			else /* print pound sign and any old character provided*/
			{
				printf("#%c", ch);
				ch = getchar();
			}
		}
		else /* just print the character */
		{
			printf("%c", ch);
			ch = getchar();
		}
	};
	/* simply return without error */
	return 0;
}


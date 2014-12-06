/*
Converts the hex codes in URLs to their actual characters
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

//only these chars should really be encoded
char *encoded_chars =	/*rfc 3986*/ "!*'();:@&=+$,/?#[]"
			/*others*/ " \"<>%{}|\\^~`";

void fail(char *fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	printf("\n\nerror: ");
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap); //probably doesn't really matter here
	exit(EXIT_FAILURE);
}

//bug: newline can be \n or \r or \r\n
#define BADC 0xFF
char fhex(char c)
{
	c = tolower(c);
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return (c - 'a') + 10;
	return BADC; //bad character
}

main(int argc, char **argv)
{
	char enc[256] = {0};
	char all_conv = 0;

	for (int i = 0; encoded_chars[i]; i++)
		enc[ (int)(encoded_chars[i]) ] = 1;

	int argi = 1;
	for (; argi < argc; argi++)
	{
		if (argv[argi][0] != '-')
			break;
		else if (strcmp(argv[argi], "--") == 0)
		{
			argi++;
			break;
		}
		else if (strcmp(argv[argi], "-a") == 0)
			all_conv = 1;
		else
		{
			fail("Unrecognized option %s", argv[argi]);
		}
	}

	for (; argi < argc; argi++)
	{
		char *str = argv[argi];
		int len = strlen(str);
		for (int i = 0; i < len; i++)
			if (str[i] != '%')
				putchar(str[i]);
			else
			{
				if (i+2 >= len)
					fail("Incorrect syntax");
				char c1 = str[i+1], c2 = str[i+2];
				if ((c1 = fhex(c1)) == BADC || (c2 = fhex(c2)) == BADC)
					fail("Invalid hex code %%%c%c", c1, c2);
//				printf("(%d %d)", c1, c2);
				putchar(c1*16 + c2);
				i += 2; //skip over hex code
			}
		putchar('\n');
	}
	return 0;
}

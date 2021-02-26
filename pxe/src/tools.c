#include "base.h"

char *to_upper(char *str)
{
	char *start = str;
	while(*str)
	{
		if(*str == '-')
			*str = '_';
		else 
			*str = toupper(*str);

		str++;
	}
	return start;
}


/*
 * Name: modified_since
 * Description: Decides whether a file's mtime is newer than the
 * If-Modified-Since header of a request.
 *

 Sun, 06 Nov 1994 08:49:37 GMT    ; RFC 822, updated by RFC 1123
 Sunday, 06-Nov-94 08:49:37 GMT   ; RFC 850, obsoleted by RFC 1036
 Sun Nov  6 08:49:37 1994         ; ANSI C's asctime() format
 31 September 2000 23:59:59 GMT   ; non-standard

 * RETURN VALUES:
 *  0: File has not been modified since specified time.
 *  1: File has been.
 * -1: Error!
 */
int modified_since(time_t *mtime, char *if_modified_since)
{


}






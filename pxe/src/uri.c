#include "base.h"
#include "httpd.h"

#define HEX_TO_DECIMAL(char1, char2)    \
    (((char1 >= 'A') ? (((char1 & 0xdf) - 'A') + 10) : (char1 - '0')) * 16) + \
    (((char2 >= 'A') ? (((char2 & 0xdf) - 'A') + 10) : (char2 - '0')))


/*  
 * Name: unescape_uri
 *  
 * Description: Decodes a uri, changing %xx encodings with the actual
 * character.  The query_string should already be gone.
 *      
 * Return values:
 *  1: success 
 *  0: illegal string
 */
int unescape_uri(char *uri, char **query_string)
{
	char *uri_old;
	char c, d;
	
	uri_old = uri;

	while((c = *uri_old))
	{
		if(c == '%')
		{
			uri_old++;
			if((c = *uri_old ++ ) && (d = *uri_old ++))
				*uri++ = HEX_TO_DECIMAL(c, d);
			else
				return 0;
		}
		else if(c == '?')
		{
			if(query_string)
				*query_string = ++uri_old;
			*uri = '\0';
			return 1;
		}
		else if(c == '#')		//fragment
		{
			if(query_string)
			{
				++uri_old;
				while((c = *uri_old))
				{
					if(c == '?')
					{
						*query_string = ++uri_old;
						break;
					}
					++uri_old;
				}
			}
			break;
		}
		else 
		{
			*uri++ = c;
			uri_old ++;
		}
	}
	*uri = '\0';
	return 1;
}

/*              
 * Name: translate_uri
 *
 * Description: Parse a request's virtual path.
 * Sets query_string, pathname directly.
 * Also sets path_info, path_translated, and script_name via
 *  init_script_alias
 *
 * Note: NPH in user dir is currently broken
 *
 * Note -- this should be broken up.
 *  
 * Return values:
 *   0: failure, close it down
 *   1: success, continue
 */   
int translate_uri(request *req)
{
	static char buf[MAX_HEADER_LENGTH + 1];
	char *req_uri;
	char *p;
	int uri_len;

	req_uri = req->uri;
	if(req_uri[0] != '/')
	{
		DEBUG("r_bad_request");
		//send_r_bad_request(req);
		return 0;
	}

	uri_len = strlen(req->uri);
	
	req->path_name = strdup("./web/index.html");
	
	//strcat();
	return 1;
}



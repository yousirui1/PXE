#include "base.h"
#include "httpd.h"

#if 0
request *new_request(int fd)
{
	request *req = (request *)malloc(sizeof(request));

	if(!req)
	{
		DEBUG("malloc for new request");
		return NULL;
	}
	//memset(req, 0, offsetof(request, buf) + 1);
	memset(req, 0, sizeof(request));
	req->fd = fd;
	return req;
}

void get_requets(int fd)
{


}

void free_request(request *req)
{


}


void process_request(int fd)
{


}


/*
 * Name: process_logline
 *
 * Description: This is called with the first req->header_line received
 * by a request, called "logline" because it is logged to a file.
 * It is parsed to determine request type and method, then passed to
 * translate_uri for further parsing.  Also sets up CGI environment if
 * needed.
 */
int process_logline(request *req)
{
	char *stop, *stop2;
	static char *SIMPLE_HTTP_VERSION = "HTTP/1.1";
	
	req->logline = req->buf;
	if(!memcmp(req->logline, "GET ", 4))
		req->method = M_GET;
	else if(!memcmp(req->logline, "HEAD ", 5))
		req->method = M_HEAD;
	else if(!memcmp(req->logline, "POST ", 5))
		req->method = M_POST;	
	else
	{
		DEBUG("malformed request: %s", req->logline);
		//send_r_not_implemented(req);	
		return 0;
	}	
	
	req->http_version = SIMPLE_HTTP_VERSION;
	req->simple = 1;

	/* find ' ' since we matched a method above */
	stop = req->logline + 3;
	if(*stop != ' ')
		++stop;
	
	while(*(++stop) == ' ');

	stop2 = stop;	

	while(*stop2 != '\0' && *stop2 != ' ')
		++stop2;

	if(stop2 - stop > MAX_HEADER_LENGTH)
	{
		DEBUG("URI too long %d %s", MAX_HEADER_LENGTH, req->logline);		
		return 0;
	}
	memcpy(req->uri, stop, stop2 - stop);
	req->uri[stop2 - stop] = '\0';

	if(*stop2 == ' ')
	{
		/* if found, we should get an HTTP/x.x */
		unsigned int p1, p2;

		++stop2;
		while(*stop2 == ' ' && *stop2 != '\0')
			++stop2;

		/* scan in HTTP/major.minor */
		if(sscanf(stop2, "HTTP/%u.%u", &p1, &p2) == 2)
		{
			if(p1 == 1 && (p2 == 0 || p2 == 1))
			{
				req->http_version = stop2;
				req->simple = 0;
			}
			else if(p1 > 1 || (p1 != 0 && p2 > 1))
				goto BAD_VERSION;	
		}
		else 
			goto BAD_VERSION;	
	}
	
	if(req->method == M_HEAD && req->simple)
	{
		return 0;
	}
	//req->cgi_env_index = 	

	return 1;
BAD_VERSION:
	return 0;	
}


int process_header_end(request *req)
{
	if(!req->logline)
	{
		//send_r_error(req);
		return 0;
	}

	/* Percent-decode request */
	if(unescape_uri(req->uri, &req->query_string) == 0)
	{
		DEBUG("Problem unescaping uri");
		//send_r_error(req);
		return 0;
	}
	
	/* clean pathname / ./ ../  */
	clean_path(req->uri);

	if(req->uri[0] != '/')
	{
		//send_r_bad_requset(req);
		return 0;
	}

	if(translate_uri(req) == 0)
	{
		return 0;
	}
	if(req->method == M_POST)
	{
		//req->data_fd = create
		if(req->data_fd == 0)
			return 0;
		return 1;
	}
	
	if(req->is_cgi)
	{
		return init_cgi(req);
	}

	req->status = WRITE;
	return init_get(req);
}

/*
 * Name: process_option_line
 *
 * Description: Parses the contents of req->header_line and takes
 * appropriate action.
 */
int process_option_line(request *req)
{
	char c, *value, *line = req->header_line;
	
	value = strchr(line, ':');
	
	if(value == NULL)
		return 0;

	*value ++ = '\0';
	to_upper(line);
	
	while((c = *value) && (c == ' ' || c == '\t'))
		value++;

	if(!memcmp(line, "IF_MODIFIED_SINCE", 18) && !req->if_modified_since)
		req->if_modified_since = value;

	else if(!memcmp(line, "CONTENT_TYPE", 13) && !req->content_type)
		req->content_type = value;

	else if(!memcmp(line, "CONTENT_LENGTH", 15) && !req->content_length)
		req->content_length = value;

	else if(!memcmp(line, "CONNECTION", 11) && !req->keepalive != KA_STOPPED)
		req->keepalive = (!strncasecmp(value, "Keep-Alive", 10) ? KA_ACTIVE : KA_STOPPED);

	else if(!memcmp(line, "ACCEPT", 7))
		add_accept_header(req, value);
	
	else if(!memcmp(line, "REFERER", 8))
	{
		req->header_referer = value;
		if(!add_cgi_env(req, "REFERER", value, 1))
			return 0;
	}
	else if(!memcmp(line, "USER_AGENT", 11))
		req->header_user_agent = value;
		if(!add_cgi_env(req, "USER_AGENT", value, 1))
			return 0;
	else
	{
		if(!add_cgi_env(req, line, value, 1))
			return 0;
	}
	return 1;
}

void add_accept_header(request *req, char *mime_type)
{

}


void free_requests(void)
{


}

#endif




#include "base.h"
#include "httpd.h"

#if 0
/*
 * Name: read_header
 * Description: Reads data from a request socket.  Manages the current
 * status via a state machine.  Changes status from READ_HEADER to
 * READ_BODY or WRITE as necessary.
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 */

int read_header(request *req)
{
	int ret, len;
	int bytes, buf_bytes_left;
	char *check, *buffer;
	if(req->status < BODY_READ)
	{
		len = MAX_BUFLEN - req->data_size;
		if(len < 1)
		{
			DEBUG("buffer over read_header status: %d data_size: %d", req->status, req->data_size);
			req->status = DEAD;
			return 0;
		}
			
		ret = read(req->fd, req->buf + req->pos, len);
		if(ret < 0)
		{
			if(errno == EINTR)
				return 1;
			if(errno == EAGAIN || errno == EWOULDBLOCK)  //request blocked	
				return -1;
			return 0;
		}
		else if(ret == 0)
		{
			return 0;
		}
		req->data_size += ret;
		req->buf[req->data_size] = '\0';
		DEBUG("buf %s", req->buf + req->pos);
	}

	check = req->buf + req->pos;
	buffer = req->buf;
	bytes = req->data_size;

	while(check < (buffer + bytes))
	{
		switch(req->status)
		{
			case READ_HEADER:
				if(*check == '\r')
				{
					req->status = ONE_CR;
					req->header_end = check;
				}
				else if(*check == '\n')
				{
					req->status = ONE_LF;
					req->header_end = check;
				}
				break;
			case ONE_CR:
				if(*check == '\n')
					req->status = ONE_LF;
				else if(*check != '\r')
					req->status = READ_HEADER;
				break;
			case ONE_LF:
				if(*check == '\r')
					req->status = TWO_CR;
				else if(*check == '\n')
					req->status = BODY_READ;
				else 
					req->status = READ_HEADER;
				break;
			case TWO_CR:
				if(*check == '\n')
					req->status = BODY_READ;
				else if(*check != '\r')
					req->status = READ_HEADER;
				break;
			default:
				break;
		}
		req->pos ++;
		check++;
		if(req->status == ONE_LF)
		{
			*req->header_end = '\0';
	
			/* terminate string that begins at req->header_line */
			if(req->logline)
			{
				if(process_option_line(req) == 0)
					return 0;
			}
			else
			{
				if(process_logline(req) == 0)
				{
					return 0;
				}
				if(req->simple)
				{
					return process_header_end(req);
				}
			}
			req->header_line = check;
		}
		else if(req->status == BODY_READ)
		{
			int ret = process_header_end(req);
			if(ret && req->method == M_POST)
			{
				req->header_line = check;
				req->header_end = req->buf + req->pos;
				req->status = BODY_WRITE;

				if(req->content_length)
				{


				}
				else
				{


				}
			}
		}

	}
	return 1;
}

/*
 * Name: read_body
 * Description: Reads body from a request socket for POST CGI
 *
 * Return values:
 *
 *  -1: request blocked, move to blocked queue
 *   0: request done, close it down
 *   1: more to do, leave on ready list
 *

 As quoted from RFC1945:

 A valid Content-Length is required on all HTTP/1.0 POST requests. An
 HTTP/1.0 server should respond with a 400 (bad request) message if it
 cannot determine the length of the request message's content.

 */
int read_body(request *req)
{
	int free_len, read_len;
	//free_len = 
	


}

/*
 * Name: write_body
 * Description: Writes a chunk of data to a file
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: EOF or error, close it down
 *   1: successful write, recycle in ready queue
 */
int write_body(request *req)
{


}
#endif

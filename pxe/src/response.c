#include "base.h"
#include "httpd.h"

int req_write(request *req, char *msg)
{
	int len = strlen(msg);
	
	if(!msg || req->status == DEAD)
		return req->buf_end;
	
	if(req->buf_end + len > MAX_BUFLEN)
	{
				
	}
	memcpy(req->buf + req->buf_end, msg, len);
	req->buf_end += len;
	return req->buf_end;
}

int req_flush(request *req)
{
	int len, ret;
	
	len = req->buf_end - req->buf_start;
	if(req->status == DEAD)
		return -2;
	
	if(len)
	{
		ret = write(req->fd, req->buf + req->buf_start, len);
		
		if(ret < 0)
		{
			if(errno == EWOULDBLOCK || errno == EAGAIN)
				return -1;
			else
			{
				req->buf_start = req->buf_end = 0;
				if(errno != EPIPE)
					;
				req->status = DEAD;
				req->buf_end = 0;
				return -2;
			}
		}

		req->buf_start += len;
	}
	if(req->buf_start == req->buf_end)
		req->buf_start = req->buf_end = 0;

	return req->buf_end;
}


void send_r_request_ok(request *req)
{
	//req->response_status = R_REQUEST_OK;
	
	//if(req->simple)
	//	return;
	
	req_write(req, "HTTP/1.1 200 OK\r\n");
	//req_flush(req);
	//DEBUG("%d", req->fd);
	//write(req->fd, "HTTP/1.1 200 OK\r\n\r\n", strlen("HTTP/1.1 200 OK\r\n\r\n"));
	//send(req->fd, "HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"), 0);
	

	//DEBUG("send_r_request_ok");

	//fflush(req->fd);
}

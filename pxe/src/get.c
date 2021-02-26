#include "base.h"
#include "httpd.h"
#include "mmap_cache.h"

#if 0
/*
 * Name: init_get
 * Description: Initializes a non-script GET or HEAD request.
 *
 * Return values:
 *   0: finished or error, request will be freed
 *   1: successfully initialized, added to ready queue
 */

int init_get(request *req)
{
	int data_fd;
	struct stat st;
	volatile int size;

	DEBUG("%s", req->path_name);
	data_fd = open(req->path_name, O_RDONLY);
	
	if(data_fd == -1)
	{
		if(errno == ENOENT)
			;//send_r_not_found(req);
		else if(errno == EACCES)
			;//send_r_not_found(req);
		else 
			;//send_r_bad_request();			
		return 0;
	}

	fstat(data_fd, &st);	

	/* directory */
	if(S_ISDIR(st.st_mode))	
	{


	}
	
	if(req->if_modified_since && 
		!modified_since(&(st.st_mtime), req->if_modified_since))
	{
		//send_r_not_modified(req);
		close(data_fd);
		return 0;
	}

	req->file_size = st.st_size;
	req->last_modified = st.st_mtime;

	if(req->method == M_HEAD || req->file_size == 0)
	{
		send_r_request_ok(req);
		close(data_fd);
		return 0;
	}
	if(req->file_size > MAX_FILE_MMAP)
	{
		send_r_request_ok(req);	//All's well
		//req->
		return 1;
	}
	
	req->mmap_entry_var = find_mmap(data_fd, &st);
	if(req->mmap_entry_var == NULL)
	{

	}
	req->data_mem = req->mmap_entry_var->mmap;	
	close(data_fd);
	
	DEBUG("req->data_mem %d", (long)req->data_mem);
	if((long) req->data_mem == -1)
	{
		DEBUG("mmap error");
		return 0;
	}

	DEBUG("send_r_request ");
	send_r_request_ok(req);
	
	size = MAX_BUFLEN - req->buf_end;

	if(size > 0)
	{
		if(size > req->file_size)
			size = req->file_size;
		memcpy(req->buf + req->buf_end, req->data_mem, size);
		DEBUG("req->buf %s", req->buf + req->buf_end);

		req->buf_end += size;
		req->file_pos += size;
		if(req->file_size == req->file_pos)
		{
			req_flush(req);
			req->status = DONE;
		}
	}
	return 1;
}

int process_get(request *req)
{



}
#endif

#include "base.h"
#include "mmap_cache.h"

int mmap_list_entries_used = 0;
int mmap_list_total_requests = 0;
int mmap_list_hash_bounces = 0;

/* define local table variable */
static struct mmap_entry mmap_list[MMAP_LIST_SIZE];

#ifdef _WIN32

#define MAP_FILE FILE_MAP_READ
#define MAP_PRIVATE FILE_MAP_READ
#define PROT_READ 0

static void* mmap(HANDLE *handle, void* start,size_t length,int prot,int flags,int fd,off_t offset)
{
#if 0
	void *data;
	HANDLE h;

	h = CreateFileMapping(fd, NULL, PAGE_READONLY,
						(DWORD) ((uint64_t) length >> 32), 
						(DWORD) (length & 0xffffffff),
						NULL);
	if(h)
	{
		return NULL;

	}

	data = MapViewOfFile(h , FILE_MAP_READ,
							 (DWORD) ((uint64_t) offset >> 32),
							(DWORD) (offset & 0xffffffff),
							length);
	if(data == NULL)
	{
		 CloseHandle(h);
	}
	
	return data;
#endif

#if 0
	HANDLE dumpFileDescriptor = CreateFileA("./Makefile",
                      GENERIC_READ | GENERIC_WRITE,
                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
	HANDLE fileMappingObject = CreateFileMapping((HANDLE)dumpFileDescriptor,
                      NULL,
                      PAGE_READWRITE,
                      0,
                      0,
                      NULL);
	void* mappedFileAddress = MapViewOfFile(fileMappingObject,
                      FILE_MAP_ALL_ACCESS,
                      0,
                      0,
                      length);
	return mappedFileAddress;
#endif
#endif
}


static int munmap(void *start, size_t length)
{
#if 0
	int ret;

    if (UnmapViewOfFile(data) == 0) {
        rc = NGX_ERROR;
    }

    if (CloseHandle(dumpFileDescriptor) == 0) {
        rc = NGX_ERROR;
    }

    return rc;
#endif
}



#endif




struct mmap_entry *find_mmap(int data_fd, struct stat *s)
{
	char *m;
	int i, start;
	mmap_list_total_requests++;
	i = start = MMAP_LIST_HASH(s->st_dev, s->st_ino, s->st_size);
	for(; mmap_list[i].use_count;)
	{
		if(mmap_list[i].dev == s->st_dev &&
			mmap_list[i].ino == s->st_ino &&
			mmap_list[i].len == s->st_size)
		
		{
			mmap_list[i].use_count ++;

			return mmap_list + i;
		}
		mmap_list_hash_bounces++;	
		i = MMAP_LIST_NEXT(i);	
		
		if(i == start)
			return NULL;
	}

	if(mmap_list_entries_used > MMAP_LIST_USE_MAX)
		return NULL;

	m = mmap(0, s->st_size, PROT_READ, MAP_FILE|MAP_PRIVATE, data_fd, 0);

	if((int) m == -1)
		return NULL;

	mmap_list_entries_used ++;
	mmap_list[i].dev = s->st_dev;
	mmap_list[i].ino = s->st_ino;
	mmap_list[i].len = s->st_size;
	mmap_list[i].mmap = m;
	mmap_list[i].use_count = 1;
	
	return mmap_list + i;
}

void release_mmap(struct mmap_entry *e)
{
	if(!e)
		return;
	if(!e->use_count)
	{
		DEBUG("mmap_list(%p)->use_count already zero",  e);
		return;
	}
	
	if(!--(e->use_count))
	{
		munmap(e->mmap, e->len);
		mmap_list_entries_used --;
	}
}

struct mmap_entry *find_named_mmap(char *fname)
{
	int data_fd;
	struct stat st;
	struct mmap_entry *e;
	data_fd = open(fname, O_RDONLY);
	if(data_fd == -1)
	{
		DEBUG("no found %s file", fname);
		return NULL;
	}	
	fstat(data_fd, &st);
	if(S_ISDIR(st.st_mode))
	{
		DEBUG("%s is a directory", fname);
		return NULL;
	}

	e = find_mmap(data_fd, &st);
	close(data_fd);
	return e;
}


int main(int argc, char *argv[])
{
    int data_fd;
    struct stat st; 

#ifdef _WIN32
    data_fd = open("Makefile", O_RDONLY);
#else
	data_fd = 
#endif

    fstat(data_fd, &st);
    struct mmap_entry *entry = find_mmap(data_fd, &st);

    char buf[1024 * 1024] = {0};
    if(entry)
    {   
        memcpy(buf, entry->mmap, entry->len);
        DEBUG("buf %s", buf);


        memset(buf, 0, sizeof(buf));
        memcpy(buf, entry->mmap, entry->len);
        DEBUG("buf %s", buf);
    
    }   
    return 0;

}

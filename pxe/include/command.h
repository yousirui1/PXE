#ifndef __COMMAND_H__
#define __COMMAND_H__


struct command {
	char *name;
	int type;
	void (*action)(char *, char *, void *);
	void *object;	
};


#endif

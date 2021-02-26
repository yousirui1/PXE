#include "base.h"

/* pipe */
int pipe_event[2] = {0};
int pipe_udp[2] = {0};
int pipe_tcp[2] = {0};
int pipe_ui[2] = {0};


void close_pipe()
{
    close_fd(pipe_event[0]);
    close_fd(pipe_event[1]);
    close_fd(pipe_tcp[0]);
    close_fd(pipe_tcp[1]);
    close_fd(pipe_udp[0]);
    close_fd(pipe_udp[1]);
}


int init_pipe()
{
    int ret;
#ifdef _WIN32
    int listenfd = 0;
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);

    listenfd = create_tcp();
    ret = bind_socket(listenfd, 22001);
    if(SUCCESS != ret)
    {   
        FAIL();
        return ERROR;
    }

    pipe_event[1] = create_tcp();
    ret = connect_server(pipe_event[1], "127.0.0.1", 22001);
    if(SUCCESS != ret)
    {   
        FAIL();
        return ERROR;
    }

    pipe_event[0] = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    close_fd(listenfd);

    listenfd = create_tcp();
    ret = bind_socket(listenfd, 22002);
    if(SUCCESS != ret)
    {   
        FAIL();
        return ERROR;
    }
    pipe_tcp[1] = create_tcp();
    ret = connect_server(pipe_tcp[1], "127.0.0.1", 22002);
    if(SUCCESS != ret)
    {   
        FAIL();
        return ERROR;
    }

    pipe_tcp[0] = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    close_fd(listenfd);

    listenfd = create_tcp();
    ret = bind_socket(listenfd, 22003);
    if(SUCCESS != ret)
    {
        FAIL();
        return ERROR;
    }

    pipe_udp[1] = create_tcp();
    ret = connect_server(pipe_udp[1], "127.0.0.1", 22003);
    if(SUCCESS != ret)
    {
        FAIL();
        return ERROR;
    }

    pipe_udp[0] = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    close_fd(listenfd);
#else
    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_tcp) < 0)
    {
        DEBUG("create server pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_tcp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_tcp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_udp) < 0)
    {
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_udp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_udp[1], F_SETFL, O_NONBLOCK);
    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_event) < 0)
    {
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_event[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_event[1], F_SETFL, O_NONBLOCK);
#endif
    return SUCCESS;
}


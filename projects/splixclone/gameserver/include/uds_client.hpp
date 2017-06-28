#ifndef _UDS_CLIENT_H_
#define _UDS_CLIENT_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


class uds_client
{

    public:
        
        uds_client(const char *socket_path)
        {
            if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                perror("socket error");
                exit(-1);
            }
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        }

        void sconnect()
        {
            if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                perror("connect error");
                exit(-1);
            }
        }

        void write_string(const char *msg)
        {
            ssize_t w;
            strcpy(buf,msg);
            strcat(buf,"\n");
            size_t n = strlen(buf);
            if ((w = write(fd, buf, n)) != (ssize_t) n) {
              if (w > 0) fprintf(stderr,"partial write");
              else {
                perror("write error");
                exit(-1);
              }
            }
        }

        void read_string()
        {
            ssize_t rc=read(fd,buf,sizeof(buf));
            if (rc > 0)
            {
                buf[rc]='\0'; 
            }
            if (rc == -1) {
              perror("read");
              exit(-1);
            }
            else if (rc == 0) {
              printf("read EOF\n");
            }
        }

 
    public: 
        struct sockaddr_un addr;
        char buf[1024];
        int fd;
};


/*
int main(int argc, char *argv[])
{
    uds_client client("/tmp/LocalSocketServerTest");
    client.sconnect();
    client.write_string("hello");
    client.read_string();
    printf("read string: '%s'", client.buf);
    return 0;
}
*/


#endif


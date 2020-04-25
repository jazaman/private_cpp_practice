#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <iostream>

using std::string;

int main (int argc, char *argv[])
{
    int ret;
    char buf[50];
    int socket_,fd; //n
    struct sockaddr_in p;
    struct hostent *h;
    //char *str = argv[1];

    //fd = open (str,O_CREAT | O_WRONLY,S_IRUSR|S_IWUSR);
    h = gethostbyname("127.0.0.1");

    memset(&p,0,sizeof(p));
    p.sin_family = AF_INET;
    p.sin_addr.s_addr = htonl(INADDR_ANY);
    p.sin_addr.s_addr = ((struct in_addr*)(h->h_addr))->s_addr;
    p.sin_port = htons(1234);

    socket_ = socket (AF_INET,SOCK_STREAM,0);
    connect (socket_,(sockaddr*)&p,sizeof(p));
    //n = write (socket_,str,strlen(str));
    std::string message = "Hello";
    write (socket_,message.c_str(),message.length());
    std::cout << "W -> ";
    size_t n = read (socket_,buf,40);
    std::cout << buf << " n->" << n << std::endl;

    //int idx = 0;

    /*do {
        n = read (s,buf,40);
        std::cin >> message;
        idx += n;
        printf(".");
        fflush(stdout);

    } while (n > 0);*/
    //close (fd);
    close (socket_);

    return 0;
}

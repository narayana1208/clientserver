#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
 
int main()
{
    int sfd, cfd;
    socklen_t len;
    char ch, buff[INET_ADDRSTRLEN];
    struct sockaddr_in saddr, caddr;
 
    sfd= socket(AF_INET, SOCK_STREAM, 0);
 
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(1205);
 
    bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
 
    listen(sfd, 5);
    signal(SIGCHLD, SIG_IGN);
 
    while(1) {
        printf("Server waiting\n");
        len=sizeof(caddr);
        cfd=accept(sfd, (struct sockaddr *)&caddr, &len);
 
        if( fork() == 0) {
            printf("Child Server Created Handling connection with %s\n",
                inet_ntop(AF_INET, &caddr.sin_addr, buff, sizeof(buff)));
 
            close(sfd);
 
            if(read(cfd, &ch, 1)<0) perror("read");
 
            while( ch != EOF) {
                if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')) 
                ch^=0x20;   
                    /* EXORing 6th bit will result in change in case */
 
                if(write(cfd, &ch, 1)<0) perror("write");
 
                if(read(cfd, &ch, 1)<0) perror("read"); 
            }
            close(cfd);
            return 0;
 
        }
 
        close(cfd);
    }
}

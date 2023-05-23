#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
int main(int argc, char *argv[]){
    if (argc !=4){
         
        printf("Sai so luong tham so.\n");
        return 1;
    }
    
    // Tao UDP Socket nhan
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Khai bao dia chi ben nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Khai bao dia chi cho du lieu den
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = inet_addr(argv[1]);
    addr1.sin_port = htons(atoi(argv[3]));  

    bind(receiver,(struct sockaddr *)&addr1,sizeof(addr1));  
    struct pollfd fds[2];
    int nfds =1;
    fds[0].fd =  receiver;
    fds[0].events = POLLIN;
    fds[1].fd = STDIN_FILENO;
    fds[1].events= POLLIN;

    char buf[256];
    while (1)
    {
         int ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("poll() failed");
            break;
        }

        // printf("ret = %d\n", ret);
        if (fds[0].revents & POLLIN){
            int rec = recvfrom(receiver,buf,sizeof(buf),0,NULL,NULL);
            if (rec<=0){
             printf("recvfrom() failed\n");
             break;   
            }
            else {
                buf[rec]=0;
                char check[64];
                printf("%s",buf);
            }
        }
        if (fds[1].revents & POLLIN){
            fgets(buf,sizeof(buf),stdin);
            sendto(sender,buf,strlen(buf),0,(struct sockaddr *)&addr,sizeof(addr));
        }
    }
    close(sender);
    close(receiver);
}
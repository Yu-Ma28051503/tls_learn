/*
 * UDP Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 50000
#define IP "127.0.0.1"

int main(void)
{
    int sockfd;
    char recv_msg[BUF_SIZE];
    char send_msg[BUF_SIZE] = "UDP Client Hello";
    int str_len;

    struct sockaddr_in servAddr;
    socklen_t servAddrlen;

    /* ソケットを作成する */
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    /* ソケットアドレスを設定する */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(IP); // 32bit IPv4 address
    servAddr.sin_port = htons(PORT);
    servAddrlen = sizeof(servAddr);

    /* サーバにメッセージを送信する */
    if(sendto(sockfd, send_msg, strlen(send_msg), 0, (struct sockaddr *)&servAddr, servAddrlen) == -1) {
        perror("sendto() error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /* サーバからのメッセージを受信する */
    str_len = recvfrom(sockfd, recv_msg, BUF_SIZE, 0, NULL, NULL);
    if(str_len == -1) {
        perror("recvfrom() error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Message from server: %s\n", recv_msg);

    close(sockfd);

    return 0;
}

/*
 * UDP Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define PORT 50000

int main(void)
{
    int sockfd;
    char recv_msg[BUF_SIZE];
    char send_msg[BUF_SIZE] = "UDP Server Hello";
    int str_len;

    struct sockaddr_in servAddr, clientAddr;
    socklen_t clientAddrlen = sizeof(clientAddr);

    /* ソケットを作成する */
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    /* ソケットアドレスを設定する */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 32bit IPv4 address
    servAddr.sin_port = htons(PORT);

    /* ソケットにソケットアドレスを割り当てる */
    if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        perror("bind() error");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connection at %d port...\n", PORT);

    /* クライアントからのメッセージを受信する */
    str_len = recvfrom(sockfd, recv_msg, BUF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrlen);  // 受信
    if(str_len == -1) {
        perror("recvfrom() error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    recv_msg[str_len] = '\0';
    printf("Message from client: %s\n", recv_msg);

    /* クライアントにメッセージを送信する */
    if (sendto(sockfd, send_msg, sizeof(send_msg), 0, (struct sockaddr *)&clientAddr, clientAddrlen) == -1) {
        perror("sendto() error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    return 0;
}

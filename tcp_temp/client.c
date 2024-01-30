// TCP connection client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define IP "127.0.0.1"
#define DEFAULT_PORT 50000

int main(void)
{
    int sockfd;	// client socket
    char message[BUF_SIZE] = "Client Hello";
    int str_len;
    struct sockaddr_in servAddr;

    /* ソケットを作成する */
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* ソケットアドレスを設定する */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;                 // IPv4のアドレスを使う
    servAddr.sin_port = htons(DEFAULT_PORT);       // ポート番号

    /* サーバのIPアドレスを設定する */
    if(inet_aton(IP, &servAddr.sin_addr) == 0) {
        perror("inet_aton");
        exit(EXIT_FAILURE);
    }

    /* サーバに接続する */
    if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    /* サーバにメッセージを送信する */
    if(write(sockfd, message, strlen(message)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* サーバからのメッセージを受信する */
    if((str_len = read(sockfd, message, BUF_SIZE)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Message from server: %s\n", message);

    /* ソケットを閉じる */
    close(sockfd);

    return 0;
}

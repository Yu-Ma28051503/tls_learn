// TCP connection server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define DEFAULT_PORT 50000

int main(void)
{
    int sockfd;  // server socket
    int connect = 0;
    char recv[BUF_SIZE];
    char send[BUF_SIZE] = "Server Hello";

    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;
    socklen_t servAddrLen = sizeof(servAddr);
    socklen_t clientAddrLen = sizeof(clientAddr);

    /* ソケットを作成する */
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* ソケットアドレスを設定する */
    memset(&servAddr, 0, servAddrLen);
    servAddr.sin_family = AF_INET;                 // IPv4のアドレスを使う
    servAddr.sin_port = htons(DEFAULT_PORT);       // ポート番号
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 受け入れるIPアドレスの範囲を設定(どのIPアドレスからの接続も受け付ける)

    /* ソケットにソケットアドレスを割り当てる */
    if(bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* コネクト要求をいくつまで待つかを設定する */
    if(listen(sockfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connection at %d port...\n", DEFAULT_PORT);

    /* コネクト要求を待つ */
    memset(&clientAddr, 0, sizeof(clientAddr));
    if((connect = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Connected from %s\n", inet_ntoa(clientAddr.sin_addr));

    /* クライアントからのメッセージを受信する */
    if(read(connect, recv, BUF_SIZE) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Message from client: %s\n", recv);

    /* クライアントにメッセージを送信する */
    if(write(connect, send, sizeof(send)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* ソケットを閉じる */
    close(connect);
    close(sockfd);

    return 0;
}

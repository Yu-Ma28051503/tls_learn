// TCP connection client
// TLS connection client
// using OpenSSL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// OpenSSL library
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUF_SIZE 1024
#define IP "127.0.0.1"
#define DEFAULT_PORT 50000

int main(void)
{
    int sockfd;	// client socket
    char serv_msg[BUF_SIZE] = "Client Hello";
    char recv_msg[BUF_SIZE];
    int str_len;
    struct sockaddr_in servAddr;

    /* OpenSSL library */
    SSL_CTX *ctx;
    SSL *ssl;

    /* SSLライブラリの初期化 */
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if(ctx == NULL) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    /* ソケットを作成する */
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    /* ソケットアドレスを設定する */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;                 // IPv4のアドレスを使う
    servAddr.sin_port = htons(DEFAULT_PORT);       // ポート番号

    /* サーバのIPアドレスを設定する */
    if(inet_aton(IP, &servAddr.sin_addr) == 0) {
        perror("inet_aton");
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    /* サーバに接続する(TCPコネクション) */
    if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        perror("connect");
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    /* SSL通信のセットアップ */
    ssl = SSL_new(ctx);  // 新しいSSL構造体を作成
    SSL_set_fd(ssl, sockfd);  // ソケットとSSL構造体を関連付ける

    /* SSL通信の確立(ハンドシェイク) */
    if(SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    /* サーバにメッセージを送信する */
    str_len = SSL_write(ssl, serv_msg, strlen(serv_msg));
    if(str_len < 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    /* サーバからメッセージを受信する */
    str_len = SSL_read(ssl, recv_msg, sizeof(recv_msg));
    if(str_len < 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    printf("Message from server: %s\n", recv_msg);

    /* SSL通信の終了 */
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}

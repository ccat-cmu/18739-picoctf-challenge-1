#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h> 
#include <sys/socket.h>

#define FLAG_BUFFER_SIZE 40
#define PACKAGE_BUFFER_SIZE 512
#define MESSAGE_BUFFER_SIZE 32

#define TRUSTED_IP "10.0.0.10"

// int read_flag(char* flag) {
//     FILE *fp = fopen("/app/flag.txt", "r");
//     if (fp == NULL)
//         return -1;

//     size_t bytes_read = fread(flag, FLAG_BUFFER_SIZE - 1, 1, fp);
//     if (bytes_read != FLAG_BUFFER_SIZE - 1) {
//         fclose(fp);
//         return -1;
//     }

//     flag[FLAG_BUFFER_SIZE - 1] = '\0';
//     return 0;
// }

int main() {
    fprintf(stderr, "Server started\n");

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    fprintf(stderr, "Server socket created\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5555);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    struct sockaddr_in bound_addr;
    socklen_t bound_len = sizeof(bound_addr);
    if (getsockname(sock, (struct sockaddr *)&bound_addr, &bound_len) == 0) {
        fprintf(stderr, "Server listening on %s:%d\n",
            inet_ntoa(bound_addr.sin_addr),
            ntohs(bound_addr.sin_port));
    } else {
        perror("getsockname");
    }

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    fprintf(stderr, "Server socket bound\n");
    fflush(stderr);

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(struct timeval));

    char buf[4096];
    struct sockaddr_in client_addr;
    while (1) {
        socklen_t addrlen = sizeof(client_addr);
        int n = recvfrom(sock, buf, sizeof(buf)-1, 0,
                        (struct sockaddr*)&client_addr, &addrlen);

        buf[n] = '\0';
        fprintf(stderr, "Received: %s\n", buf);
        //if (n < 0) perror("recv");

        // int n = recvfrom(sock, buf, sizeof(buf)-1, MSG_PEEK,
        //                  (struct sockaddr*)&client_addr, &addrlen);

        // n = recvfrom(sock, buf, sizeof(buf)-1, 0,
        //                  (struct sockaddr*)&client_addr, &addrlen);

        fprintf(stderr, "Server received a packet\n");
        // if (n == 0) { perror("no data received"); continue; }
        // if (n < 0) { perror("recvfrom"); continue; }

        // buf[n] = '\0';
        // fprintf(stderr, "Received from %s:%d: %s\n",
        //        inet_ntoa(client_addr.sin_addr),
        //        ntohs(client_addr.sin_port),
        //        buf);

        // // Respond back to the client IP and port
        // const char *resp = "Hello from server";
        // int sent = sendto(sock, resp, strlen(resp), 0,
        //                   (struct sockaddr*)&client_addr, addrlen);
        // if (sent < 0) {
        //     perror("sendto");
        // }
    }



    // char flag[48];
    // const char *peer_ip;
    // char packet_buffer[512];
    // char message_buffer[32];
    // bool unprivileged = true;

    // setvbuf(stderr, NULL, _IONBF, 0);
    // fprintf(stderr, "Starting server");
    // fwrite("Starting server", strlen("Starting server"), 1, stdout);

    // size_t flag_n = read_flag(flag);
    // if (flag_n == -1)
    //     return -1;

    // /* Verify trusted source */
    // peer_ip = getenv("SOCAT_PEERADDR");
    // if (strcmp(peer_ip, TRUSTED_IP) != 0) {
    //     const char *response = "Not trusted source.\n";
    //     fwrite(response, strlen(response), 1, stdout);
    //     return 0;
    // }

    // /* Verify package format */
    // size_t n = fread(packet_buffer, 1, sizeof(packet_buffer)-1, stdin);
    // packet_buffer[n] = '\0';
    // if (n < 5 || strncmp((char*)packet_buffer, "MSG=", 4) != 0) {
    //     const char *response = "Malformed package, expecting MSG=<message>.\n";
    //     fwrite(response, strlen(response), 1, stdout);
    //     return 0;
    // }

    // /* Parse message */
    // char *message = (char*)packet_buffer + 4;
    // strncat(message_buffer, message, (size_t)MESSAGE_BUFFER_SIZE);

    // /* Check privilage */
    // if (unprivileged) {
    //     fwrite(message, strlen(message), 1, stdout);
    //     return 0;
    // } else {
    //     fwrite(flag, strlen(flag), 1, stdout);
    // }

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FLAG_BUFFER_SIZE 48
#define PACKAGE_BUFFER_SIZE 512
#define MESSAGE_BUFFER_SIZE 32

#define TRUSTED_IP "10.0.0.10"

int read_flag(char* flag) {
    FILE *fp = fopen("/app/flag.txt", "r");
    if (fp == NULL)
        return -1;

    size_t bytes_read = fread(flag, FLAG_BUFFER_SIZE - 1, 1, fp);
    if (bytes_read != FLAG_BUFFER_SIZE - 1) {
        fclose(fp);
        return -1;
    }

    flag[FLAG_BUFFER_SIZE - 1] = '\0';
    return 0;
}

int main() {
    char flag[48];
    const char *peer_ip;
    char packet_buffer[512];
    char message_buffer[32];
    bool unprivileged = true;

    setvbuf(stderr, NULL, _IONBF, 0);
    fprintf(stderr, "Starting server");
    fwrite("Starting server", strlen("Starting server"), 1, stdout);

    size_t flag_n = read_flag(flag);
    if (flag_n == -1)
        return -1;

    /* Verify trusted source */
    peer_ip = getenv("SOCAT_PEERADDR");
    if (strcmp(peer_ip, TRUSTED_IP) != 0) {
        const char *response = "Not trusted source.\n";
        fwrite(response, strlen(response), 1, stdout);
        return 0;
    }

    /* Verify package format */
    size_t n = fread(packet_buffer, 1, sizeof(packet_buffer)-1, stdin);
    packet_buffer[n] = '\0';
    if (n < 5 || strncmp((char*)packet_buffer, "MSG=", 4) != 0) {
        const char *response = "Malformed package, expecting MSG=<message>.\n";
        fwrite(response, strlen(response), 1, stdout);
        return 0;
    }

    /* Parse message */
    char *message = (char*)packet_buffer + 4;
    strncat(message_buffer, message, (size_t)MESSAGE_BUFFER_SIZE);

    /* Check privilage */
    if (unprivileged) {
        fwrite(message, strlen(message), 1, stdout);
        return 0;
    } else {
        fwrite(flag, strlen(flag), 1, stdout);
    }

}
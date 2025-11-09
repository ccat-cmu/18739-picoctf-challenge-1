# Unlock Canary
  - Namespace: 18739ctf
  - ID: unlock-canary
  - Type: custom
  - Category: Binary
  - Points: 300
  - Templatable: yes

## Description

Canary has been added to the stack against overwrite to protect our echo server! Can you leak it?

## Details

Connect to the remote service with netcat:

`$ nc {{server}} {{port}}`

## Hints

- Do you know format string attack?
- The first few parameters are stored in register on a 64-bit machine, what if we need more parameters?
- ASLR, PIE and compiler stack protector are disabled.

## Tags

- medium

## Solution Overview

This shows how secret, or specifically canary here, can be leaked 
with format string vulnerability and the understanding of parameter
placement is 64-bit machines. Although actual number of registers
used for parameters are different on each architecture, the stack 
frame is relatively small which allows quick probing to locate
the canary stored as a local variable. 

With the correct offset, win() function address and the canary value,
the flag can be obtained when the control flow is hijacked. 

## Attributes
- author: Jou-Yin Huang
- event: 18739-ctf

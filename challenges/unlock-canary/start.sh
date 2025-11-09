#!/bin/bash

set -e

# socat UDP-RECVFROM:5555,reuseaddr,fork UDP-SENDTO:host.docker.internal:50664
socat tcp-listen:5555,reuseaddr,fork EXEC:/challenge/unlockcanary
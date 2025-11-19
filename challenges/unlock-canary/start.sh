#!/bin/bash

set -e

socat tcp-listen:5555,reuseaddr,fork EXEC:/challenge/unlockcanary

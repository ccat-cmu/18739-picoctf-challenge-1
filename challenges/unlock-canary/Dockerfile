FROM debian:bullseye AS challenge

RUN apt-get update \
 && apt-get install -y --no-install-recommends python3 socat gcc make build-essential \
 && rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

WORKDIR /app

COPY . . 
COPY start.sh /opt/
RUN chmod +x /opt/start.sh

RUN mkdir /challenge && chmod 700 /challenge
COPY setup-challenge.py /challenge/setup-challenge.py
COPY unlockcanary.c /challenge/unlockcanary.c

RUN gcc -std=c11 -O0 -fno-stack-protector -no-pie -o /challenge/unlockcanary /challenge/unlockcanary.c

ARG FLAG="CTF{default_flag}"
RUN chmod +x /challenge/setup-challenge.py
RUN /challenge/setup-challenge.py

# PUBLISH 5555 AS nc
EXPOSE 5555/udp
CMD ["/opt/start.sh"]

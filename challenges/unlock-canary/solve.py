from scapy.all import *
from scapy.all import IP, UDP

# HOST = "0.0.0.0"
HOST = "0.0.0.0"
PORT = 58016
# SOURCE_IP = "10.0.0.10"
# SOURCE_PORT = 5500
# BIND_IP = "100.110.145.17"
# BIND_PORT = 65535

def send_package(msg: str):
    payload = "MSG=" + msg

    # s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # local_ip = s.getsockname()[0]
    # s.bind((local_ip, 0))
    # s.bind(("0.0.0.0", 0))
    # local_ip, local_port = s.getsockname()
    # source_ip = s.getsockname()[0]
    # #source_port = s.getsockname()[1]
    # source_port = 50664

    # ip_layer = IP(dst=HOST, src=SOURCE_IP)
    ip_layer = IP(dst=HOST)
    udp_layer = UDP(dport=PORT)
    packet = ip_layer / udp_layer / Raw(load=payload.encode())
   

    #sr1 sends the packet and waits for one reply
    reply = sr1(packet, timeout=2) 
    if reply:
        print("Reply:", reply.summary())
        if Raw in reply:
            print("Payload:", reply[Raw].load.decode(errors="replace"))
    else:
        print("No reply received")

    send(IP(dst=HOST)/UDP(dport=PORT)/Raw(load=payload))
    pkts = sniff(filter=f"udp and src {HOST} and dst port 5555", timeout=2)
    print(pkts)
    
    # s.sendto(payload.encode(), (HOST, PORT))

    # # send(package)
    # s.sendto(payload.encode(), (HOST, PORT))
    # print(f"UDP packet to {HOST}:{PORT} from {local_ip}:{local_port} with payload {payload} sent.")

    # data, addr = s.recvfrom(1024)
    # print("Reply:", data, "from", addr)

# def receive_response():
#     s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#     s.bind((BIND_IP, BIND_PORT))

#     data, addr = s.recvfrom(5500)
#     print(f"Received {len(data)} bytes from {addr}")
#     # Print details
#     try:
#         data_str = data.decode(errors="replace")
#     except Exception:
#         data_str = repr(data)
#     print("Payload (decoded):")
#     print(data_str)
#     s.close()

def main():
    send_package("test")
    #receive_response()

if __name__ == "__main__":
    main()

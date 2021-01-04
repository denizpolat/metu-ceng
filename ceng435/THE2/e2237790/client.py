import socket
import time  # to calculate avg && total transmission time
import sys
import struct  # used in encoding messages


# this if check is something that ive found online for preventing BrokenPipe error (bec of my os)
if __name__ == "__main__":

    # take args from command line
    host = sys.argv[1]
    udplistenport = int(sys.argv[2])
    tcplistenport = int(sys.argv[3])
    udpsendport = int(sys.argv[4])
    tcpsendport = int(sys.argv[5])

    try:
        soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # socket.socket()
    except socket.error as err:
        print("Failed! Error %s" % err)
    hhh = ("127.0.0.1", tcpsendport)

    host1 = (host, tcplistenport)

    soc.bind(hhh) # bind socket to local machine in order to use the correct ports

    soc.connect(host1) # connection between server && client
    # soc.send("Socket connected")

    filename = "transfer_file_TCP.txt"
    with open(filename, "rb") as file:
        num = 0
        while True:
            chunk = file.read(1000) # chunks in size 1000
            if chunk == b"" or chunk is None or not chunk: # if no more chunks
                break

            tmr = time.time()

            # print(f"Chunk {num} time {tmr}")

            message = struct.pack("d", tmr) + chunk # pack time info into chunks

            soc.sendto(message, host1) # send time+chunk info to server

            num = num+1

            # time.sleep(1)  # try 1 sec

    soc.close() # close socket
    print("connection closed.")

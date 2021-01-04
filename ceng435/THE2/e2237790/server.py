import socket
import time
import sys
import struct


# this if check is something that ive found online for preventing BrokenPipe error (bec of my os)
if __name__ == "__main__":

    # take args from command line
    udplistenport = int(sys.argv[1])
    tcplistenport = int(sys.argv[2])

    #create socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    host = ("127.0.0.1", tcplistenport) # local

    s.bind(host) # bind to local using appropriate port

    # s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # not needed

    s.listen(3) # wait client connection
    print("Server is listening...")

    packageCnt = 0
    totalTime = 0
    filename = "received_file_TCP.txt"
    connection, address = s.accept() # establist connection

    with open(filename, "wb") as file:
        while True:
            data = connection.recv(1008) # receive data
            # if data == "end33":
            #    print("end33")
            #    break

            if not data or data == b"" or data is None: # if no more data
                break


            tmr = (time.time() - struct.unpack("d", data[0:8])[0]) * 1000 # decode embedded time info, take difference, convert ms
            packageCnt = packageCnt + 1
            totalTime += tmr

            file.write(data[8:]) # write rest of info (without time info)
        print(f"Total time : {totalTime} ms")
        print(f"Avg time : {totalTime/packageCnt} ms")

        connection.close()
        print("Connection closed")
    s.close()
    print("Socket closed")

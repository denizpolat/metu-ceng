
First, you need to run the command chmod +x <name> to execute the scripts.

then, run following commands:

- server.py <udplistenport> <tcplistenport>
- client.py <hostname> <udplistenport> <tcplistenport> <udpsenderport> <tcpsenderport>

client.py will read a file named transfer_file_TCP.txt and transfer it to a file named received_file_TCP.txt


First, I created a socket and made necessary connections and bindings. Then, I divided file into chunks in client.py. I also embedded time information into chunks. Then, in server.py, I received these chunks, and decoded to time info and the part of the file. Then I used the time info for time calculations and file to write.

I have followed the steps in the same order as I have written. It took 2-3 days to implement this part. After this study, I understand basics of socket programming, how to create a socket and make a connection between server and client.

It was very problematic for me to implement time information. Converting time.time() to something that I can transfer was hard to understand (and I couldnt handle it, I guess). 

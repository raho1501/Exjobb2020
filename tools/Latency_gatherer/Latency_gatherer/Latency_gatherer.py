import socket
import random
import string
from time import time_ns, sleep


#def randomString(stringLength=1024):
#    """Generate a random string of fixed length """
#    letters = string.ascii_lowercase
#    return ''.join(random.choice(letters) for i in range(stringLength))


#f = open("data.txt", "w")
#text = randomString(962)
#print(len(text))
#f.write(text)
#f.close()

f = open("data.txt", "r")
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_ip = "92.32.148.23"
server_socket = (server_ip, 6066)
payload = f.read()
payload = str.encode(payload)
f.close()
f = open("latency_results__ns.txt", "a")
runheader = input("Which testpoint is it?")
f.write(runheader+",")

for i in range(0,30):
    timepoint = time_ns()
    sock.sendto(payload ,server_socket)
    
    rec_data = sock.recvfrom(1024)
    timepoint_end = time_ns()
    RTT = timepoint_end-timepoint
    sleep(0.1)
    print(rec_data[1], '\n', RTT )
    f.write(str(RTT)+ ",")
    print("saved value for response: ", i)
f.write("\n")
f.close()


    
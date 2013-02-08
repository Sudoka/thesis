#!/usr/local/bin/python

import sys
import random
from scapy.all import *
from time import sleep
import subprocess
from ctypes import c_uint32

print("Hello, let's check if a target computer has poor entropy!\n")
print(sys.version)
#initialize command line arguments (TODO: need to sanitize input and automate ip grab)
local_ip = sys.argv[1]
remote_ip = sys.argv[2]
remote_port = sys.argv[3]
seq_create = sys.argv[4]
spoof_diff_calc = sys.argv[5]


for i in range(0, 2):
#initialize our ports for comparing seq #s
    local_port_1 = random.randint(49152, 65535)
    local_port_2 = random.randint(49152, 65535)
    while(local_port_1 == local_port_2):
        local_port_2 = random.randint(49152, 65535)



    #determine sequence numbers ahead-of-time to reduce delay before spoof
    #local_ip here is dest_ip for server
    base_seq_num_1 = subprocess.check_output([seq_create, remote_ip, remote_port, local_ip, str(local_port_1)])

    base_seq_num_2 = subprocess.check_output([seq_create, remote_ip, remote_port, local_ip, str(local_port_2)])



#create connection to server and record sequence number 1
    outbound_ip = IP(src=local_ip, dst=remote_ip)
    outbound_tcp = TCP(sport=int(local_port_1), dport=int(remote_port), flags="S", options=[('Timestamp', (0, 0))])
    packet = outbound_ip/outbound_tcp
    response = sr1(packet)

    response_seq_num_1 = int(response[TCP].seq)
    jiffie_reading_1 = int(response[TCP].options[3][1][0])

#create connection to server and record sequence number 2
    outbound_ip = IP(src=local_ip, dst=remote_ip)
    outbound_tcp = TCP(sport=int(local_port_2), dport=int(remote_port), flags="S", options=[('Timestamp', (0, 0))])
    packet = outbound_ip/outbound_tcp
    response = sr1(packet)

    response_seq_num_2 = int(response[TCP].seq)
    jiffie_reading_2 = int(response[TCP].options[3][1][0])

    jiffies_between_packets = jiffie_reading_2 - jiffie_reading_1
    print(jiffies_between_packets)
#calculate time component of each packet and subtract difference in sending time
    nano_weight = 62503.90625  #the amount we expect each read to differ for a change by 1 jiffie
    ns_between_packets = int(jiffies_between_packets * nano_weight)
    second_seq = subprocess.check_output([spoof_diff_calc, str(response_seq_num_1), str(base_seq_num_1), str(base_seq_num_2)])
    second_seq = int(second_seq)

    print("Seq Num Response for first connection:\t " + str(response_seq_num_1))
    print("-")
    print("The Seq Num w/out time for first connection\t " + str(base_seq_num_1))
    print "+"
    print("The Seq Num w/out time for second connection\t " + str(base_seq_num_2))
    print "+"
    print("The time between each connection\t " + str(ns_between_packets))
    print "\n"
    print("Equals " + str(second_seq + ns_between_packets) + "compared to actual number of " + str(response_seq_num_2))
    
    diff = abs((second_seq + ns_between_packets) - response_seq_num_2)
    
    print("Difference!\t" + str(diff))

    if(diff < 100):
        print("VULNERABLE!")
    
    else:
        print("NOT VULNERABLE!")


#sleep(.0001)



    







#calculate time from sequence number

#spoof!

